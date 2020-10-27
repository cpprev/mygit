#include "commands/commands.hh"

#include "objects/tree.hh"
#include "objects/blob.hh"

#include "utils/file_state.hh"
#include "utils/zlib.hh"
#include "utils/wrappers.hh"
#include "utils/utils.hh"
#include "utils/get_paths.hh"

namespace mygit
{
    void merge (int argc, char *argv[])
    {
        /// Create opt object
        auto opt = options::MergeOptions(argc, argv);

        /// Check ref/destination is a commit object
        utils::ExitIfTrue(not utils::IsCommitObject(opt.branchToMerge), "Not valid commit object.");

        /// If opt.branchToMerge is an ancestor of HEAD -> do nothing
        std::string hashCommitToMerge = utils::GetCommitHash(opt.branchToMerge);
        std::string hashCommitCurrent = utils::GetMostRecentCommit();

        utils::ExitIfTrue(hashCommitToMerge == hashCommitCurrent, "Nothing to merge");
        utils::ExitIfTrue(utils::IsCommitAncestorOf(hashCommitToMerge, hashCommitCurrent), opt.branchToMerge + " is an ancestor of HEAD, therefore cant be merged.");

        /// If HEAD is an ancestor of opt.branchToMerge -> Fast forward merge
        if (utils::IsCommitAncestorOf(hashCommitCurrent, hashCommitToMerge))
        {
            FastForwardMerge(hashCommitCurrent, hashCommitToMerge);
        }
        /// Recursive merge
        else
        {
            RecursiveMerge(opt, hashCommitCurrent, hashCommitToMerge);
        }
    }

    void FastForwardMerge (const std::string& hashCommitCurrent, const std::string& hashCommitToMerge)
    {
        std::cout << "[Fast forward merge ...]\n";

        /// Update Working Dir and Index
        utils::UpdateWorkingDirectoryAndIndex(hashCommitCurrent, hashCommitToMerge);

        /// Update HEAD
        std::string currentBranch = utils::GetCurrentBranch();
        if (not currentBranch.empty())
        {
            std::string pathToBranch = utils::PathToBranch(currentBranch);
            utils::WriteFile(pathToBranch, hashCommitToMerge);
        }
        /// Commit in head : detached head case
        else
        {
            utils::WriteFile(utils::PathToHEAD(), hashCommitToMerge);
        }
    }

    void RecursiveMerge (const options::MergeOptions& opt, const std::string& hashCommitCurrent, const std::string& hashCommitToMerge)
    {
        std::cout << "[Recursive merge ...]\n";

        /// Diffs from common ancestor
        std::string hashCommonAncestor = utils::GetCommonAncestor(hashCommitToMerge, hashCommitCurrent);
        utils::ExitIfTrue(hashCommonAncestor.empty(), "Common ancestor not found, unexpected behaviour.");

        std::string hashTreeCommonAncestor = utils::GetTreeHashFromCommit(hashCommonAncestor);
        std::string hashTreeToMerge = utils::GetTreeHashFromCommit(hashCommitToMerge);
        std::string hashTreeCurrent = utils::GetTreeHashFromCommit(hashCommitCurrent);

        std::map<std::string, std::string> entryCommonAncestor = objects::Tree::TreeHashToEntryMap(hashTreeCommonAncestor);
        std::map<std::string, std::string> entryToMerge = objects::Tree::TreeHashToEntryMap(hashTreeToMerge);
        std::map<std::string, std::string> entryCurrent = objects::Tree::TreeHashToEntryMap(hashTreeCurrent);

        /// Diff between entryCommonAncestor and entryCurrent
        std::map<std::string, std::string> inCurrent;
        std::map<std::string, utils::FileState> inCurrentStatus;
        utils::GetDiffBetweenTrees(entryCurrent, entryCommonAncestor, inCurrent, inCurrentStatus);

        std::map<std::string, std::string> inToMerge;
        std::map<std::string, utils::FileState> inToMergeStatus;
        utils::GetDiffBetweenTrees(entryToMerge, entryCommonAncestor, inToMerge, inToMergeStatus);

        /// FIXME TODEL
        for (const auto& add1 : inCurrent)
            std::cout << inCurrentStatus[add1.first] << ' ' << add1.first << ' ' << add1.second << '\n';
        std::cout << '\n';

        std::vector<std::string> conflicts;
        std::map<std::string, std::string> addToIndex;
        std::vector<std::string> rmFromIndex;
        for (const auto& add2 : inToMerge)
        {
            std::string add2File = add2.first;
            std::string add2Hash = add2.second;
            utils::FileState add2Status = inToMergeStatus[add2File];

            std::cout << add2Status << ' ' << add2File << ' ' << add2Hash << '\n';

            auto it = inCurrent.find(add2File);
            std::string pathFileFromDotMyGit = utils::CleanPath(utils::AppendPathToRootRepo(add2File));
            if (it == inCurrent.end())
            {
                //std::cout << "pathDotGit: " << pathFileFromDotMyGit << '\n';
                if (add2Status == utils::ADDED or add2Status == utils::MODIFIED)
                {
                    std::cout << ((add2Status == utils::ADDED) ? "Adding : " : "Modifying : ") << pathFileFromDotMyGit << '\n';

                    addToIndex.insert({add2File, add2Hash});

                    std::string pathFile = utils::PathToObjectFile(add2Hash);
                    std::string fileContents = objects::GetContentBlobDecompressed(utils::DecompressString(utils::ReadFile(pathFile)));
                    /// Add directories if needed
                    utils::CreateDirectoriesAboveFileReturnFirstToDelete(pathFileFromDotMyGit);
                    /// Write to file in case of Add/Modify
                    utils::WriteFile(pathFileFromDotMyGit, fileContents);
                }
                else if (add2Status == utils::DELETED)
                {
                    std::cout << "Removing :" << pathFileFromDotMyGit << '\n';

                    rmFromIndex.push_back(add2File);

                    remove(pathFileFromDotMyGit.c_str());

                    /// If dir containing file is empty, delete it
                    utils::DeleteDirectoryIfEmpty(pathFileFromDotMyGit);
                }
            }
            /// Conflict case
            else if (it->second != add2Hash or inCurrentStatus[it->first] != add2Status)
            {
                /// Cases possible : Added/Added ; Modified/Modified ; Modified/Deleted
                std::string pathFileCurrent = utils::PathToObjectFile(it->second);
                std::string pathFileToMerge = utils::PathToObjectFile(add2Hash);

                /// Case modified in Current and deleted in ToMerge
                /// Case modified in ToMerge and deleted in Current
                /// -> create file with contents modified
                if ((inCurrentStatus[it->first] == utils::MODIFIED and add2Status == utils::DELETED)
                or (inCurrentStatus[it->first] == utils::DELETED and add2Status == utils::MODIFIED))
                {
                    /// FIXME
                    /// Add directories if needed
                    utils::CreateDirectoriesAboveFileReturnFirstToDelete(pathFileFromDotMyGit);
                    /// Write to file in case of Add/Modify
                    //utils::WriteFile(pathFileFromDotMyGit, fileContents);
                }

                /// Case Added in Current and Added in ToMerge
                /// Case Modified in Current and Modified in ToMerge
                /// -> annoying case, make the diff in one file
                if ((inCurrentStatus[it->first] == utils::ADDED and add2Status == utils::ADDED)
                or (inCurrentStatus[it->first] == utils::MODIFIED and add2Status == utils::MODIFIED))
                {
                    std::string contentsInCurrent = objects::GetContentBlobDecompressed(utils::DecompressString(utils::ReadFile(pathFileCurrent)));
                    std::string contentsInToMerge = objects::GetContentBlobDecompressed(utils::DecompressString(utils::ReadFile(pathFileToMerge)));

                    std::cout << "ay: " << contentsInCurrent << "\n\n";

                    std::cout << "ay: " << contentsInToMerge << '\n';

                    std::vector<std::string> currentLines = utils::GetLinesAsVect(contentsInCurrent);
                    std::vector<std::string> toMergeLines = utils::GetLinesAsVect(contentsInToMerge);
                    std::vector<std::vector<int>> LCSMatrix = BuildLcsMatrix(currentLines, toMergeLines);
                    std::string result;
                    std::string beforeState;
                    bool after = false;
                    GetMergeDiff(opt.branchToMerge, LCSMatrix, currentLines, toMergeLines, currentLines.size(), toMergeLines.size(), result, beforeState, after);
                    if (after)
                        result += "==========\n";

                    std::cout << "\n_____________\nRESULT:\n" << result << "____________\n";
                }

                std::cout << "Conflict in file: " << add2File << '\n';
                conflicts.push_back(add2File);
            }

            /// Update Index file
            utils::AddOrRemoveElementsInIndex(addToIndex, rmFromIndex);

            /// No conflicts -> commit changes (with 2 parent commits)
            if (conflicts.empty())
            {
                options::CommitOptions commitOpt("Merge branch \'" + opt.branchToMerge + "\' into branch \'" + utils::GetCurrentBranch() + "\'.");
                /// FIXME When implementing multiple parents for a commit, uncomment this line
                //mygit::commit(commitOpt, hashCommitToMerge);
                mygit::commit_wrapper(commitOpt);
            }
            else
            {
                std::cout << "You need to resolve your conflicts first and then commit for the merge to complete.\n";
            }
        }
    }

    void GetMergeDiff (const std::string& mergeBranchName, const std::vector<std::vector<int>>& lookup, const std::vector<std::string>& x,
                               const std::vector<std::string>& y, int m, int n, std::string& result, std::string& beforeState, bool& after)
    {
        if (m > 0 and n > 0 and x[m - 1] == y[n - 1])
        {
            GetMergeDiff(mergeBranchName, lookup, x, y, m - 1, n - 1, result, beforeState, after);
            beforeState.clear();
            if (after)
            {
                result += "==========\n";
                after = false;
            }
            result += "\033[0m" + x[m - 1] + "\033[0m";
        }
        else if (n > 0 and (m == 0 || lookup[m][n - 1] >= lookup[m - 1][n]))
        {
            GetMergeDiff(mergeBranchName, lookup, x, y, m, n - 1, result, beforeState, after);

            after = true;
            if (beforeState != "add")
            {
                result += "<<<<<<<<<< [" + mergeBranchName + "]\n";
                beforeState = "add";
            }
            result +=  "\033[1;32m" + y[n - 1] + "\033[0m";
        }
        else if (m > 0 and (n == 0 || lookup[m][n - 1] < lookup[m - 1][n]))
        {
            GetMergeDiff(mergeBranchName, lookup, x, y, m - 1, n, result, beforeState, after);

            after = true;
            if (beforeState != "rm")
            {
                result += "<<<<<<<<<< [HEAD]\n";
                beforeState = "rm";
            }
            result += "\033[1;31m" + x[m - 1] + "\033[0m";
        }
    }
}
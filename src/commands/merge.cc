#include "commands/commands.hh"

#include "objects/tree.hh"
#include "objects/blob.hh"

#include "utils/zlib.hh"
#include "utils/wrappers.hh"
#include "utils/utils.hh"
#include "utils/get_paths.hh"

namespace mygit
{
    void merge (const options::MergeOptions& opt)
    {
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
        std::map<std::string, std::string> inCurrent, inCurrentStatus;
        utils::GetDiffBetweenTrees(entryCurrent, entryCommonAncestor, inCurrent, inCurrentStatus);

        std::map<std::string, std::string> inToMerge, inToMergeStatus;
        utils::GetDiffBetweenTrees(entryToMerge, entryCommonAncestor, inToMerge, inToMergeStatus);

        /// FIXME TODEL
        for (const auto& add1 : inCurrent)
            std::cout << inCurrentStatus[add1.first] << ' ' << add1.first << ' ' << add1.second << '\n';
        std::cout << '\n';

        std::vector<std::string> conflicts;
        std::map<std::string, std::string> addToIndex, rmFromIndex;
        for (const auto& add2 : inToMerge)
        {
            std::string add2File = add2.first;
            std::string add2Hash = add2.second;
            std::string add2Status = inToMergeStatus[add2File];

            std::cout << add2Status << ' ' << add2File << ' ' << add2Hash << '\n';

            auto it = inCurrent.find(add2File);
            if (it == inCurrent.end())
            {
                std::string pathFileFromDotMyGit = utils::CleanPath(utils::AppendPathToRootRepo(add2File));
                //std::cout << "pathDotGit: " << pathFileFromDotMyGit << '\n';
                if (add2Status == "added" or add2Status == "modified")
                {
                    std::cout << ((add2Status == "added") ? "Adding : " : "Modifying : ") << pathFileFromDotMyGit << '\n';

                    addToIndex.insert({add2File, add2Hash});

                    std::string pathFile = utils::PathToObjectFile(add2Hash);
                    std::string fileContents = objects::GetContentBlobDecompressed(utils::DecompressString(utils::ReadFile(pathFile)));
                    /// Add directories if needed
                    utils::CreateDirectoriesAboveFile(pathFileFromDotMyGit);
                    /// Write to file in case of Add/Modify
                    utils::WriteFile(pathFileFromDotMyGit, fileContents);
                }
                else if (add2Status == "deleted")
                {
                    std::cout << "Removing :" << pathFileFromDotMyGit << '\n';

                    rmFromIndex.insert({add2File, add2Hash});

                    remove(pathFileFromDotMyGit.c_str());

                    /// If dir containing file is empty, delete it
                    utils::DeleteDirectoryIfEmpty(pathFileFromDotMyGit);
                }
            }
            /// Conflict case
            else if (it->second != add2Hash or inCurrentStatus[it->first] != add2Status)
            {
                std::cout << "Conflict: " << add2File << '\n';
                conflicts.push_back(add2File);
                /// FIXME
            }

            /// Update INDEX
            std::map<std::string, std::string> entries = utils::ReadIndexAndGetEntries();
            for (const auto& toAdd : addToIndex)
            {
                entries[toAdd.first] = toAdd.second;
            }
            for (const auto& toDelete : rmFromIndex)
            {
                entries.erase(toDelete.first);
            }
            std::string res;
            for (const auto& p : entries)
            {
                res += p.second + ' ' + p.first + '\n';
            }
            std::string compressed = utils::CompressString(res);
            utils::WriteFile(utils::PathToIndex(), compressed);

            /// No conflicts -> commit changes (with 2 parent commits)
            if (conflicts.empty())
            {
                options::CommitOptions commitOpt("Merge branch \'" + opt.branchToMerge + "\' into branch \'" + utils::GetCurrentBranch() + "\'.");
                mygit::commit(commitOpt, hashCommitToMerge);
            }
            else
            {
                std::cout << "You need to resolve your conflicts first and then commit for the merge to complete.\n";
            }
        }
    }
}
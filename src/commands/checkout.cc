#include "commands/commands.hh"

#include "objects/blob.hh"
#include "objects/tree.hh"

#include "utils/wrappers.hh"
#include "utils/get_paths.hh"
#include "utils/zlib.hh"
#include "utils/utils.hh"

namespace mygit
{
    void checkout (const options::CheckoutOptions& opt)
    {
        /// Display branches case
        if (opt.displayBranches)
        {
            utils::DisplayBranches();
        }
        /// Branch checkout case
        else
        {
            /// Check ref/destination is a commit object
            utils::ExitIfTrue(not utils::IsCommitObject(opt.commitToCheckoutOn), "Not valid commit object.");

            /// Check that you already are not on this commit/branch
            utils::ExitIfTrue(utils::IsAlreadyOnCommit(opt.commitToCheckoutOn), "You already are on this commit / branch.");

            /// Checkout that working directory is clean
            utils::ExitIfTrue(not utils::IsWorkingDirectoryClean(), "Working directory is not clean, commit or stash your changes.");

            /// Compare ref/destination tree and HEAD tree
            std::string hashCommitDst = utils::GetCommitHash(opt.commitToCheckoutOn);
            std::string hashCommitHead = utils::GetMostRecentCommit();

            //std::cout << hashCommitDst << ' ' << hashCommitHead << '\n';

            std::string hashTreeDst = utils::GetTreeHashFromCommit(hashCommitDst);
            std::string hashTreeHead = utils::GetTreeHashFromCommit(hashCommitHead);

            //std::cout << hashTreeDst << ' ' << hashTreeHead << '\n';

            std::map<std::string, std::string> entryDst = objects::Tree::TreeHashToEntryMap(hashTreeDst);
            std::map<std::string, std::string> entryHead = objects::Tree::TreeHashToEntryMap(hashTreeHead);

            /*for (auto e : entryHead)
            {
                std::cout << "head: " << e.first << ' ' << e.second << '\n';
            }*/

            for (const auto& wantedEntry : entryDst)
            {
                std::string file = wantedEntry.first;
                std::string hash = wantedEntry.second;

                std::string pathFile = utils::PathToObjectFile(hash);
                std::string fileContents = objects::GetContentBlobDecompressed(utils::DecompressString(utils::ReadFile(pathFile)));
                //std::cout << "dst: " << file << ' ' << hash << '\n';

                auto it = entryHead.find(file);
                std::string pathFileFromDotMyGit = utils::CleanPath(utils::AppendPathToRootRepo(file));
                /// Case where we need to add a file
                if (it == entryHead.end())
                {
                    /// Add directories if needed
                    utils::CreateDirectoriesAboveFile(pathFileFromDotMyGit);
                }
                /// Write to file in case of Add/Modify
                utils::WriteFile(pathFileFromDotMyGit, fileContents);
            }

            for (const auto& headEntry : entryHead)
            {
                std::string file = headEntry.first;
                /// Delete file
                auto it = entryDst.find(file);
                if (it == entryDst.end())
                {
                    std::string pathFileFromDotMyGit = utils::CleanPath(utils::AppendPathToRootRepo(file));
                    remove(pathFileFromDotMyGit.c_str());

                    /// If dir containing file is empty, delete it
                    utils::DeleteDirectoryIfEmpty(pathFileFromDotMyGit);
                }
            }

            /// Update INDEX
            std::string indexUpdate;
            for (const auto& wantedEntry : entryDst)
            {
                //std::cout << "dst: " << wantedEntry.first << ' ' << wantedEntry.second << '\n';
                indexUpdate += wantedEntry.second + ' ' + wantedEntry.first + '\n';
            }
            std::string compressedIndex = utils::CompressString(indexUpdate);
            utils::WriteFile(utils::PathToIndex(), compressedIndex);

            /// Update HEAD
            /// Is branchname case
            if (utils::CheckBranchExists(opt.commitToCheckoutOn))
                utils::WriteFile(utils::PathToHEAD(), "ref: refs/heads/" + opt.commitToCheckoutOn);
            /// Commit hash case
            else
                utils::WriteFile(utils::PathToHEAD(), opt.commitToCheckoutOn);
        }
    }
}
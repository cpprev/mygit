#include "commands.hh"

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

                std::string pathFile = g_pathToRootRepo + "/.mygit/objects/" + hash.substr(0, 2) + "/" + hash.substr(2);
                std::string fileContents = objects::GetContentBlobDecompressed(utils::DecompressString(utils::ReadFile(pathFile)));
                //std::cout << "dst: " << file << ' ' << hash << '\n';

                auto it = entryHead.find(file);
                /// Case where we need to add a file
                if (it == entryHead.end())
                {
                    /// Add directories if needed
                    /// FIXME
                }
                /// Write to file in case of Add/Modify
                std::string pathFileFromDotMyGit = g_pathToRootRepo + "/" + file;
                utils::WriteFile(pathFileFromDotMyGit, fileContents);
            }

            for (const auto& headEntry : entryHead)
            {
                std::string file = headEntry.first;
                /// Delete file
                auto it = entryDst.find(file);
                if (it == entryDst.end())
                {
                    std::string pathFileFromDotMyGit = g_pathToRootRepo + "/" + file;
                    remove(pathFileFromDotMyGit.c_str());

                    /// If dir containing file is empty, delete it
                    /// FIXME
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
            utils::WriteFile(g_pathToRootRepo + "/.mygit/index", compressedIndex);

            /// Update HEAD
            /// Is branchname case
            if (utils::CheckBranchExists(opt.commitToCheckoutOn))
                utils::WriteFile(g_pathToRootRepo + "/.mygit/HEAD", "ref: refs/heads/" + opt.commitToCheckoutOn);
            /// Commit hash case
            else
                utils::WriteFile(g_pathToRootRepo + "/.mygit/HEAD", opt.commitToCheckoutOn);
        }
    }
}
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

            /// Compare ref/destination tree and HEAD tree
            std::string hashCommitDst = utils::GetCommitHash(opt.commitToCheckoutOn);
            std::string hashCommitHead = utils::GetMostRecentCommit();

            //std::cout << hashCommitDst << ' ' << hashCommitHead << '\n';

            std::string hashTreeDst = utils::GetTreeHashFromCommit(hashCommitDst);
            std::string hashTreeHead = utils::GetTreeHashFromCommit(hashCommitHead);

            //std::cout << hashTreeDst << ' ' << hashTreeHead << '\n';

            std::map<std::string, std::string> entryDst = objects::Tree::TreeHashToEntryMap(hashTreeDst);
            std::map<std::string, std::string> entryHead = objects::Tree::TreeHashToEntryMap(hashTreeHead);

            for (auto e : entryHead)
            {
                std::cout << e.first << ' ' << e.second << '\n';
            }

            /// Update INDEX

            /// Update HEAD
        }
    }
}
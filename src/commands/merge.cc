#include "commands/commands.hh"

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
    }
}
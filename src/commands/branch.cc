#include "commands/commands.hh"

#include "utils/wrappers.hh"
#include "utils/get_paths.hh"
#include "utils/utils.hh"

namespace mygit
{
    void branch (int argc, char *argv[])
    {
        /// Create opt object
        auto opt = options::BranchOptions(argc, argv);

        /// Display branches case
        if (opt.displayBranches)
        {
            utils::DisplayBranches();
        }
        /// Create branch case
        else if (not opt.branchToCreate.empty())
        {
            /// Check if branch does not exist already
            utils::ExitIfTrue(utils::CheckBranchExists(opt.branchToCreate), "Branch \'" + opt.branchToCreate + "\' already exists.");

            /// Check if repo has atleast 1 commit
            utils::ExitIfTrue(not utils::CheckRepoHasOneCommit(), "You need to have atleast one commit to create a branch.");

            /// Get current commit hash (HEAD)
            std::string latestCommit = utils::GetMostRecentCommit();

            /// Write it to .mygit/refs/heads/BRANCH_NAME
            std::string branchPath = utils::PathToBranch(opt.branchToCreate);
            utils::WriteFile(branchPath, latestCommit);
            
        }
        /// Delete branch case
        else if (not opt.branchToDelete.empty())
        {
            /// User cant delete the branch he is on
            utils::ExitIfTrue(utils::GetCurrentBranch() == opt.branchToDelete, "Can't delete the branch you are on.");
            /// Check if branch does exist
            utils::ExitIfTrue(not utils::CheckBranchExists(opt.branchToDelete), "Branch \'" + opt.branchToDelete + "\' does not exist.");

            /// Remove branch
            std::string branchPath = utils::PathToBranch(opt.branchToDelete);
            remove(branchPath.c_str());
        }
    }
}
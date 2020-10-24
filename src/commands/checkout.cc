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
            std::string hashCommitToCopy = utils::GetCommitHash(opt.commitToCheckoutOn);
            std::string hashCommitCurrent = utils::GetMostRecentCommit();

            /// Update Working Dir and Index
            utils::UpdateWorkingDirectoryAndIndex(hashCommitCurrent, hashCommitToCopy);

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
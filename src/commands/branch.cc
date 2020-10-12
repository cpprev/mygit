#include "commands.hh"

namespace mygit
{
    void branch (const options::BranchOptions& opt)
    {
        /// Display branches case
        if (opt.displayBranches)
        {

        }
        /// Create branch case
        else if (not opt.branchToCreate.empty())
        {

        }
        /// Delete branch case
        else if (not opt.branchToDelete.empty())
        {

        }
    }
}
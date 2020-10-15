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

        }
    }
}
#include "commands.hh"

namespace mygit
{
    void init ()
    {
        /// Error checking
        utils::ExitIfTrue(utils::IsDirExists(".mygit/"), "Repository has already been initialized.");
        utils::ExitIfTrue(not utils::CreateDir(".mygit/"), "'.mygit' can not be created.");

        /// File setup
        utils::CreateFile(".mygit/HEAD");
        utils::CreateDir(".mygit/objects/");
        utils::CreateDir(".mygit/refs/");
        utils::CreateDir(".mygit/refs/heads/");
        utils::CreateDir(".mygit/refs/remotes/");

        std::cout << "Initialized empty MyGit repository\n";
    }
}
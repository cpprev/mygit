#include "commands.hh"

namespace mygit
{
    void init ()
    {
        if (utils::IsDirExists(".mygit/"))
        {
            utils::ExitProgramWithMessage(1, "Repository has already been initialized.");
        }

        if (not utils::CreateDir(".mygit/"))
        {
            utils::ExitProgramWithMessage(1, "'.mygit' can not be created.");
        }

        utils::CreateFile(".mygit/HEAD");

        utils::CreateDir(".mygit/objects/");

        utils::CreateDir(".mygit/refs/");
        utils::CreateDir(".mygit/refs/heads/");
        utils::CreateDir(".mygit/refs/remotes/");

        std::cout << "Initialized empty MyGit repository\n";
    }
}
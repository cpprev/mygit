#include "commands.hh"

namespace mygit
{
    void ls_files ()
    {
        std::string pathToDotMyGit = utils::FindPathToDotMyGit();
        if (pathToDotMyGit.empty())
        {
            utils::ExitProgramWithMessage(1, "You are not in a MyGit repository.");
        }
        std::cout << "Files currently in the 'staging area' / 'index' :\n\n";
        std::string indexContents = utils::ReadFile(pathToDotMyGit + "/.mygit/index");
        if (not indexContents.empty())
            std::cout << utils::DecompressString(indexContents);
    }
}
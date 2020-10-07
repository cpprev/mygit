#include "commands.hh"

namespace mygit
{
    void ls_files ()
    {
        std::string pathToRootRepo = utils::FindPathToRootRepo();

        std::cout << "Files currently in the 'staging area' / 'index' :\n\n";
        std::string indexContents = utils::ReadFile(pathToRootRepo + "/.mygit/index");
        if (not indexContents.empty())
            std::cout << utils::DecompressString(indexContents);
    }
}
#include "commands.hh"
#include "utils/get_paths.hh"

namespace mygit
{
    void ls_files ()
    {
        std::cout << "Files currently in the 'staging area' / 'index' :\n\n";
        std::string indexContents = utils::ReadFile(utils::PathToIndex());
        if (not indexContents.empty())
            std::cout << utils::DecompressString(indexContents);
    }
}
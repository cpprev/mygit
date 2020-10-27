#include "commands/commands.hh"

#include "utils/get_paths.hh"
#include "utils/zlib.hh"
#include "utils/utils.hh"

namespace mygit
{
    void ls_files (int argc, char *argv[])
    {
        (void) argc;
        (void) argv;

        std::cout << "Files currently in the 'staging area' / 'index' :\n\n";
        std::string indexContents = utils::ReadFile(utils::PathToIndex());
        if (not indexContents.empty())
            std::cout << utils::DecompressString(indexContents);
    }
}
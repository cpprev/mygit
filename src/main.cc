#include <string>

#include "utils/utils.hh"
#include "commands/commands.hh"

int main(int argc, char *argv[])
{
    if (argc == 1)
    {
        utils::ExitProgramWithMessage(1, "You need to specify a MyGit command.");
    }

    std::string command = argv[1];
    if (command == "init")
    {
        mygit::init();
    }
    else if (command == "add")
    {
        if (argc <= 2)
        {
            utils::ExitProgramWithMessage(1, "You need to specify a parameter to the 'add' command.");
        }
        mygit::add(argv[2]);
    }
    else if (command == "hash-object")
    {
        if (argc <= 2)
        {
            utils::ExitProgramWithMessage(1, "You need to specify a parameter to the 'hash-object' command.");
        }
        mygit::hash_object(mygit::HashObjectOptions(argc, argv));
    }
    else if (command == "cat-file")
    {
        /// FIXME
    }
    else if (command == "ls-files")
    {
        mygit::ls_files();
    }
    else
    {
        utils::ExitProgramWithMessage(1, "Unknown command.");
    }

    return 0;
}
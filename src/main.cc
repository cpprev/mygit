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

    return 0;
}
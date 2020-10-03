#include <string>
#include <iostream>

#include "commands/init.hh"

int main(int argc, char *argv[])
{
    if (argc == 1)
    {
        throw std::runtime_error("FIXME Later");
    }

    std::string command = argv[1];
    if (command == "init")
    {
        mygit::init();
    }

    return 0;
}
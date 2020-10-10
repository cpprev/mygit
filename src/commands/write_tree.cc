#include "commands.hh"

namespace mygit
{
    void write_tree ()
    {
        std::map<std::string, std::string> map = utils::ReadIndexAndGetEntries();

        objects::Tree tree = objects::Tree(map, "", 1);
        tree.SetupTrees();

        std::cout << tree._hash << "\n";
        //tree.PrintTree(0);
    }
}
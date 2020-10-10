#include "commands.hh"

namespace mygit
{
    std::string write_tree_wrap ()
    {
        std::map<std::string, std::string> map = utils::ReadIndexAndGetEntries();

        objects::Tree tree = objects::Tree(map, "", 1);
        tree.SetupTrees();

        //tree.PrintTree(0);
        return tree._hash;
    }

    void write_tree()
    {
        std::cout << write_tree_wrap() << "\n";
    }
}
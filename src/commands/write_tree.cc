#include "commands/commands.hh"

#include "objects/tree.hh"

#include "utils/utils.hh"

namespace mygit
{
    void write_tree(int argc, char *argv[])
    {
        (void) argc;
        (void) argv;

        std::cout << write_tree_wrap() << "\n";
    }

    std::string write_tree_wrap ()
    {
        std::map<std::string, std::string> map = utils::ReadIndexAndGetEntries();

        objects::Tree tree = objects::Tree(map, "", 1);
        tree.SetupTrees();

        //tree.PrintTree(0);
        return tree._hash;
    }
}
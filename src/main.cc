#include <string>

#include "utils/utils.hh"
#include "utils/wrappers.hh"
#include "utils/get_paths.hh"
#include "commands/commands.hh"

int main(int argc, char *argv[])
{
    /// Check
    utils::ExitIfTrue(argc == 1, "You need to specify a MyGit command.");

    /// Set globals
    utils::SetGlobalVariables();

    std::string command = argv[1];
    /// Pre parse arguments
    utils::ExitIfTrue(utils::DoesRequireRepo(command) and utils::PathToRootRepo().empty(), "You are not in a MyGit repository.");
    utils::ExitIfTrue(utils::DoesRequireOneOrMoreArguments(command) and argc <= 2, "You need to specify more parameters to this command.");

    if (command == "init")
        mygit::init();
    else if (command == "add")
        mygit::add(options::AddOptions(argc, argv));
    else if (command == "status")
        mygit::status();
    else if (command == "diff")
        mygit::diff();
    else if (command == "branch")
        mygit::branch(options::BranchOptions(argc, argv));
    else if (command == "checkout")
        mygit::checkout(options::CheckoutOptions(argc, argv));
    else if (command == "log")
        mygit::log(options::LogOptions(argc, argv));
    else if (command == "hash-object")
        mygit::hash_object(options::HashObjectOptions(argc, argv));
    else if (command == "cat-file")
        mygit::cat_file(options::CatFileOptions(argc, argv));
    else if (command == "ls-files")
        mygit::ls_files();
    else if (command == "write-tree")
        mygit::write_tree();
    else if (command == "commit")
        mygit::commit(options::CommitOptions(argc, argv));
    else if (command == "config")
        mygit::config(options::ConfigOptions(argc, argv));
    else
        utils::ExitProgramWithMessage(1, "Unknown command.");

    return 0;
}
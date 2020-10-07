#include <string>

#include "utils/utils.hh"
#include "commands/commands.hh"

int main(int argc, char *argv[])
{
    if (argc == 1)
    {
        utils::ExitProgramWithMessage(1, "You need to specify a MyGit command.");
    }
    /// Set globals
    g_pathToRootRepo = utils::FindPathToRootRepo();
    std::string myGitIgnoreContents = utils::ReadFile(g_pathToRootRepo + "/.mygitignore");
    g_myGitIgnorePatterns = utils::ReadMyGitIgnorePatterns(myGitIgnoreContents);

    std::string command = argv[1];
    /// Pre parse arguments
    if (utils::DoesRequireRepo(command) and g_pathToRootRepo.empty())
    {
        utils::ExitProgramWithMessage(1, "You are not in a MyGit repository.");
    }
    if (utils::DoesRequireOneOrMoreArguments(command) and argc <= 2)
    {
        utils::ExitProgramWithMessage(1, "You need to specify more parameters to this command.");
    }

    if (command == "init")
    {
        mygit::init();
    }
    else if (command == "add")
    {
        mygit::add(mygit::AddOptions(argc, argv));
    }
    else if (command == "status")
    {
        mygit::status();
    }
    else if (command == "hash-object")
    {
        mygit::hash_object(mygit::HashObjectOptions(argc, argv));
    }
    else if (command == "cat-file")
    {
        mygit::cat_file(mygit::CatFileOptions(argc, argv));
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
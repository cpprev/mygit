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
    g_pathToRepoRoot = utils::FindPathToDotMyGit();
    std::string myGitIgnoreContents = utils::ReadFile(g_pathToRepoRoot + "/.mygitignore");
    g_myGitIgnorePatterns = utils::ReadMyGitIgnorePatterns(myGitIgnoreContents);

    std::string command = argv[1];


    if (utils::DoesRequireRepo(command) and g_pathToRepoRoot.empty())
    {
        utils::ExitProgramWithMessage(1, "You are not in a MyGit repository.");
    }

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
        mygit::add(mygit::AddOptions(argc, argv));
    }
    else if (command == "status")
    {
        mygit::status();
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
        if (argc <= 2)
        {
            utils::ExitProgramWithMessage(1, "You need to specify a parameter to the 'cat-file' command.");
        }
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
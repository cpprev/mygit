#include "commands/commands.hh"

#include "utils/wrappers.hh"
#include "utils/utils.hh"
#include "utils/get_paths.hh"

namespace mygit
{
    void init ()
    {
        /// Error checking
        utils::ExitIfTrue(utils::IsDirExists(g_DB_FILE + "/"), "Repository has already been initialized.");
        utils::ExitIfTrue(not utils::CreateDir(g_DB_FILE + "/"), "'" + g_DB_FILE + "' can not be created.");

        /// File setup
        utils::CreateFile(g_DB_FILE + "/HEAD");
        utils::CreateDir(g_DB_FILE + "/objects/");
        utils::CreateDir(g_DB_FILE + "/refs/");
        utils::CreateDir(g_DB_FILE + "/refs/heads/");
        utils::CreateDir(g_DB_FILE + "/refs/remotes/");

        /// Setup first branch
        utils::WriteFile(g_DB_FILE + "/HEAD", "ref: refs/heads/master");
        utils::CreateFile(g_DB_FILE + "/refs/heads/master");

        std::cout << "Initialized empty MyGit repository\n";
    }
}
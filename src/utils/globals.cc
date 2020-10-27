#include <string>

#include "utils/utils.hh"
#include "utils/config.hh"

#include "commands/commands.hh"

/// Init globals
std::vector<std::string> g_myGitIgnorePatterns = std::vector<std::string>();
std::string g_pathToRootRepo = "";
utils::Config g_Config = utils::Config();
std::string g_DB_FILE = ".mygit";
std::map<std::string, MyGitCommand> g_commands =
{
    { "init", &mygit::init },
    { "add", &mygit::add },
    { "status", &mygit::status },
    { "diff", &mygit::diff },
    { "branch", &mygit::branch },
    { "checkout", &mygit::checkout },
    { "log", &mygit::log },
    { "hash-object", &mygit::hash_object },
    { "cat-file", &mygit::cat_file },
    { "ls-files", &mygit::ls_files },
    { "write-tree", &mygit::write_tree },
    { "config", &mygit::config },
    { "commit", &mygit::commit },
    { "merge", &mygit::merge },
};
#include <string>

#include "utils/utils.hh"
#include "utils/wrappers.hh"
#include "utils/get_paths.hh"

int main(int argc, char *argv[])
{
    /// Check
    utils::ExitIfTrue(argc == 1, "You need to specify a MyGit command.");

    /// Set globals
    utils::SetGlobals();

    std::string command = argv[1];
    /// Pre parse arguments
    utils::ExitIfTrue(utils::DoesRequireRepo(command) and utils::PathToRootRepo().empty(), "You are not in a MyGit repository.");
    utils::ExitIfTrue(utils::DoesRequireOneOrMoreArguments(command) and argc <= 2, "You need to specify more parameters to this command.");

    /// Execute the command
    auto it = g_commands.find(command);
    utils::ExitIfTrue(it == g_commands.end(), "Unknown command.");
    it->second(argc, argv);

    return 0;
}
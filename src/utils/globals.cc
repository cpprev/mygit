#include <string>

#include "utils/utils.hh"
#include "utils/config.hh"

/// Init globals
std::vector<std::string> g_myGitIgnorePatterns = std::vector<std::string>();
std::string g_pathToRootRepo = "";
utils::Config g_Config = utils::Config();
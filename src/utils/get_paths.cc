#include "utils.hh"

namespace utils
{
    std::string PathToHEAD ()
    {
        return g_pathToRootRepo + "/.mygit/HEAD";
    }

    std::string PathToIndex ()
    {
        return g_pathToRootRepo + "/.mygit/index";
    }

    std::string PathToLocalConfig ()
    {
        return g_pathToRootRepo + "/.mygit/config";
    }

    std::string PathToGlobalConfig ()
    {
        return std::string(getenv("HOME")) + "/.mygitconfig";
    }

    std::string PathToGitIgnore ()
    {
        return g_pathToRootRepo + "/.mygitignore";
    }

    std::string PathToBranch (const std::string& branchName)
    {
        return g_pathToRootRepo + "/.mygit/refs/heads/" + branchName;
    }

    std::string PathToObjectDir (const std::string& hash)
    {
        return g_pathToRootRepo + "/.mygit/objects/" + hash.substr(0, 2);
    }

    std::string PathToObjectFile (const std::string& hash)
    {
        return g_pathToRootRepo + "/.mygit/objects/" + hash.substr(0, 2) + "/" + hash.substr(2);
    }
}
#include "utils.hh"

namespace utils
{
    std::string PathToRootRepo ()
    {
        return CleanPath(g_pathToRootRepo + "/");
    }

    std::string PathToHEAD ()
    {
        return CleanPath(PathToRootRepo() + "/" + g_DB_FILE + "/HEAD");
    }

    std::string PathToIndex ()
    {
        return CleanPath(PathToRootRepo() + "/" + g_DB_FILE + "/index");
    }

    std::string PathToLocalConfig ()
    {
        return CleanPath(PathToRootRepo() + "/" + g_DB_FILE + "/config");
    }

    std::string PathToGlobalConfig ()
    {
        return CleanPath(std::string(getenv("HOME")) + "/" + g_DB_FILE + "config");
    }

    std::string PathToGitIgnore ()
    {
        return CleanPath(PathToRootRepo() + "/" + g_DB_FILE + "ignore");
    }

    std::string PathToRefsHeads ()
    {
        return CleanPath(PathToRootRepo() + "/" + g_DB_FILE + "/refs/heads");
    }

    std::string PathToBranch (const std::string& branchName)
    {
        return CleanPath(PathToRootRepo() + "/" + g_DB_FILE + "/refs/heads/" + branchName);
    }

    std::string PathToObjectDir (const std::string& hash)
    {
        return CleanPath(PathToRootRepo() + "/" + g_DB_FILE + "/objects/" + hash.substr(0, 2));
    }

    std::string PathToObjectFile (const std::string& hash)
    {
        return CleanPath(PathToRootRepo() + "/" + g_DB_FILE + "/objects/" + hash.substr(0, 2) + "/" + hash.substr(2));
    }

    std::string AppendPathToRootRepo (const std::string& path)
    {
        return CleanPath(PathToRootRepo() + "/" + path);
    }

    std::string AppendPathToDotMyGit (const std::string& path)
    {
        return CleanPath(PathToRootRepo() + "/" + g_DB_FILE + "/" + path);
    }
}
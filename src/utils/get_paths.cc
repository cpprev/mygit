#include "utils.hh"

namespace utils
{
    std::string PathToRootRepo ()
    {
        return g_pathToRootRepo + "/";
    }

    std::string PathToHEAD ()
    {
        return PathToRootRepo() + "/.mygit/HEAD";
    }

    std::string PathToIndex ()
    {
        return PathToRootRepo() + "/.mygit/index";
    }

    std::string PathToLocalConfig ()
    {
        return PathToRootRepo() + "/.mygit/config";
    }

    std::string PathToGlobalConfig ()
    {
        return std::string(getenv("HOME")) + "/.mygitconfig";
    }

    std::string PathToGitIgnore ()
    {
        return PathToRootRepo() + "/.mygitignore";
    }

    std::string PathToRefsHeads ()
    {
        return PathToRootRepo() + "/.mygit/refs/heads";
    }

    std::string PathToBranch (const std::string& branchName)
    {
        return PathToRootRepo() + "/.mygit/refs/heads/" + branchName;
    }

    std::string PathToObjectDir (const std::string& hash)
    {
        return PathToRootRepo() + "/.mygit/objects/" + hash.substr(0, 2);
    }

    std::string PathToObjectFile (const std::string& hash)
    {
        return PathToRootRepo() + "/.mygit/objects/" + hash.substr(0, 2) + "/" + hash.substr(2);
    }

    std::string AppendPathToRootRepo (const std::string& path)
    {
        return PathToRootRepo() + "/" + path;
    }

    std::string AppendPathToDotMyGit (const std::string& path)
    {
        return PathToRootRepo() + "/.mygit/" + path;
    }
}
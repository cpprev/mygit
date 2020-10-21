#include "utils.hh"

namespace utils
{
    std::string PathToRootRepo ()
    {
        return CleanPath(g_pathToRootRepo + "/");
    }

    std::string PathToHEAD ()
    {
        return CleanPath(PathToRootRepo() + "/.mygit/HEAD");
    }

    std::string PathToIndex ()
    {
        return CleanPath(PathToRootRepo() + "/.mygit/index");
    }

    std::string PathToLocalConfig ()
    {
        return CleanPath(PathToRootRepo() + "/.mygit/config");
    }

    std::string PathToGlobalConfig ()
    {
        return CleanPath(std::string(getenv("HOME")) + "/.mygitconfig");
    }

    std::string PathToGitIgnore ()
    {
        return CleanPath(PathToRootRepo() + "/.mygitignore");
    }

    std::string PathToRefsHeads ()
    {
        return CleanPath(PathToRootRepo() + "/.mygit/refs/heads");
    }

    std::string PathToBranch (const std::string& branchName)
    {
        return CleanPath(PathToRootRepo() + "/.mygit/refs/heads/" + branchName);
    }

    std::string PathToObjectDir (const std::string& hash)
    {
        return CleanPath(PathToRootRepo() + "/.mygit/objects/" + hash.substr(0, 2));
    }

    std::string PathToObjectFile (const std::string& hash)
    {
        return CleanPath(PathToRootRepo() + "/.mygit/objects/" + hash.substr(0, 2) + "/" + hash.substr(2));
    }

    std::string AppendPathToRootRepo (const std::string& path)
    {
        return CleanPath(PathToRootRepo() + "/" + path);
    }

    std::string AppendPathToDotMyGit (const std::string& path)
    {
        return CleanPath(PathToRootRepo() + "/.mygit/" + path);
    }
}
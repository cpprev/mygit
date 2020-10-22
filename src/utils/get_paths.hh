#pragma once

#include <iostream>
#include <string>

namespace utils
{
    std::string PathToRootRepo ();

    std::string PathToHEAD ();

    std::string PathToIndex ();

    std::string PathToLocalConfig ();

    std::string PathToGlobalConfig ();

    std::string PathToGitIgnore ();

    std::string PathToRefsHeads ();

    std::string PathToBranch (const std::string& branchName);

    std::string PathToObjectDir (const std::string& hash);

    std::string PathToObjectFile (const std::string& hash);

    std::string AppendPathToRootRepo (const std::string& path);

    std::string AppendPathToDotMyGit (const std::string& path);
}
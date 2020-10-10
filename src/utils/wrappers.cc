#include "utils.hh"

namespace utils
{
    void ExitIfTrue (bool assertion, const std::string& message)
    {
        if (assertion)
            ExitProgramWithMessage(1, message);
    }

    void ExitProgramWithMessage(const int& status, const std::string& message)
    {
        std::cerr << message << std::endl;
        exit(status);
    }

    void SetGlobalVariables ()
    {
        g_pathToRootRepo = utils::FindPathToRootRepo();
        std::string myGitIgnoreContents = utils::ReadFile(g_pathToRootRepo + "/.mygitignore");
        g_myGitIgnorePatterns = utils::ReadMyGitIgnorePatterns(myGitIgnoreContents);
    }

    bool DoesRequireRepo (const std::string& command)
    {
        int len = sizeof(RequireRepoCommands) / sizeof(const char *);
        for (int i = 0; i < len; i++)
            if (command == RequireRepoCommands[i])
                return true;
        return false;
    }

    bool DoesRequireOneOrMoreArguments (const std::string& command)
    {
        int len = sizeof(RequireOneOrMoreArguments) / sizeof(const char *);
        for (int i = 0; i < len; i++)
            if (command == RequireOneOrMoreArguments[i])
                return true;
        return false;
    }

    bool ChangeDirWrapper(const std::string& path)
    {
        // Change dir
        chdir(path.c_str());
        // Update g_pathToRootRepo
        g_pathToRootRepo = utils::FindPathToRootRepo();
        return true;
    }
}
#include "utils.hh"
#include "config.hh"

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
        g_Config = utils::Config();
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

    void GoToCharAfterNextSlash (const std::string& path, size_t& i)
    {
        while (i < path.size() and path[i] == '/')
        {
            i += 1;
        }
    }

    void AddDiffCharacterBeforeLine (std::string& input, bool plus)
    {
        std::string newStr;
        bool mark = false;
        for (size_t i = 0; i < input.size(); i++)
        {
            if (i == 0 or mark)
                newStr += plus ? "\033[1;32m+\t" : "\033[1;31m-\t";
            mark = false;
            newStr += input[i];
            if (input[i] == '\n')
                mark = true;
        }
        newStr += "\033[0m";
        input = newStr;
    }

    std::string StripPaddingSpaces (const std::string& str)
    {
        int len = str.size();
        int left;
        for (left = 0; left < len and (str[left] == ' ' or str[left] == '\t'); left++)
        {}

        int right;
        for (right = len - 1; right >= 0 and (str[right] == ' ' or str[right] == '\t'); right--)
        {}

        return str.substr(left, right - left + 1);
    }
}
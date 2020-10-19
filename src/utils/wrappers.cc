#include "utils.hh"
#include "config.hh"
#include <ctime>

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
        int countNewLine = 1;
        for (size_t i = 0; i < input.size(); i++)
        {
            if (i == 0 or mark)
                newStr += "\033[1;33m[" + std::to_string(countNewLine) + "]" + (plus ? "\033[1;32m\t+\t" : "\033[1;31m\t-\t");
            mark = false;
            newStr += input[i];
            if (input[i] == '\n')
            {
                mark = true;
                countNewLine++;
            }
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

    std::string GetDateIntValue ()
    {
        time_t  timev;
        time(&timev);
        return std::to_string(timev);
    }

    std::string GetDateCommitFormat ()
    {
        time_t  timev;
        time(&timev);
        char buf[96];
        struct tm tm = *gmtime(&timev);
        strftime(buf, sizeof(buf) - 1, "%z", &tm);
        return std::to_string(timev) + " " + buf;
    }

    std::string GetDate ()
    {
        char buf[96];
        time_t now = time(0);
        struct tm tm = *gmtime(&now);
        strftime(buf, sizeof(buf) - 1, "%a, %d %b %Y %H:%M:%S %z (%Z)", &tm);
        std::string str = buf;
        return str;
    }

    void AddToNElementsList (const std::string& elm, std::list<std::string>& li, size_t maxSize)
    {
        if (li.size() < maxSize)
            li.emplace_back(elm);
        else
        {
            li.push_back(elm);
            li.pop_front();
        }
    }
}
#include "utils.hh"

namespace utils
{
    void ExitProgramWithMessage(const int& status, const std::string& message)
    {
        std::cerr << message << std::endl;
        exit(status);
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
}
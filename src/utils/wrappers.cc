#include "wrappers.hh"

namespace utils
{
    void ExitProgramWithMessage(const int& status, const std::string& message)
    {
        std::cerr << message << std::endl;
        exit(status);
    }
}
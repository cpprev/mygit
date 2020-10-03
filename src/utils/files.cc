#include "files.hh"

namespace utils
{
    bool IsDirExists(const std::string &path)
    {
        struct stat st;
        if (stat(path.c_str(), &st) != 0)
            return false;
        return (st.st_mode & S_IFDIR) != 0;
    }

    bool CreateDir(const std::string &name)
    {
        if (mkdir(name.c_str(), 0777) == -1)
        {
            return false;
        }
        return true;
    }

    bool CreateFile(const std::string &name)
    {
        std::ofstream file { name };
        return true;
    }
}
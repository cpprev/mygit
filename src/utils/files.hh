#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>

namespace utils
{
    bool IsDirExists(const std::string &path);

    bool CreateDir(const std::string &name);

    bool CreateFile(const std::string &name);
}
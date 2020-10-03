#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "utils/utils.hh"
#include "utils/zlib.hh"

namespace mygit
{
    /// MyGit commands
    void init();
    void add(const std::string& path);
    void ls_files ();

    /// Sub functions
    void UpdateIndex(const std::string& pathToFile, const std::string& pathToDotMyGit);
}
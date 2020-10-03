#pragma once

#include <string>
#include <stdexcept>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstring>

#include <zlib.h>

namespace utils
{
    std::string CompressString(const std::string &str,
                               int compressionlevel = Z_BEST_COMPRESSION);

    std::string DecompressString(const std::string &str);
}
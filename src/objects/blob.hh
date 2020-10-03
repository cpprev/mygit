#pragma once

#include <string>
#include <iostream>
#include <openssl/sha.h>

#include "utils/utils.hh"

namespace objects
{
    class Blob
    {
    public:
        std::string ToString () const;
        std::string ToHash();
        Blob(const std::string& pathFileFromDotMyGit, const std::string& fullPath);

    private:
        std::string _pathFileFromDotMyGit;
        std::string _fullPath;
        std::string _contentsFile;
    };
}
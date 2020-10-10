#pragma once

#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <cstring>

#include "objects/object_type.hh"

namespace options
{
    class HashObjectOptions
    {
    public:
        HashObjectOptions() = default;
        HashObjectOptions(int argc, char *argv[]);

        std::vector<std::string> params;
        objects::ObjectType type = objects::BLOB;
        bool write = false;
    };

    class CatFileOptions
    {
    public:
        CatFileOptions(int argc, char *argv[]);

        std::string first_param;
        bool size = false;
        bool type = false;
        bool content = false;
        bool raw = false;
    };

    class AddOptions
    {
    public:
        AddOptions(int argc, char *argv[]);

        std::vector<std::string> pathArguments;
        bool force = false;
    };

    class CommitOptions
    {
    public:
        CommitOptions(int argc, char *argv[]);

        std::string commitMessage;
    };
}
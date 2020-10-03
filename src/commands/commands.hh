#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <map>

#include "objects/object_type.hh"
#include "utils/utils.hh"
#include "utils/zlib.hh"

namespace mygit
{
    class HashObjectOptions
    {
    public:
        HashObjectOptions(int argc, char *argv[]);

        std::string first_param;
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
        bool where = false;
    };

    /// MyGit commands
    void init();
    void add(const std::string& path);
    void ls_files ();
    void hash_object (const HashObjectOptions& opt);
    void cat_file (const CatFileOptions& opt);

    /// Sub functions
    void UpdateIndex(const std::string& pathToFile, const std::string& pathToDotMyGit);
}
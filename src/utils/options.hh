#pragma once

#include <iostream>
#include <map>
#include <vector>
#include <string>

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

    class LogOptions
    {
    public:
        LogOptions(int argc, char *argv[]);

        // FIXME
    };

    class BranchOptions
    {
    public:
        BranchOptions(int argc, char *argv[]);

        bool displayBranches = false;
        std::string branchToCreate;
        std::string branchToDelete;
    };

    class CheckoutOptions
    {
    public:
        CheckoutOptions(int argc, char *argv[]);

        bool displayBranches = false;
        std::string commitToCheckoutOn;
    };

    class ConfigOptions
    {
    public:
        ConfigOptions(int argc, char *argv[]);

        bool local = true;
        bool global = false;
        std::optional<std::pair<std::string, std::string>> add;
        std::string unset;
        std::string get;
    };

    class MergeOptions
    {
    public:
        MergeOptions(int argc, char *argv[]);

        std::string branchToMerge;
    };
}
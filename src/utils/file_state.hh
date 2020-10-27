#pragma once

#include <string>

namespace utils
{
    enum FileState
    {
        ADDED,
        DELETED,
        MODIFIED,
    };

    FileState ObjectTypeFromString (const std::string& str);
    std::string ObjectTypeToString (const FileState& state);
}
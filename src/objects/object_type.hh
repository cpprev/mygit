#pragma once

#include <string>

namespace objects
{
    enum ObjectType
    {
        BLOB,
        TREE,
        COMMIT,
    };

    ObjectType ObjectTypeFromString (const std::string& str);
}
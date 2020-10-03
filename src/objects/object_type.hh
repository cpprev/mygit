#pragma once

#include "blob.hh"

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
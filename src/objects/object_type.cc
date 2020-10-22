#include "object_type.hh"
#include "utils/wrappers.hh"

namespace objects
{
    ObjectType ObjectTypeFromString (const std::string& str)
    {
        if (str == "blob")
            return objects::BLOB;
        else if (str == "commit")
            return objects::COMMIT;
        else if (str == "tree")
            return objects::TREE;
        else
            utils::ExitProgramWithMessage(1, "Unknown type (valid types : blob, commit, tree)");
        return BLOB;
    }
}
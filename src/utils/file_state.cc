#include "utils/file_state.hh"

namespace utils
{
    FileState ObjectTypeFromString (const std::string& str)
    {
        if (str == "added")
            return utils::ADDED;
        else if (str == "deleted")
            return utils::DELETED;
        else if (str == "modified")
            return utils::MODIFIED;
        return utils::ADDED;
    }

    std::string ObjectTypeToString (const FileState& state)
    {
        if (state == utils::ADDED)
            return "added";
        if (state == utils::DELETED)
            return "deleted";
        if (state == utils::MODIFIED)
            return "modified";
        return "";
    }
}
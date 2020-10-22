#include "commands.hh"

namespace mygit
{
    std::string status_str ()
    {
        std::map<std::string, std::string> added, deleted, modified;
        utils::GetWorkDirFileStatus(added, deleted, modified);

        std::string output = "\033[0;31mChanged not staged for commit : \033[0m\n\n";

        for (const auto& f : added)
        {
            output += "\033[0;31m\tAdded:  \t" + f.second + "\033[0m\n";
        }

        for (const auto& f : deleted)
        {
            output += "\033[0;31m\tDeleted:\t" + f.second + "\033[0m\n";
        }

        for (const auto& f : modified)
        {
            output += "\033[0;31m\tModified:\t" + f.second + "\033[0m\n";
        }

        return output;
    }

    void status()
    {
        std::cout << status_str();
    }
}
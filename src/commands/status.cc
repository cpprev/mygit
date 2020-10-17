#include "commands.hh"

namespace mygit
{
    std::string status_str ()
    {
        /// Read index files
        std::vector<std::string> indexEntries = utils::ReadIndexAndGetEntriesIndexAsList();
        /// Read working directory files
        std::vector<std::string> workDirEntries = utils::GetWorkingDirectoryFiles();
        std::vector<std::string> workDirEntriesFromActualPos = workDirEntries;
        for (size_t i = 0; i < workDirEntries.size(); i++)
        {
            workDirEntriesFromActualPos[i] = utils::GetPathRelativeToYourself(workDirEntries[i]);
            //std::cout << workDirEntries[i] << ' ' << workDirEntriesFromActualPos[i] << '\n';
            workDirEntries[i] = utils::GetPathRelativeToDotMyGit(workDirEntries[i]);
        }

        std::string output;
        output += "\033[0;31mChanged not staged for commit : \033[0m\n\n";
        for (size_t i = 0; i < workDirEntries.size(); i++)
        {
            std::string wdFile = workDirEntries[i];
            std::string wdFileFromActualPos = workDirEntriesFromActualPos[i];
            if (std::find(indexEntries.begin(), indexEntries.end(), wdFile) != indexEntries.end())
            {
                objects::Blob blob = objects::Blob(wdFile, wdFileFromActualPos);
                std::string hash = blob.ToHash();
                std::string blobPath = g_pathToRootRepo + "/.mygit/objects/" + hash.substr(0, 2) + "/" + hash.substr(2);
                if (not utils::IsFileExists(blobPath))
                {
                    output += "\033[0;31m\tModified:\t" + wdFileFromActualPos + "\033[0m\n";
                }
            }
            /// File not in index, there was added
            else
            {
                if (not utils::IsFileExcluded(wdFileFromActualPos))
                    output += "\033[0;31m\tAdded:  \t" + wdFileFromActualPos + "\033[0m\n";
            }
        }

        for (const auto& indEntry : indexEntries)
        {
            if (std::find(workDirEntries.begin(), workDirEntries.end(), indEntry) == workDirEntries.end())
            {
                if (not utils::IsFileExcluded(indEntry))
                    output += "\033[0;31m\tDeleted:\t" + utils::GetPathRelativeToYourself(utils::RemoveUselessCharInPath(g_pathToRootRepo + "/" + indEntry)) + "\033[0m\n";
            }
        }
        output += "\n";

        return output;
    }

    void status()
    {
        std::cout << status_str();
    }
}
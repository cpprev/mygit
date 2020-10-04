#include "commands.hh"

namespace mygit
{
    void status()
    {
        std::string pathToDotMyGit = utils::FindPathToDotMyGit();
        if (pathToDotMyGit.empty())
        {
            utils::ExitProgramWithMessage(1, "You are not in a MyGit repository.");
        }

        /// Read index files
        std::vector<std::string> indexEntries = utils::ReadIndexAndGetEntriesIndexAsList(pathToDotMyGit);
        std::vector<std::string> indexEntriesFromActualPos = indexEntries;
        for (size_t i = 0; i < indexEntriesFromActualPos.size(); i++)
        {
            indexEntriesFromActualPos[i] = utils::RemoveUselessCharInPath(pathToDotMyGit + "/" + indexEntriesFromActualPos[i]);
        }

        //for (auto e : indexEntriesFromActualPos)
        //    std::cout << "1111: " <<  e << "\n";

        /// Read working directory files
        std::vector<std::string> workDirEntries = utils::GetWorkingDirectoryFiles(pathToDotMyGit);

        //for (auto e : workDirEntries)
        //    std::cout << "2222: " << e << "\n";

        std::cout << "\033[0;31m" << "Changed not staged for commit :" << "\033[0m" << "\n\n";
        for (const auto& wdFile : workDirEntries)
        {
            if (std::find(indexEntriesFromActualPos.begin(), indexEntriesFromActualPos.end(), wdFile) != indexEntriesFromActualPos.end())
            {
                std::string pathFileFromDotMyGit = utils::GetPathRelativeToDotMyGit(wdFile, pathToDotMyGit);
                objects::Blob blob = objects::Blob(pathFileFromDotMyGit, wdFile);
                std::string hash = blob.ToHash();
                std::string blobPath = pathToDotMyGit + "/.mygit/objects/" + hash.substr(0, 2) + "/" + hash.substr(2);
                if (not utils::IsFileExists(blobPath))
                {
                    std::cout << "\033[0;31m" << "\tModifed:\t" << wdFile << "\033[0m" << "\n";
                }
            }
            /// File not in index, there was added
            else
            {
                std::cout << "\033[0;31m" << "\tAdded:  \t" << wdFile << "\033[0m" << "\n";
            }
        }

        for (const auto& indEntry : indexEntriesFromActualPos)
        {
            if (std::find(workDirEntries.begin(), workDirEntries.end(), indEntry) == workDirEntries.end())
            {
                std::cout << "\033[0;31m" << "\tDeleted:\t" << indEntry << "\033[0m" << "\n";
            }
        }
        std::cout << "\n";
    }
}
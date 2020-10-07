#include "commands.hh"

namespace mygit
{
    std::string status_str ()
    {
        std::string pathToDotMyGit = utils::FindPathToDotMyGit();
        if (pathToDotMyGit.empty())
        {
            utils::ExitProgramWithMessage(1, "You are not in a MyGit repository.");
        }

        /// Read index files
        std::vector<std::string> indexEntries = utils::ReadIndexAndGetEntriesIndexAsList(pathToDotMyGit);
        //std::vector<std::string> indexEntriesFromActualPos = indexEntries;
        /*for (size_t i = 0; i < indexEntriesFromActualPos.size(); i++)
        {
            indexEntriesFromActualPos[i] = utils::RemoveUselessCharInPath(pathToDotMyGit + "/" + indexEntriesFromActualPos[i]);
        }*/

        //for (auto e : indexEntries)
        //    std::cout << "indexEntries: " <<  e << "\n";

        //for (auto e : indexEntriesFromActualPos)
        //    std::cout << "indexEntriesFromActualPos: " <<  e << "\n";

        /// Read working directory files
        std::vector<std::string> workDirEntries = utils::GetWorkingDirectoryFiles(pathToDotMyGit);
        std::vector<std::string> workDirEntriesFromActualPos = workDirEntries;
        for (size_t i = 0; i < workDirEntries.size(); i++)
        {
            workDirEntriesFromActualPos[i] = utils::GetPathRelativeToYourself(workDirEntries[i]);

            workDirEntries[i] = utils::GetPathRelativeToDotMyGit(workDirEntries[i], pathToDotMyGit);
        }
        /*for (auto e : workDirEntries)
        {
            //std::cout << "entry: " << e << "\n";
            //std::string r = utils::GetPathRelativeToYourself(utils::RemoveUselessCharInPath(e));
            std::cout << "workDirEntries: " << e << "\n";
        }
        for (auto e : workDirEntriesFromActualPos)
        {
            std::cout << "workDirEntriesFromPos: " << e << "\n";
        }*/

        std::string output;
        output += "\033[0;31mChanged not staged for commit : \033[0m\n\n";
        for (size_t i = 0; i < workDirEntries.size(); i++)
        {
            std::string wdFile = workDirEntries[i];
            std::string wdFileFromActualPos = workDirEntriesFromActualPos[i];
            if (std::find(indexEntries.begin(), indexEntries.end(), wdFile) != indexEntries.end())
            {
                //std::string pathFileFromDotMyGit = utils::GetPathRelativeToDotMyGit(wdFile, pathToDotMyGit);
                objects::Blob blob = objects::Blob(wdFile, wdFileFromActualPos);
                std::string hash = blob.ToHash();
                std::string blobPath = pathToDotMyGit + "/.mygit/objects/" + hash.substr(0, 2) + "/" + hash.substr(2);
                if (not utils::IsFileExists(blobPath))
                {
                    output += "\033[0;31m\tModified:\t" + wdFileFromActualPos + "\033[0m\n";
                }
            }
            /// File not in index, there was added
            else
            {
                output += "\033[0;31m\tAdded:  \t" + wdFileFromActualPos + "\033[0m\n";
            }
        }

        for (const auto& indEntry : indexEntries)
        {
            if (std::find(workDirEntries.begin(), workDirEntries.end(), indEntry) == workDirEntries.end())
            {
                if (not utils::IsFileExcluded(indEntry))
                    output += "\033[0;31m\tDeleted:\t" + utils::GetPathRelativeToYourself(utils::RemoveUselessCharInPath(pathToDotMyGit + "/" + indEntry)) + "\033[0m\n";
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
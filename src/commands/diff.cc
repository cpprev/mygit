#include "commands.hh"

namespace mygit
{
    std::string diff_str ()
    {
        /// Read index files
        std::vector<std::string> indexEntries = utils::ReadIndexAndGetEntriesIndexAsList();
        std::map<std::string, std::string> indexEntriesAsMap = utils::ReadIndexAndGetEntries();
        /// Read working directory files
        std::vector<std::string> workDirEntries = utils::GetWorkingDirectoryFiles();
        std::vector<std::string> workDirEntriesFromActualPos = workDirEntries;
        for (size_t i = 0; i < workDirEntries.size(); i++)
        {
            workDirEntriesFromActualPos[i] = utils::GetPathRelativeToYourself(workDirEntries[i]);
            workDirEntries[i] = utils::GetPathRelativeToDotMyGit(workDirEntries[i]);
        }

        for (auto e : indexEntries)
            std::cout << "indexEntry: " << e << "\n";
        for (auto e : workDirEntries)
            std::cout << "workDirEntry: " << e << "\n";

        std::string output;
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
                    /// FIXME
                    //output += "\033[0;31m\tModified:\t" + wdFileFromActualPos + "\033[0m\n";
                }
            }
            /// File not in index, there was added
            else
            {
                std::string contents = utils::ReadFile(wdFileFromActualPos);
                utils::AddDiffCharacterBeforeLine(contents, true);
                output += "\033[1;32m[" + wdFileFromActualPos + "]\n" + contents + "\n";
            }
        }

        for (const auto& indexEntry : indexEntries)
        {
            if (std::find(workDirEntries.begin(), workDirEntries.end(), indexEntry) == workDirEntries.end())
            {
                if (not utils::IsFileExcluded(indexEntry))
                {
                    std::string hashBlob = indexEntriesAsMap[indexEntry];
                    std::string pathToBlob = g_pathToRootRepo + "/.mygit/objects/" + hashBlob.substr(0, 2) + "/" + hashBlob.substr(2);
                    std::string contents = objects::GetContentBlobDecompressed(utils::DecompressString(utils::ReadFile(pathToBlob)));
                    utils::AddDiffCharacterBeforeLine(contents, false);
                    output += "\033[1;31m[" + utils::GetPathRelativeToYourself(utils::RemoveUselessCharInPath(g_pathToRootRepo + "/" + indexEntry)) + "]\n" + contents + "\n";
                }
            }
        }

        return output;
    }

    void diff ()
    {
        std::cout << diff_str() << "\n";
    }
}
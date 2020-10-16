#include "commands.hh"

namespace mygit
{

    void diff ()
    {
        std::string diffStr = diff_str();
        std::cout << diffStr << "\n";
    }

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
                    std::string hashExBlob = indexEntriesAsMap[wdFile];
                    std::string exBlobPath = g_pathToRootRepo + "/.mygit/objects/" + hashExBlob.substr(0, 2) + "/" + hashExBlob.substr(2);
                    std::string contentsBlob = objects::GetContentBlobDecompressed(utils::DecompressString(utils::ReadFile(exBlobPath)));
                    std::string contentsWd = utils::ReadFile(wdFileFromActualPos);

                    std::vector<std::string> oldLines = utils::GetLinesAsVect(contentsBlob);
                    std::vector<std::string> newLines = utils::GetLinesAsVect(contentsWd);
                    std::vector<std::vector<int>> LCSMatrix = BuildLcsMatrix(oldLines, newLines);
                    std::string result;
                    GetDiffModifiedFiles(LCSMatrix, oldLines, newLines, oldLines.size(), newLines.size(), result);
                    output += "\033[1;32m[" + wdFileFromActualPos + "]\033[0m\n\n" + result + "\n";
                }
            }
            /// File not in index, there was added
            else
            {
                std::string contents = utils::ReadFile(wdFileFromActualPos);
                utils::AddDiffCharacterBeforeLine(contents, true);
                output += "\033[1;32m[" + wdFileFromActualPos + "]\033[0m\n\n" + contents + "\n";
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
                    output += "\033[1;31m[" + utils::GetPathRelativeToYourself(utils::RemoveUselessCharInPath(g_pathToRootRepo + "/" + indexEntry)) + "]\033[0m\n\n" + contents + "\n";
                }
            }
        }

        return output;
    }

    std::vector<std::vector<int>> BuildLcsMatrix (const std::vector<std::string>& oldFileLines, const std::vector<std::string>& newFileLines)
    {
        std::vector<int> sub(newFileLines.size() + 1, 0);
        std::vector<std::vector<int>> lookup(oldFileLines.size() + 1, sub);

        size_t lenA = oldFileLines.size();
        size_t lenB = newFileLines.size();
        for (size_t i = 0; i <= lenA; i++)
        {
            for (size_t j = 0; j <= lenB; j++)
            {
                if (i == 0 || j == 0)
                    lookup[i][j] = 0;
                else if (oldFileLines[i - 1] == newFileLines[j - 1])
                    lookup[i][j] = lookup[i - 1][j - 1] + 1;
                else
                    lookup[i][j] = std::max(lookup[i - 1][j], lookup[i][j - 1]);
            }
        }

        return lookup;
    }

    void GetDiffModifiedFiles (const std::vector<std::vector<int>>& lookup, const std::vector<std::string>& x,
                               const std::vector<std::string>& y, int m, int n, std::string& result)
    {
        if (m > 0 and n > 0 and x[m - 1] == y[n - 1])
        {
            GetDiffModifiedFiles(lookup, x, y, m - 1, n - 1, result);
            result += " \t" + x[m - 1] + "\033[0m";
        }
        else if (n > 0 and (m == 0 || lookup[m][n - 1] >= lookup[m - 1][n]))
        {
            GetDiffModifiedFiles(lookup, x, y, m, n - 1, result);
            result += "\033[1;32m+\t" + y[n - 1] + "\033[0m";
        }
        else if (m > 0 and (n == 0 || lookup[m][n - 1] < lookup[m - 1][n]))
        {
            GetDiffModifiedFiles(lookup, x, y, m - 1, n, result);
            result += "\033[1;31m-\t" + x[m - 1] + "\033[0m";
        }
    }
}
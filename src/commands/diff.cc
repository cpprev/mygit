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
                    GetDiffModifiedFiles(LCSMatrix, oldLines, newLines, oldLines.size() - 1, newLines.size() - 1, result);
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
        std::vector<int> sub(newFileLines.size(), 0);
        std::vector<std::vector<int>> res(oldFileLines.size() + 1, sub);

        size_t lenA = oldFileLines.size();
        size_t lenB = newFileLines.size();
        for (size_t i = 0; i <= lenA; i++)
        {
            for (size_t j = 0; j <= lenB; j++)
            {
                if (i == 0 || j == 0)
                    res[i][j] = 0;
                else if (oldFileLines[i - 1] == newFileLines[j - 1])
                    res[i][j] = res[i - 1][j - 1] + 1;
                else
                    res[i][j] = std::max(res[i - 1][j], res[i][j - 1]);
            }
        }
        return res;
    }

    void GetDiffModifiedFiles (const std::vector<std::vector<int>>& c, const std::vector<std::string>& x,
                               const std::vector<std::string>& y, int i, int j, std::string& result)
    {
        if (i < 0 and j < 0)
            return;
        else if (i < 0)
        {
            GetDiffModifiedFiles(c, x, y, i, j - 1, result);
            result += "\033[1;32m+\t" + y[j] + "\033[0m";
        }
        else if (j < 0)
        {
            GetDiffModifiedFiles(c, x, y, i - 1, j, result);
            result += "\033[1;31m-\t" + x[i] + "\033[0m";
        }
        else if (x[i] == y[j])
        {
            GetDiffModifiedFiles(c, x, y, i - 1, j - 1, result);
            result += " \t" + x[i] + "\033[0m";
        }
        else if (c[i][j - 1] >= c[i - 1][j])
        {
            GetDiffModifiedFiles(c, x, y, i, j - 1, result);
            result += "\033[1;32m+\t" + y[j] + "\033[0m";
        }
        else if (c[i][j - 1] < c[i - 1][j])
        {
            GetDiffModifiedFiles(c, x, y, i - 1, j, result);
            result += "\033[1;31m-\t" + x[i] + "\033[0m";
        }
    }
}
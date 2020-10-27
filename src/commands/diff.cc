#include "commands/commands.hh"

#include "objects/blob.hh"

#include "utils/wrappers.hh"
#include "utils/get_paths.hh"
#include "utils/zlib.hh"
#include "utils/utils.hh"

#define LINES_BEFORE_DIFF 4

namespace mygit
{

    void diff (int argc, char *argv[])
    {
        (void) argc;
        (void) argv;

        std::string diffStr = diff_str();
        utils::LessCommandWrapper(diffStr);
    }

    std::string diff_str ()
    {
        std::map<std::string, std::string> indexEntriesAsMap = utils::ReadIndexAndGetEntries();

        std::map<std::string, std::string> added, deleted, modified;
        utils::GetWorkDirFileStatus(added, deleted, modified);

        std::string output;

        for (const auto& f : added)
        {
            std::string wdFile = f.first;
            std::string wdFileFromActualPos = f.second;

            std::string contents = utils::ReadFile(wdFileFromActualPos);
            utils::AddDiffCharacterBeforeLine(contents, true);
            output += "\033[1;32m[" + wdFileFromActualPos + "]\033[0m\n\n" + contents + "\n\n";
        }

        for (const auto& f : deleted)
        {
            std::string indexEntry = f.first;
            std::string indexEntryFromActualPos = f.second;

            std::string hashBlob = indexEntriesAsMap[indexEntry];
            std::string pathToBlob = utils::PathToObjectFile(hashBlob);
            std::string contents = objects::GetContentBlobDecompressed(utils::DecompressString(utils::ReadFile(pathToBlob)));
            utils::AddDiffCharacterBeforeLine(contents, false);
            output += "\033[1;31m[" + indexEntryFromActualPos + "]\033[0m\n\n" + contents + "\n\n";
        }

        for (const auto& f : modified)
        {
            std::string wdFile = f.first;
            std::string wdFileFromActualPos = f.second;

            std::string hashExBlob = indexEntriesAsMap[wdFile];
            std::string exBlobPath = utils::PathToObjectFile(hashExBlob);
            std::string contentsBlob = objects::GetContentBlobDecompressed(utils::DecompressString(utils::ReadFile(exBlobPath)));
            std::string contentsWd = utils::ReadFile(wdFileFromActualPos);

            std::vector<std::string> oldLines = utils::GetLinesAsVect(contentsBlob);
            std::vector<std::string> newLines = utils::GetLinesAsVect(contentsWd);
            std::vector<std::vector<int>> LCSMatrix = BuildLcsMatrix(oldLines, newLines);
            std::string result;
            int count = 0;
            std::list<std::string> previousLines;
            GetDiffModifiedFiles(LCSMatrix, oldLines, newLines, oldLines.size(), newLines.size(), result, count, previousLines);
            output += "\033[1;32m[" + wdFileFromActualPos + "]\033[0m\n\n" + result + "\n\n";
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

    void ShrinkPreviousLines (std::string& result, std::list<std::string>& previousLines)
    {
        while (not previousLines.empty())
        {
            result += "\033[0m\t\t" + previousLines.front() + "\033[0m";
            previousLines.pop_front();
        }
    }

    void GetDiffModifiedFiles (const std::vector<std::vector<int>>& lookup, const std::vector<std::string>& x,
                               const std::vector<std::string>& y, int m, int n, std::string& result, int& count, std::list<std::string>& previousLines)
    {
        if (m > 0 and n > 0 and x[m - 1] == y[n - 1])
        {
            GetDiffModifiedFiles(lookup, x, y, m - 1, n - 1, result, count, previousLines);
            if (count == 0)
                utils::AddToNElementsList(x[m - 1], previousLines, LINES_BEFORE_DIFF);
            if (count > 0)
            {
                result += "\033[0m\t\t" + x[m - 1] + "\033[0m";
                count--;
            }
        }
        else if (n > 0 and (m == 0 || lookup[m][n - 1] >= lookup[m - 1][n]))
        {
            GetDiffModifiedFiles(lookup, x, y, m, n - 1, result, count, previousLines);

            ShrinkPreviousLines(result, previousLines);

            count = LINES_BEFORE_DIFF;
            result += "\033[1;33m[" + std::to_string(n) + "]\t\033[1;32m+\t" + y[n - 1] + "\033[0m";
        }
        else if (m > 0 and (n == 0 || lookup[m][n - 1] < lookup[m - 1][n]))
        {
            GetDiffModifiedFiles(lookup, x, y, m - 1, n, result, count, previousLines);

            ShrinkPreviousLines(result, previousLines);

            count = LINES_BEFORE_DIFF;
            result += "\033[1;33m[" + std::to_string(n) + "]\t\033[1;31m-\t" + x[m - 1] + "\033[0m";
        }
    }
}
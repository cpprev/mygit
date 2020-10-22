#include "commands.hh"
#include "utils/wrappers.hh"
#include "utils/get_paths.hh"

namespace mygit
{
    void add(const options::AddOptions& opt)
    {
        for (const auto& path : opt.pathArguments)
        {
            /// Handle git add of deleted files
            std::string pathRelativeToDotMyGit = utils::CleanPath(utils::GetPathRelativeToDotMyGit(path));
            //std::cout << "path: " << pathRelativeToDotMyGit << '\n';
            std::vector<std::string> indexEntries = utils::ReadIndexAndGetEntriesIndexAsList();
            std::vector<std::string> removeFromIndex;
            for (const auto& indEntry : indexEntries)
            {
                //std::cout << indEntry << " " << pathRelativeToDotMyGit << '\n';
                if (not utils::IsFileExists(utils::AppendPathToRootRepo(indEntry))
                and (indEntry == pathRelativeToDotMyGit or indEntry.find(pathRelativeToDotMyGit) != std::string::npos))
                {
                    //std::cout << "DELETE FROM INDEX\n";
                    removeFromIndex.push_back(indEntry);
                }
            }

            /// Case where 'path' leads to a directory
            if (utils::IsDirExists(path) or not removeFromIndex.empty())
            {
                /// Retreive all files from this directory
                std::vector<std::string> entries = utils::GetCurrentDirectoryFiles(path);
                UpdateIndexMultipleFiles(entries, opt, removeFromIndex);
            }
            /// Case where 'path' lads to a regular file
            else if (utils::IsFileExists(path) or not removeFromIndex.empty())
            {
                UpdateIndex(path, opt, removeFromIndex);
            }
            /// Wrongly formatted argument case
            else
            {
                utils::ExitProgramWithMessage(1, "You need to specify a valid parameter to the 'add' command.");
            }
        }
    }

    void UpdateIndex(const std::string& pathToFile, const options::AddOptions& opt, const std::vector<std::string>& removeFromIndex)
    {
        std::vector<std::string> paths = { pathToFile };
        UpdateIndexMultipleFiles(paths, opt, removeFromIndex);
    }

    void UpdateIndexMultipleFiles(const std::vector<std::string>& pathsToFiles, const options::AddOptions& opt, const std::vector<std::string>& removeFromIndex)
    {
        std::map<std::string, std::string> hashesAndPaths;
        for (const auto& pathToFile : pathsToFiles)
        {
            /// Get path to file relative to .mygit file
            std::string pathFileFromDotMyGit = utils::GetPathRelativeToDotMyGit(pathToFile);
            if (not opt.force and utils::IsFileExcluded(pathFileFromDotMyGit))
            {
                std::cout << "File \'" << pathFileFromDotMyGit << "\' is supposed to be ignored, therefore was not added.\n";
                continue;
            }
            /// Create blob (hash + compressed file) in .mygit/objects
            std::string hash = objects::CreateBlob(pathFileFromDotMyGit);

            hashesAndPaths.insert({pathFileFromDotMyGit, hash});
        }

        /// Read contents of 'index' file
        std::string indexPath = utils::PathToIndex();
        std::string contents = utils::ReadFile(indexPath);

        /// Decompress .mygit/index file
        std::string decompressed;
        if (not contents.empty())
            decompressed = utils::DecompressString(contents);

        /// Get entries from 'index' file
        std::map<std::string, std::string> entries = utils::GetEntriesFromIndex(decompressed);

        for (const auto& pair : hashesAndPaths)
        {
            entries[pair.first] = pair.second;
        }

        /// Delete elements from removeFromIndex vector
        for (const auto& toDelete : removeFromIndex)
        {
            entries.erase(toDelete);
        }

        /// Update the contents
        decompressed.clear();
        for (const auto& p : entries)
        {
            decompressed += p.second + ' ' + p.first + '\n';
        }

        /// Compress and update .mygit/index file
        std::string compressed = utils::CompressString(decompressed);
        utils::WriteFile(indexPath, compressed);
    }
}
#include "commands.hh"

namespace mygit
{
    AddOptions::AddOptions(int argc, char *argv[])
    {
        for (int i = 2; i < argc; i++)
        {
            if (strcmp(argv[i], "-f") == 0)
            {
                force = true;
            }
            else
            {
                pathArguments.push_back(argv[i]);
            }
        }
    }

    void add(const AddOptions& opt)
    {
        std::string pathToDotMyGit = utils::FindPathToDotMyGit();
        if (pathToDotMyGit.empty())
        {
            utils::ExitProgramWithMessage(1, "You are not in a MyGit repository.");
        }

        /// FIXME For now we handle only one path argument
        std::string path = opt.pathArguments[0];

        /// Case where 'path' leads to a directory
        if (utils::IsDirExists(path))
        {
            /// Retreive all files from this directory
            std::vector<std::string> entries = utils::GetCurrentDirectoryFiles(path);
            UpdateIndexMultipleFiles(entries, pathToDotMyGit, opt);
        }
        /// Case where 'path' lads to a regular file
        else if (utils::IsFileExists(path))
        {
            UpdateIndex(path, pathToDotMyGit, opt);
        }
        /// Wrongly formatted argument case
        else
        {
            utils::ExitProgramWithMessage(1, "You need to specify a valid parameter to the 'add' command.");
        }
    }

    void UpdateIndex(const std::string& pathToFile, const std::string& pathToDotMyGit, const AddOptions& opt)
    {
        /// Get path to file relative to .mygit file
        std::string pathFileFromDotMyGit = utils::GetPathRelativeToDotMyGit(pathToFile, pathToDotMyGit);
        if (not opt.force and utils::IsFileExcluded(pathFileFromDotMyGit))
        {
            std::cout << "File \'" << pathFileFromDotMyGit << "\' is supposed to be ignored, therefore was not added.\n";
            return;
        }

        /// Create blob (hash + compressed file) in .mygit/objects
        std::string hash = objects::CreateBlob(pathToDotMyGit, pathFileFromDotMyGit);

        /// Read contents of 'index' file
        std::string indexPath = pathToDotMyGit + ".mygit/index";
        std::string contents = utils::ReadFile(indexPath);

        /// Decompress .mygit/index file
        std::string decompressed;
        if (not contents.empty())
            decompressed = utils::DecompressString(contents);

        /// Get entries from 'index' file
        std::map<std::string, std::string> entries = utils::GetEntriesFromIndex(decompressed);

        entries[pathFileFromDotMyGit] = hash;

        /// Update the contents
        decompressed.clear();
        for (const auto& p : entries)
        {
            decompressed += p.second + ' ' + p.first + "\n";
        }

        /// Compress and update .mygit/index file
        std::string compressed = utils::CompressString(decompressed);
        utils::WriteFile(indexPath, compressed);
    }

    void UpdateIndexMultipleFiles(const std::vector<std::string>& pathsToFiles, const std::string& pathToDotMyGit, const AddOptions& opt)
    {
        std::map<std::string, std::string> hashesAndPaths;
        for (const auto& pathToFile : pathsToFiles)
        {
            /// Get path to file relative to .mygit file
            std::string pathFileFromDotMyGit = utils::GetPathRelativeToDotMyGit(pathToFile, pathToDotMyGit);
            if (not opt.force and utils::IsFileExcluded(pathFileFromDotMyGit))
            {
                std::cout << "File \'" << pathFileFromDotMyGit << "\' is supposed to be ignored, therefore was not added.\n";
                continue;
            }
            /// Create blob (hash + compressed file) in .mygit/objects
            std::string hash = objects::CreateBlob(pathToDotMyGit, pathFileFromDotMyGit);

            hashesAndPaths.insert({pathFileFromDotMyGit, hash});
        }

        /// Read contents of 'index' file
        std::string indexPath = pathToDotMyGit + ".mygit/index";
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
        //entries[pathFileFromDotMyGit] = hash;

        /// Update the contents
        decompressed.clear();
        for (const auto& p : entries)
        {
            decompressed += p.second + ' ' + p.first + "\n";
        }

        /// Compress and update .mygit/index file
        std::string compressed = utils::CompressString(decompressed);
        utils::WriteFile(indexPath, compressed);
    }
}
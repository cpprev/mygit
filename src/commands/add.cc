#include "commands.hh"

namespace mygit
{
    void add(const std::string& path)
    {
        std::string pathToDotMyGit = utils::FindPathToDotMyGit();
        if (pathToDotMyGit.empty())
        {
            utils::ExitProgramWithMessage(1, "You are not in a MyGit repository.");
        }

        /// Case where 'path' leads to a directory
        if (utils::IsDirExists(path))
        {
            std::cout << "DIR" << std::endl;
            /// Retreive all files from this directory
        }
        /// Case where 'path' lads to a regular file
        else if (utils::IsFileExists(path))
        {
            std::cout << "FILE" << std::endl;
            UpdateIndex(path, pathToDotMyGit);
        }
        /// Wrongly formatted argument case
        else
        {
            utils::ExitProgramWithMessage(1, "You need to specify a valid parameter to the 'add' command.");
        }
    }

    void UpdateIndex(const std::string& pathToFile, const std::string& pathToDotMyGit)
    {
        /// TODEL
        std::cout << pathToFile << ' ' << pathToDotMyGit << "\n";

        /// Get path to file relative to .mygit file
        std::string pathFileFromDotMyGit = utils::GetPathRelativeToDotMyGit(pathToFile, pathToDotMyGit);
        //std::cout << "test: " << pathFileFromDotMyGit << "\n";

        /// Create blob (hash + compressed file) in .mygit/objects
        std::string hash = utils::CreateBlob(pathToDotMyGit, pathFileFromDotMyGit);

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

        //std::cout << decompressed << "\n";

        /// Compress and update .mygit/index file
        std::string compressed = utils::CompressString(decompressed);
        utils::WriteFile(indexPath, compressed);
    }
}
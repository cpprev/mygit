#include "utils.hh"

namespace utils
{
    bool IsDirExists(const std::string &path)
    {
        struct stat st;
        if (stat(path.c_str(), &st) != 0)
            return false;
        return (st.st_mode & S_IFDIR) != 0;
    }

    bool IsFileExists(const std::string &path)
    {
        struct stat st;
        if (stat(path.c_str(), &st) != 0)
            return false;
        return (st.st_mode & S_IFREG) != 0;
    }

    bool CreateDir(const std::string &name)
    {
        if (mkdir(name.c_str(), 0777) == -1)
        {
            return false;
        }
        return true;
    }

    bool CreateFile(const std::string &name)
    {
        std::ofstream file { name };
        return true;
    }

    std::string FindPathToDotMyGit()
    {
        std::string currentPath = "./";
        while (true)
        {
            if (opendir(currentPath.c_str()) == nullptr)
                break;
            if (IsDirExists(currentPath + ".mygit/"))
            {
                return currentPath;
            }
            else
            {
                currentPath += "../";
            }
        }
        return "";
    }

    std::string ReadFile (const std::string& path)
    {
        std::ifstream ifs(path);
        std::string contents((std::istreambuf_iterator<char>(ifs)),
                              (std::istreambuf_iterator<char>()));
        return contents;
    }

    void WriteFile (const std::string& path, const std::string& str)
    {
        std::ofstream myfile;
        myfile.open (path);
        myfile << str;
        myfile.close();
    }

    std::string RemoveUselessCharInPath (const std::string& path)
    {
        std::string newPath;
        size_t i = 0;
        while(i < path.size())
        {
            if (i + 2 < path.size() and path[i] == '.' and path[i + 1] == '.' and path[i + 2] == '/')
            {
                newPath += "../";
                i += 3;
            }
            else if (i + 1 < path.size() and path[i] == '.' and path[i + 1] == '/')
            {
                i += 2;
            }
            else if (i + 1 < path.size() and path[i] == '/' and path[i + 1] == '/')
            {
                if (not newPath.empty())
                    newPath += "/";
                while (i < path.size() and path[i] == '/')
                {
                    i += 1;
                }
            }
            else
            {
                newPath += path[i];
                i += 1;
            }

        }
        return newPath;
    }

    std::string GetPathRelativeToDotMyGit(const std::string& pathToFile, const std::string& pathToDotMyGit)
    {
        std::string newPathToFile = RemoveUselessCharInPath(pathToFile);
        std::string newToDotMyGit = RemoveUselessCharInPath(pathToDotMyGit);

        std::string res;
        size_t i = 0;
        while (i < newPathToFile.size() and i < newToDotMyGit.size() and newPathToFile[i] == newToDotMyGit[i])
        {
            i += 1;
        }

        while (i < newPathToFile.size())
        {
            res += newPathToFile[i++];
        }

        return res;
    }

    std::string CreateBlob (const std::string& pathToDotMyGit, const std::string& pathFileFromDotMyGit)
    {
        /// Create the blob and generate the hash from it
        objects::Blob blob = objects::Blob(pathFileFromDotMyGit, pathToDotMyGit + '/' + pathFileFromDotMyGit);
        //std::cout << blob.ToString() << "\n";
        std::string hash = blob.ToHash();

        /// Store the blob
        std::string blobDirName = pathToDotMyGit + "/.mygit/objects/" + hash.substr(0, 2);
        std::cout << blobDirName << "\n";
        if (not IsDirExists(blobDirName))
            CreateDir(blobDirName);

        CreateFile(blobDirName + "/" + hash.substr(2));

        return hash;
    }
}
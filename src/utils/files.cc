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
                while (i < path.size() and path[i] == '/')
                {
                    i += 1;
                }
            }
            else if (i + 1 < path.size() and path[i] == '.' and path[i + 1] == '/')
            {
                i += 2;
                while (i < path.size() and path[i] == '/')
                {
                    i += 1;
                }
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

    std::map<std::string, std::string> GetEntriesFromIndex (const std::string& input)
    {
        std::map<std::string, std::string> res;
        std::string filePath, hash;
        bool hit_middle = false;
        for (size_t i = 0; i < input.size(); i++)
        {
            if (input[i] == '\n')
            {
                res.insert({filePath, hash});

                hit_middle = false;
                filePath.clear();
                hash.clear();
            }
            else if (input[i] == ' ')
            {
                hit_middle = true;
            }
            else
            {
                if (not hit_middle)
                    hash += input[i];
                else
                    filePath += input[i];
            }
        }
        return res;
    }

    std::vector<std::string> GetEntriesFromIndexAsList (const std::string& input)
    {
        std::map<std::string, std::string> map = GetEntriesFromIndex(input);
        std::vector<std::string> res;
        res.reserve(map.size());
        for (const auto& elm : map)
        {
            res.push_back(elm.first);
        }
        return res;
    }

    std::vector<std::string> ReadIndexAndGetEntriesIndexAsList (const std::string& pathToDotMyGit)
    {
        std::string indexContents = utils::ReadFile(pathToDotMyGit + "/.mygit/index");
        std::string decompressed;
        if (not indexContents.empty())
            decompressed = utils::DecompressString(indexContents);
        return GetEntriesFromIndexAsList(decompressed);
    }

    void IterateDir (const std::string& path, std::vector<std::string>& files)
    {
        DIR *dir = opendir(path.c_str());
        if (dir == nullptr)
            return;
        else
        {
            struct dirent *ent;
            while ((ent = readdir(dir)))
            {
                // Do not go through ".." or "." directories
                std::string tmp = ent->d_name;
                if (tmp == "." or tmp == "..")
                    continue;
                // Update path
                std::string new_path;
                if (path[path.size() - 1] == '/')
                    new_path = path + ent->d_name;
                else
                    new_path = path + "/" + ent->d_name;

                // Update files (only put regular files)
                if (IsFileExists(new_path))
                    files.push_back(RemoveUselessCharInPath(new_path));

                // Recurse (since its a directory)
                DIR *tstdir = opendir(new_path.c_str());
                if (tstdir != nullptr)
                {
                    IterateDir(new_path, files);
                    closedir(tstdir);
                }
            }
        }
        closedir(dir);
    }

    std::vector<std::string> GetWorkingDirectoryFiles (const std::string& pathToDotMyGit)
    {
        std::vector<std::string> files;
        IterateDir(pathToDotMyGit, files);
        return files;
    }
}
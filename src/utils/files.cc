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

    std::string GetCwd ()
    {
        char buf[256];
        std::string cwd = getcwd(buf, sizeof(buf));
        return buf;
    }

    std::string GetPathRelativeToDotMyGit(const std::string& pathToFileCpy, const std::string& pathToDotMyGitCpy)
    {
        (void) pathToDotMyGitCpy;
        /// Get current dir
        std::string origin = GetCwd();

        std::string pathToFile = pathToFileCpy;
        std::string filename = CutFileInPath(pathToFile);

        if (pathToFile.empty())
            pathToFile = ".";
        chdir(pathToFile.c_str());

        std::string pathToDotMyGit = utils::FindPathToDotMyGit();

        std::string cwd = GetCwd();

        /// cd to root dir
        chdir(pathToDotMyGit.c_str());
        /// Fullpath of root directory (containing .mygit/)
        std::string rootWD = GetCwd();
        /// cd back to origin dir
        chdir(origin.c_str());
        if (cwd == rootWD)
            return RemoveUselessCharInPath(filename);

        size_t ind_diff = cwd.find(rootWD) + rootWD.size() + 1;
        std::string diff = cwd.substr(ind_diff);

        return RemoveUselessCharInPath(diff + '/' + filename);
    }

    std::string CutFileInPath(std::string& pathRelativeToYouLong)
    {
        int i = pathRelativeToYouLong.size() - 1;
        if (IsDirExists(pathRelativeToYouLong))
            return "";
        std::string res;
        while (i >= 0 and pathRelativeToYouLong[i] != '/')
        {
            res = pathRelativeToYouLong[i] + res;
            i--;
        }
        pathRelativeToYouLong = pathRelativeToYouLong.substr(0, i);
        return res;
    }

    int CalcHeightDir (const std::string& dir1, const std::string& dir2)
    {
        int countSlash = 0;
        if (dir1.size() > dir2.size())
        {
            size_t i = dir1.size() - 1;
            while (i >= dir2.size())
            {
                if (dir1[i] == '/')
                    countSlash += 1;
                i--;
            }
        }
        else
            utils::ExitProgramWithMessage(1, "Not implemented");
        return countSlash;
    }

    std::string GetPathRelativeToYourself(const std::string& pathRelativeToYouLongCpy)
    {
        /// Get current dir
        std::string cwd = GetCwd();
        /// cd to target dir
        std::string pathRelativeToYouLong = pathRelativeToYouLongCpy;
        std::string fileName = CutFileInPath(pathRelativeToYouLong);
        //std::cout << "path: " << pathRelativeToYouLong << "\n";
        //std::cout << "filename: " << fileName << "\n";
        chdir(pathRelativeToYouLong.c_str());
        /// Fullpath of root directory (containing .mygit/)
        std::string cwd2 = GetCwd();

        //std::cout << cwd << "\n";
        //std::cout << cwd2 << "\n";
        /// cd back to origin dir
        chdir(cwd.c_str());
        if (cwd == cwd2)
            return fileName;
        else if (cwd2.size() > cwd.size())
        {
            size_t ind_diff = cwd2.find(cwd) + cwd.size() + 1;
            std::string res = cwd2.substr(ind_diff);
            //std::cout << "RES: " << utils::RemoveUselessCharInPath(res + "/" + fileName) << "\n";
            return utils::RemoveUselessCharInPath(res + "/" + fileName);
        }
        else
        {
            int diffHeight = CalcHeightDir(cwd, cwd2);
            std::string res;
            while (diffHeight > 0)
            {
                res += "../";
                diffHeight--;
            }
            return utils::RemoveUselessCharInPath(res + fileName);
        }
        return "";
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

                if (IsFileExcluded(new_path))
                    continue;

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

    bool IsFileExcluded (const std::string& path)
    {
        /// Exclude .mygit/ and .mygitignore files
        if (not fnmatch("*.mygit*", path.c_str(), 0))
            return true;

        std::string pathToDotMyGit = utils::FindPathToDotMyGit();

        for (const auto& pattern : g_myGitIgnorePatterns)
        {
            //std::cout << "pattern: " << pattern << "\n";
            if (not fnmatch(pattern.c_str(), path.c_str(), 0))
                return true;
        }

        return false;
    }

    std::vector<std::string> ReadMyGitIgnorePatterns(const std::string& contents)
    {
        std::vector<std::string> res;
        std::string cur;
        for (size_t i = 0; i < contents.size(); i++)
        {
            if (contents[i] == '\n' or i == contents.size() - 1)
            {
                if (i == contents.size() - 1 and contents[i] != '\n')
                    cur += contents[i];
                res.push_back(cur);
                cur.clear();
            }
            else
            {
                cur += contents[i];
            }
        }
        return res;
    }

    std::vector<std::string> GetWorkingDirectoryFiles (const std::string& pathToDotMyGit)
    {
        std::vector<std::string> files;
        IterateDir(pathToDotMyGit, files);
        return files;
    }

    std::vector<std::string> GetCurrentDirectoryFiles (const std::string& pathToDir)
    {
        std::vector<std::string> files;
        IterateDir(pathToDir, files);
        return files;
    }
}
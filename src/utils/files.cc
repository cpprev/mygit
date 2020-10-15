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

    std::string FindPathToRootRepo()
    {
        std::string currentPath = "./";
        while (true)
        {
            if (opendir(currentPath.c_str()) == nullptr)
                break;
            if (IsDirExists(currentPath + ".mygit/"))
                return currentPath;
            else
                currentPath += "../";
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
                utils::GoToCharAfterNextSlash(path, i);
            }
            else if (i + 1 < path.size() and path[i] == '.' and path[i + 1] == '/')
            {
                i += 2;
                utils::GoToCharAfterNextSlash(path, i);
            }
            else if (i + 1 < path.size() and path[i] == '/' and path[i + 1] == '/')
            {
                if (not newPath.empty())
                    newPath += "/";
                utils::GoToCharAfterNextSlash(path, i);
            }
            else
                newPath += path[i++];
        }
        return newPath;
    }

    std::string GetCwd ()
    {
        char buf[256];
        std::string cwd = getcwd(buf, sizeof(buf) / sizeof(char));
        return buf;
    }

    std::string GetPathRelativeToDotMyGit(const std::string& pathToFileCpy)
    {
        std::string origin = GetCwd();

        std::string pathToFile = pathToFileCpy;
        std::string filename = CutFileInPath(pathToFile);

        if (pathToFile.empty())
            pathToFile = ".";

        utils::ChangeDirWrapper(pathToFile);

        std::string cwd = GetCwd();

        utils::ChangeDirWrapper(g_pathToRootRepo);

        std::string rootWD = GetCwd();

        utils::ChangeDirWrapper(origin);
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
        if (i < 0)
            pathRelativeToYouLong = ".";
        else
            pathRelativeToYouLong = pathRelativeToYouLong.substr(0, i);
        return res;
    }

    int CalcHeightDir (const std::string& dir1, const std::string& dir2)
    {
        int countSlash = 0;
        if (dir1.size() >= dir2.size())
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
        std::string cwd = GetCwd();

        std::string pathRelativeToYouLong = pathRelativeToYouLongCpy;
        std::string fileName = CutFileInPath(pathRelativeToYouLong);
        if (pathRelativeToYouLong.empty())
            pathRelativeToYouLong = ".";

        utils::ChangeDirWrapper(pathRelativeToYouLong);

        std::string cwd2 = GetCwd();

        utils::ChangeDirWrapper(cwd);
        if (cwd == cwd2)
            return fileName;
        else if (cwd2.size() > cwd.size())
        {
            size_t ind_diff = cwd2.find(cwd) + cwd.size() + 1;
            std::string res = cwd2.substr(ind_diff);
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

    std::map<std::string, std::string> ReadIndexAndGetEntries ()
    {
        std::string indexContents = utils::ReadFile(g_pathToRootRepo + "/.mygit/index");
        std::string decompressed;
        if (not indexContents.empty())
            decompressed = utils::DecompressString(indexContents);
        return GetEntriesFromIndex(decompressed);
    }

    std::vector<std::string> ReadIndexAndGetEntriesIndexAsList ()
    {
        std::string indexContents = utils::ReadFile(g_pathToRootRepo + "/.mygit/index");
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
                std::string tmp = ent->d_name;
                if (tmp == "." or tmp == "..")
                    continue;

                std::string new_path;
                if (path[path.size() - 1] == '/')
                    new_path = path + ent->d_name;
                else
                    new_path = path + "/" + ent->d_name;

                if (IsFileExcluded(new_path))
                    continue;

                if (IsFileExists(new_path))
                    files.push_back(RemoveUselessCharInPath(new_path));

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
        if (not fnmatch("*.mygit*", path.c_str(), 0))
            return true;

        for (const auto& pattern : g_myGitIgnorePatterns)
        {
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

    std::vector<std::string> GetWorkingDirectoryFiles ()
    {
        std::vector<std::string> files;
        IterateDir(g_pathToRootRepo, files);
        return files;
    }

    std::vector<std::string> GetCurrentDirectoryFiles (const std::string& pathToDir)
    {
        std::vector<std::string> files;
        IterateDir(pathToDir, files);
        return files;
    }

    std::string ReadHEAD()
    {
        std::string contents = ReadFile(g_pathToRootRepo + "/.mygit/HEAD");
        return contents;
    }

    std::string ReadBranchPathInHead (const std::string& headContents)
    {
        std::string match = "ref: ";
        std::string::size_type ind = headContents.find(match);
        if (ind != std::string::npos)
        {
            return headContents.substr(ind + match.size());
        }
        return "";
    }

    std::string ReadBranchPathInHead ()
    {
        std::string headContents = ReadHEAD();
        return ReadBranchPathInHead(headContents);
    }

    std::string GetMostRecentCommit (const std::string& headContents)
    {
        std::string redirect = ReadBranchPathInHead();
        if (not redirect.empty())
        {
            /// Case where latest commit is in ref dir
            std::string readRedirect = ReadFile(g_pathToRootRepo + "/.mygit/" + redirect);
            //std::cout << redirect<< ' '<< readRedirect << "\n";
            return readRedirect;
        }
        else
        {
            return headContents;
        }
    }

    std::string GetMostRecentCommit ()
    {
        std::string headContents = ReadHEAD();
        return GetMostRecentCommit(headContents);
    }

    std::vector<std::string> GetLinesAsVect (const std::string& input)
    {
        std::vector<std::string> res;
        std::string dummy;
        size_t len = input.size();
        for (size_t i = 0; i < len; i++)
        {
            if (i == len - 1 or input[i] == '\n')
            {
                dummy += input[i];
                res.push_back(dummy);
                dummy.clear();
            }
            else
            {
                dummy += input[i];
            }
        }
        return res;
    }

    std::vector<std::string> ListBranches ()
    {
        std::string cwd = GetCwd();
        std::string pathToRefs = "./" + g_pathToRootRepo + "/.mygit/refs/heads/.";

        std::vector<std::string> res;
        DIR *dir = opendir(pathToRefs.c_str());
        struct dirent *ent;
        while ((ent = readdir(dir)))
        {
            std::string entName = ent->d_name;
            if (entName == "." or entName == "..")
                continue;
            res.push_back(entName);
        }
        closedir(dir);

        return res;
    }

    std::string GetCurrentBranch ()
    {
        std::string redirect = ReadBranchPathInHead();
        std::string match = "refs/heads/";
        std::string::size_type ind = redirect.find(match);
        if (ind == std::string::npos)
            return "";
        return redirect.substr(ind + match.size());
    }

    void DisplayBranches ()
    {
        std::string mainBranch = GetCurrentBranch();
        std::vector<std::string> branches = utils::ListBranches();
        for (const auto& branch : branches)
        {
            if (branch == mainBranch)
                std::cout << "* \033[1;32m" << branch << "\033[0m\n";
        }
        for (const auto& branch : branches)
        {
            if (branch != mainBranch)
                std::cout << branch << '\n';
        }
    }

    bool CheckRepoHasOneCommit ()
    {
        std::string headContents = utils::GetMostRecentCommit();
        return not headContents.empty();
    }

    bool CheckBranchExists (const std::string& branchName)
    {
        std::vector<std::string> branches = utils::ListBranches();
        return std::find(branches.begin(), branches.end(), branchName) != branches.end();
    }
}
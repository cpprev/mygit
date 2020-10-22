#include <ctime>
#include <algorithm>
#include <dirent.h>
#include <sys/wait.h>

#include "utils/utils.hh"
#include "utils/wrappers.hh"
#include "utils/config.hh"
#include "utils/get_paths.hh"
#include "utils/zlib.hh"

namespace utils
{
    void ExitIfTrue (bool assertion, const std::string& message)
    {
        if (assertion)
            ExitProgramWithMessage(1, message);
    }

    void ExitProgramWithMessage(const int& status, const std::string& message)
    {
        std::cerr << message << std::endl;
        exit(status);
    }

    void SetGlobalVariables ()
    {
        g_pathToRootRepo = utils::FindPathToRootRepo();
        std::string myGitIgnoreContents = utils::ReadFile(utils::PathToGitIgnore());
        g_myGitIgnorePatterns = utils::ReadMyGitIgnorePatterns(myGitIgnoreContents);
        g_Config = utils::Config();
    }

    bool DoesRequireRepo (const std::string& command)
    {
        int len = sizeof(RequireRepoCommands) / sizeof(const char *);
        for (int i = 0; i < len; i++)
            if (command == RequireRepoCommands[i])
                return true;
        return false;
    }

    bool DoesRequireOneOrMoreArguments (const std::string& command)
    {
        int len = sizeof(RequireOneOrMoreArguments) / sizeof(const char *);
        for (int i = 0; i < len; i++)
            if (command == RequireOneOrMoreArguments[i])
                return true;
        return false;
    }

    bool ChangeDirWrapper(const std::string& path)
    {
        // Change dir
        chdir(path.c_str());
        // Update g_pathToRootRepo
        g_pathToRootRepo = utils::FindPathToRootRepo();
        return true;
    }

    void GoToCharAfterNextSlash (const std::string& path, size_t& i)
    {
        while (i < path.size() and path[i] == '/')
        {
            i += 1;
        }
    }

    void AddDiffCharacterBeforeLine (std::string& input, bool plus)
    {
        std::string newStr;
        bool mark = false;
        int countNewLine = 1;
        for (size_t i = 0; i < input.size(); i++)
        {
            if (i == 0 or mark)
                newStr += "\033[1;33m[" + std::to_string(countNewLine) + "]" + (plus ? "\033[1;32m\t+\t" : "\033[1;31m\t-\t");
            mark = false;
            newStr += input[i];
            if (input[i] == '\n')
            {
                mark = true;
                countNewLine++;
            }
        }
        newStr += "\033[0m";
        input = newStr;
    }

    std::string StripPaddingSpaces (const std::string& str)
    {
        int len = str.size();
        int left;
        for (left = 0; left < len and (str[left] == ' ' or str[left] == '\t'); left++)
        {}

        int right;
        for (right = len - 1; right >= 0 and (str[right] == ' ' or str[right] == '\t'); right--)
        {}

        return str.substr(left, right - left + 1);
    }

    std::string GetDateIntValue ()
    {
        time_t timev;
        time(&timev);
        return std::to_string(timev);
    }

    std::string GetDateCommitFormat ()
    {
        time_t timev;
        time(&timev);
        char buf[96];
        struct tm tm = *gmtime(&timev);
        strftime(buf, sizeof(buf) - 1, "%z", &tm);
        return std::to_string(timev) + " " + buf;
    }

    std::string GetDate ()
    {
        char buf[96];
        time_t now = time(0);
        struct tm tm = *gmtime(&now);
        strftime(buf, sizeof(buf) - 1, "%a, %d %b %Y %H:%M:%S %z (%Z)", &tm);
        std::string str = buf;
        return str;
    }

    void AddToNElementsList (const std::string& elm, std::list<std::string>& li, size_t maxSize)
    {
        if (li.size() < maxSize)
            li.emplace_back(elm);
        else
        {
            li.push_back(elm);
            li.pop_front();
        }
    }

    void LessCommandWrapper (const std::string& contents)
    {
        std::string filepath = "A1278ZE8JS27";
        utils::WriteFile(filepath, contents);
        std::string command = "less -XRFf " + filepath;
        int pid = system(command.c_str());
        int status;
        waitpid(pid, &status, 0);
        remove(filepath.c_str());
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
        std::string pathToRefs = utils::PathToRefsHeads();

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
        bool hit = false;
        for (const auto& branch : branches)
        {
            if (branch == mainBranch)
            {
                hit = true;
                std::cout << "\033[1;32m* " << branch << "\033[0m\n";
            }
        }
        /// Detached HEAD case
        if (not hit)
        {
            std::cout << "\033[1;32m* " << ReadHEAD() << " (Detached HEAD)\033[0m\n";
        }
        for (const auto& branch : branches)
        {
            if (branch != mainBranch)
                std::cout << "  " << branch << '\n';
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

    bool IsAlreadyOnCommit (const std::string& commit)
    {
        std::string headContents = ReadHEAD();
        if (headContents == commit)
            return true;
        if (GetCurrentBranch() == commit)
            return true;
        return false;
    }

    bool IsCommitObject (const std::string& commitHash)
    {
        /// commitName could be a commit or a branch
        std::vector<std::string> branches = utils::ListBranches();
        /// Branch case
        if (std::find(branches.begin(), branches.end(), commitHash) != branches.end())
            return true;
        /// Commit case (detached head)
        std::string commitPath = utils::PathToObjectFile(commitHash);
        if (IsFileExists(commitPath))
            return true;
        return false;
    }

    std::string GetCommitHash (const std::string& commitOrBranch)
    {
        /// Branch case
        if (CheckBranchExists(commitOrBranch))
        {
            std::string commitPath = utils::PathToBranch(commitOrBranch);
            return ReadFile(commitPath);
        }
        /// Commit case
        return commitOrBranch;
    }

    std::string GetTreeHashFromCommit (const std::string& commitHash)
    {
        std::string commitPath = utils::PathToObjectFile(commitHash);
        std::string rawContents = utils::ReadFile(commitPath);

        utils::ExitIfTrue(rawContents.empty(), "Commit contents = empty (GetTreeHashFromCommit method).");

        std::string commitContents = utils::DecompressString(rawContents);

        std::string match = "tree ";
        std::string::size_type ind = commitContents.find(match);

        utils::ExitIfTrue(ind == std::string::npos, "Commit wrongly formatted (GetTreeHashFromCommit method).");

        std::string treeHash;
        for (size_t i = ind + match.size(); i < commitContents.size() and commitContents[i] != '\n'; i++)
        {
            treeHash += commitContents[i];
        }

        return treeHash;
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
            std::string readRedirect = ReadFile(utils::AppendPathToDotMyGit(redirect));
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

    std::string GetCwd ()
    {
        char buf[256];
        std::string cwd = getcwd(buf, sizeof(buf) / sizeof(char));
        return buf;
    }
}
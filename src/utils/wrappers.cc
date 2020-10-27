#include <ctime>
#include <algorithm>
#include <dirent.h>
#include <sys/wait.h>

#include "objects/blob.hh"
#include "objects/commit.hh"
#include "objects/tree.hh"

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

    void SetGlobals ()
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
        if (commitHash.size() < 2)
            return false;
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

    bool IsCommitAncestorOf (const std::string& c1, const std::string& c2)
    {
        std::string currentCommitHash = c2;
        while (not currentCommitHash.empty())
        {
            if (currentCommitHash == c1)
            {
                return true;
            }

            std::string commitPath = utils::PathToObjectFile(currentCommitHash);
            std::string commitRawContent = utils::DecompressString(utils::ReadFile(commitPath));
            std::string contentContent = objects::GetContentBlobDecompressed(commitRawContent);

            /// Update commit
            currentCommitHash = objects::ExtractParentCommit(contentContent);
        }

        return false;
    }

    std::string GetCommonAncestor (const std::string& c1, const std::string& c2)
    {
        std::string slowIterationCommitHash = c1;
        while (not slowIterationCommitHash.empty())
        {
            std::string fastIterationCommitHash = c2;
            while (not fastIterationCommitHash.empty())
            {
                /// Common ancestor found
                if (fastIterationCommitHash == slowIterationCommitHash)
                {
                    return fastIterationCommitHash;
                }

                /// Update fastCommit
                std::string fastCommitPath = utils::PathToObjectFile(fastIterationCommitHash);
                std::string fastCommitRawContent = utils::DecompressString(utils::ReadFile(fastCommitPath));
                std::string fastContentContent = objects::GetContentBlobDecompressed(fastCommitRawContent);
                fastIterationCommitHash = objects::ExtractParentCommit(fastContentContent);
            }

            /// Update slowCommit
            std::string slowCommitPath = utils::PathToObjectFile(slowIterationCommitHash);
            std::string slowCommitRawContent = utils::DecompressString(utils::ReadFile(slowCommitPath));
            std::string slowContentContent = objects::GetContentBlobDecompressed(slowCommitRawContent);
            slowIterationCommitHash = objects::ExtractParentCommit(slowContentContent);
        }

        return "";
    }

    void UpdateWorkingDirectoryAndIndex (const std::string& hashCommitCurrent, const std::string& hashCommitToCopy)
    {
        std::string hashTreeToCopy = utils::GetTreeHashFromCommit(hashCommitToCopy);
        std::string hashTreeCurrent = utils::GetTreeHashFromCommit(hashCommitCurrent);

        std::map<std::string, std::string> entryToCopy = objects::Tree::TreeHashToEntryMap(hashTreeToCopy);
        std::map<std::string, std::string> entryCurrent = objects::Tree::TreeHashToEntryMap(hashTreeCurrent);

        for (const auto& wantedEntry : entryToCopy)
        {
            std::string file = wantedEntry.first;
            std::string hash = wantedEntry.second;

            std::string pathFile = utils::PathToObjectFile(hash);
            std::string fileContents = objects::GetContentBlobDecompressed(utils::DecompressString(utils::ReadFile(pathFile)));

            auto it = entryCurrent.find(file);
            std::string pathFileFromDotMyGit = utils::CleanPath(utils::AppendPathToRootRepo(file));
            /// Case where we need to add a file
            if (it == entryCurrent.end())
            {
                /// Add directories if needed
                utils::CreateDirectoriesAboveFile(pathFileFromDotMyGit);
            }
            /// Write to file in case of Add/Modify
            utils::WriteFile(pathFileFromDotMyGit, fileContents);
        }

        for (const auto& curEntry : entryCurrent)
        {
            std::string file = curEntry.first;
            /// Delete file
            auto it = entryToCopy.find(file);
            if (it == entryToCopy.end())
            {
                std::string pathFileFromDotMyGit = utils::CleanPath(utils::AppendPathToRootRepo(file));
                remove(pathFileFromDotMyGit.c_str());

                /// If dir containing file is empty, delete it
                utils::DeleteDirectoryIfEmpty(pathFileFromDotMyGit);
            }
        }

        /// Update INDEX
        std::string indexUpdate;
        for (const auto& wantedEntry : entryToCopy)
        {
            indexUpdate += wantedEntry.second + ' ' + wantedEntry.first + '\n';
        }
        std::string compressedIndex = utils::CompressString(indexUpdate);
        utils::WriteFile(utils::PathToIndex(), compressedIndex);
    }

    void GetDiffBetweenTrees (const std::map<std::string, std::string>& tree1Entries, const std::map<std::string, std::string>& tree2Entries,
                              std::map<std::string, std::string>& inTree1, std::map<std::string, std::string>& inTree1Status)
    {
        for (const auto& curEntry : tree1Entries)
        {
            std::string curFile = curEntry.first;
            std::string curHash = curEntry.second;
            auto it = tree2Entries.find(curFile);
            if (it == tree2Entries.end())
            {
                /// Added in Tree1
                inTree1.insert({curFile, curHash});
                inTree1Status.insert({curFile, "added"});
            }
            else
            {
                if (curHash != it->second)
                {
                    /// Modified in Tree1
                    inTree1.insert({curFile, curHash});
                    inTree1Status.insert({curFile, "modified"});
                }
            }
        }
        for (const auto& ancestorEntry : tree2Entries)
        {
            std::string ancestorFile = ancestorEntry.first;
            std::string ancestorHash = ancestorEntry.second;
            auto it = tree1Entries.find(ancestorFile);
            if (it == tree1Entries.end())
            {
                /// Removed in Tree1
                inTree1.insert({ancestorFile, ancestorHash});
                inTree1Status.insert({ancestorFile, "deleted"});
            }
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
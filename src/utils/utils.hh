#pragma once

#include <fstream>
#include <map>
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <openssl/sha.h>
#include <cstdio>
#include <cstring>
#include <list>
#include <cstdlib>
#include <fnmatch.h>
#include <err.h>
#include <fcntl.h>


#include "objects/blob.hh"
#include "zlib.hh"

/// Globals
extern std::vector<std::string> g_myGitIgnorePatterns;
extern std::string g_pathToRootRepo;

/// Constexprs
constexpr const char *RequireRepoCommands[] = { "add", "status", "diff", "branch", "checkout", "commit", "push", "pull", "hash-object", "cat-file", "ls-files", "merge", "log" };
constexpr const char *RequireOneOrMoreArguments[] = { "add", "commit", "push", "pull", "hash-object", "cat-file", "merge" };

namespace utils
{
    void ExitIfTrue (bool assertion, const std::string& message);

    void ExitProgramWithMessage(const int& status, const std::string& message);

    void SetGlobalVariables ();

    bool IsDirExists(const std::string &path);

    bool IsFileExists(const std::string &path);

    bool CreateDir(const std::string &name);

    bool CreateFile(const std::string &name);

    std::string FindPathToRootRepo();

    std::string ReadFile (const std::string& path);

    void WriteFile (const std::string& path, const std::string& str);

    std::string RemoveUselessCharInPath (const std::string& path);

    std::string GetCwd ();

    std::string SHA1_Wrapper (const std::string& input);

    std::string CutFileInPath(std::string& pathRelativeToYouLong);

    std::string GetPathRelativeToYourself(const std::string& pathRelativeToYouLong);

    std::string GetPathRelativeToDotMyGit(const std::string& pathToFile);

    bool IsFileExcluded (const std::string& path);

    std::map<std::string, std::string> GetEntriesFromIndex (const std::string& input);

    std::vector<std::string> GetEntriesFromIndexAsList (const std::string& input);

    std::vector<std::string> ReadIndexAndGetEntriesIndexAsList ();

    std::map<std::string, std::string> ReadIndexAndGetEntries ();

    void IterateDir (const std::string& path, std::vector<std::string>& files);

    std::vector<std::string> GetWorkingDirectoryFiles ();

    std::vector<std::string> GetCurrentDirectoryFiles (const std::string& pathToDir);

    std::vector<std::string> ReadMyGitIgnorePatterns(const std::string& contents);

    bool DoesRequireRepo (const std::string& command);

    bool DoesRequireOneOrMoreArguments (const std::string& command);

    bool ChangeDirWrapper(const std::string& path);

    void GoToCharAfterNextSlash (const std::string& path, size_t& i);

    std::string ReadHEAD();

    std::string GetMostRecentCommit ();

    std::string GetMostRecentCommit (const std::string& headContents);

    std::string ReadBranchPathInHead ();

    std::string ReadBranchPathInHead (const std::string& headContents);

    void AddDiffCharacterBeforeLine (std::string& input, bool plus);

    std::vector<std::string> GetLinesAsVect (const std::string& input);

    std::vector<std::string> ListBranches ();

    void DisplayBranches ();

    std::string GetCurrentBranch ();

    bool CheckRepoHasOneCommit ();

    bool CheckBranchExists (const std::string& branchName);

    bool IsCommitObject (const std::string& commitHash);

    std::string GetCommitHash (const std::string& commitOrBranch);

    std::string GetTreeHashFromCommit (const std::string& commitHash);

    bool IsWorkingDirectoryClean ();

    std::vector<std::string> ListEntriesInDirOneLayer (const std::string& path);

    bool IsDirEmpty (const std::string& path);

    bool IsAlreadyOnCommit (const std::string& commit);

    void DeleteDirectoryIfEmpty (const std::string& pathFileFromDotMyGit);

    void CreateDirectoriesAboveFile (const std::string& pathFileFromDotMyGit);

    std::string CreateDirectoriesAboveFileReturnFirstToDelete (const std::string& pathFileFromDotMyGit);

    std::string StripPaddingSpaces (const std::string& str);

    std::string GetDate ();

    void AddToNElementsList(const std::string& elm, std::list<std::string>& vect, size_t maxSize);

    std::string GetDateIntValue ();

    std::string GetDateCommitFormat ();

    void LessCommandWrapper (const std::string& contents);

    std::string PathToRootRepo ();

    std::string PathToHEAD ();

    std::string PathToIndex ();

    std::string PathToLocalConfig ();

    std::string PathToGlobalConfig ();

    std::string PathToGitIgnore ();

    std::string PathToRefsHeads ();

    std::string PathToBranch (const std::string& branchName);

    std::string PathToObjectDir (const std::string& hash);

    std::string PathToObjectFile (const std::string& hash);

    std::string AppendPathToRootRepo (const std::string& path);

    std::string AppendPathToDotMyGit (const std::string& path);
}
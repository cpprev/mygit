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
    std::string ReadFile (const std::string& path);

    void WriteFile (const std::string& path, const std::string& str);

    bool IsDirExists(const std::string &path);

    bool IsFileExists(const std::string &path);

    bool CreateDir(const std::string &name);

    bool CreateFile(const std::string &name);

    std::string FindPathToRootRepo();

    std::string CleanPath (const std::string& path);

    void GetWorkDirFileStatus (std::map<std::string, std::string>& added, std::map<std::string, std::string>& deleted,
                               std::map<std::string, std::string>& modified);

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

    std::string ReadHEAD();

    std::string GetMostRecentCommit ();

    std::string GetMostRecentCommit (const std::string& headContents);

    std::string ReadBranchPathInHead ();

    std::string ReadBranchPathInHead (const std::string& headContents);

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
}
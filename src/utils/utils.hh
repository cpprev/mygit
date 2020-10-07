#pragma once

#include <fstream>
#include <map>
#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <openssl/sha.h>
#include <cstdio>
#include <cstring>
#include <fnmatch.h>

#include "objects/blob.hh"
#include "zlib.hh"

/// Globals
extern std::vector<std::string> g_myGitIgnorePatterns;
extern std::string g_pathToRepoRoot;

/// Constexprs
constexpr const char *RequireRepoCommands[] = {"add", "status", "diff", "commit", "push", "pull", "hash-object", "cat-file", "ls-files"};

namespace utils
{
    void ExitProgramWithMessage(const int& status, const std::string& message);

    bool IsDirExists(const std::string &path);

    bool IsFileExists(const std::string &path);

    bool CreateDir(const std::string &name);

    bool CreateFile(const std::string &name);

    std::string FindPathToDotMyGit();

    std::string ReadFile (const std::string& path);

    void WriteFile (const std::string& path, const std::string& str);

    std::string RemoveUselessCharInPath (const std::string& path);

    std::string GetCwd ();

    std::string SHA1_Wrapper (const std::string& input);

    std::string CutFileInPath(std::string& pathRelativeToYouLong);

    std::string GetPathRelativeToYourself(const std::string& pathRelativeToYouLong);

    std::string GetPathRelativeToDotMyGit(const std::string& pathToFile, const std::string& pathToDotMyGit);

    bool IsFileExcluded (const std::string& path);

    std::map<std::string, std::string> GetEntriesFromIndex (const std::string& input);

    std::vector<std::string> GetEntriesFromIndexAsList (const std::string& input);

    std::vector<std::string> ReadIndexAndGetEntriesIndexAsList (const std::string& pathToDotMyGit);

    void IterateDir (const std::string& path, std::vector<std::string>& files);

    std::vector<std::string> GetWorkingDirectoryFiles (const std::string& pathToDotMyGit);

    std::vector<std::string> GetCurrentDirectoryFiles (const std::string& pathToDir);

    std::vector<std::string> ReadMyGitIgnorePatterns(const std::string& contents);

    bool DoesRequireRepo (const std::string& command);
}
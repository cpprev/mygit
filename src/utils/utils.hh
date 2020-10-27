#pragma once

#include <iostream>
#include <map>
#include <string>
#include <vector>

typedef void (*MyGitCommand)(int argc, char *argv[]);

/// Globals
extern std::string g_DB_FILE;
extern std::vector<std::string> g_myGitIgnorePatterns;
extern std::string g_pathToRootRepo;
extern std::map<std::string, MyGitCommand> g_commands;

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

    bool IsWorkingDirectoryClean ();

    std::vector<std::string> ListEntriesInDirOneLayer (const std::string& path);

    bool IsDirEmpty (const std::string& path);

    void DeleteDirectoryIfEmpty (const std::string& pathFileFromDotMyGit);

    std::string CreateDirectoriesAboveFileReturnFirstToDelete (const std::string& pathFileFromDotMyGit);

    void WriteEntriesToIndex (const std::map<std::string, std::string>& entries);
}
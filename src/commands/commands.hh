#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <list>

#include "utils/options.hh"

namespace mygit
{
    /// MyGit commands
    void init(int argc = 0, char *argv[] = nullptr);
    void add(int argc, char *argv[]);
    void ls_files (int argc, char *argv[]);
    void hash_object (int argc, char *argv[]);
    void cat_file (int argc, char *argv[]);
    void status(int argc, char *argv[]);
    void write_tree (int argc, char *argv[]);
    void diff (int argc, char *argv[]);
    void commit (int argc, char *argv[]);
    void log(int argc, char *argv[]);
    void branch (int argc, char *argv[]);
    void checkout (int argc, char *argv[]);
    void config (int argc, char *argv[]);
    void merge (int argc, char *argv[]);

    /// Sub functions
    std::string status_str ();
    void UpdateIndex(const std::string& pathToFile, const options::AddOptions& opt, const std::vector<std::string>& removeFromIndex);
    void UpdateIndexMultipleFiles(const std::vector<std::string>& pathsFiles, const options::AddOptions& opt, const std::vector<std::string>& removeFromIndex);
    std::string write_tree_wrap ();
    std::string diff_str ();
    std::vector<std::vector<int>> BuildLcsMatrix (const std::vector<std::string>& oldFileLines, const std::vector<std::string>& newFileLines);
    void GetDiffModifiedFiles (const std::vector<std::vector<int>>& lookup, const std::vector<std::string>& x,
                               const std::vector<std::string>& y, int m, int n, std::string& result, int& count, std::list<std::string>& previousLines);
    void FastForwardMerge (const std::string& hashCommitCurrent, const std::string& hashCommitToMerge);
    void RecursiveMerge (const options::MergeOptions& opt, const std::string& hashCommitCurrent, const std::string& hashCommitToMerge);
    void GetMergeDiff (const std::string& mergeBranchName, const std::vector<std::vector<int>>& lookup, const std::vector<std::string>& x,
                       const std::vector<std::string>& y, int m, int n, std::string& result, std::string& beforeState, bool& after);
    void commit_wrapper (const options::CommitOptions& opt, const std::string& secondParent = "");
    std::string log_str(const options::LogOptions& opt);
}
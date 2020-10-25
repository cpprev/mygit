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
    void init();
    void add(const options::AddOptions& opt);
    void ls_files ();
    void hash_object (const options::HashObjectOptions& opt);
    void cat_file (const options::CatFileOptions& opt);
    void status();
    void write_tree ();
    void commit (const options::CommitOptions& opt, const std::string& secondParent = "");
    void diff ();
    void log(const options::LogOptions& opt);
    void branch (const options::BranchOptions& opt);
    void checkout (const options::CheckoutOptions& opt);
    void config (const options::ConfigOptions& opt);
    void merge (const options::MergeOptions& opt);

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
}
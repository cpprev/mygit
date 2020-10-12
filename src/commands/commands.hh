#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <cstring>
#include <algorithm>
#include <map>

#include "objects/object_type.hh"
#include "utils/utils.hh"
#include "objects/tree.hh"
#include "objects/commit.hh"
#include "utils/options.hh"
#include "utils/zlib.hh"

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
    void commit (const options::CommitOptions& opt);
    void diff ();
    void log(const options::LogOptions& opt);

    /// Sub functions
    std::string status_str ();
    void UpdateIndex(const std::string& pathToFile, const options::AddOptions& opt);
    void UpdateIndexMultipleFiles(const std::vector<std::string>& pathsFiles, const options::AddOptions& opt);
    std::string write_tree_wrap ();
    std::string diff_str ();
    std::vector<std::vector<int>> BuildLcsMatrix (const std::vector<std::string>& oldFileLines, const std::vector<std::string>& newFileLines);
    void GetDiffModifiedFiles (const std::vector<std::vector<int>>& c, const std::vector<std::string>& x,
                               const std::vector<std::string>& y, int i, int j, std::string& result);
}
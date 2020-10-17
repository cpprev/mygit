#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <cstring>
#include <algorithm>
#include <map>
#include <cstdlib>
#include <sys/types.h>
#include <sys/wait.h>

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
    void branch (const options::BranchOptions& opt);
    void checkout (const options::CheckoutOptions& opt);


    /// Sub functions
    std::string status_str ();
    void UpdateIndex(const std::string& pathToFile, const options::AddOptions& opt, const std::vector<std::string>& removeFromIndex);
    void UpdateIndexMultipleFiles(const std::vector<std::string>& pathsFiles, const options::AddOptions& opt, const std::vector<std::string>& removeFromIndex);
    std::string write_tree_wrap ();
    std::string diff_str ();
    std::vector<std::vector<int>> BuildLcsMatrix (const std::vector<std::string>& oldFileLines, const std::vector<std::string>& newFileLines);
    void GetDiffModifiedFiles (const std::vector<std::vector<int>>& lookup, const std::vector<std::string>& x,
                               const std::vector<std::string>& y, int m, int n, std::string& result);
}
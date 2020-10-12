#pragma once

#include <vector>
#include <iostream>
#include <map>
#include <set>
#include <string>

#include "utils/utils.hh"

namespace objects
{
    class Commit
    {
    public:
        Commit(const std::string& hashTree, const std::string& hashParentCommit, const std::string& author,
                                const std::string& committer, const std::string& commitMessage);
        void SetupCommit(const std::string& hash);
        std::string ToString() const;
        std::string ToHash();

        std::string _contentsCommitToHash;
        std::string _contentsCommit;
    };

    std::string ExtractParentCommit (const std::string& input);
}
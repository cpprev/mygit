#pragma once

#include <vector>
#include <iostream>
#include <map>
#include <set>
#include <string>

namespace objects
{
    class Commit
    {
    public:
        Commit(const std::string& hashTree, const std::string& hashParentCommit, const std::string& author,
                                const std::string& committer, const std::string& commitMessage, const std::string& dateString);
        void SetupCommit(const std::string& input);
        std::string ToString() const;
        std::string ToHash();
        static std::string ReadCommitToLogFormat(const std::string& input, const std::string& hash);

        std::string _contentsCommitToHash;
        std::string _contentsCommit;
    };

    std::string ExtractParentCommit (const std::string& input);
}
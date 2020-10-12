#include "commit.hh"

namespace objects
{
    Commit::Commit(const std::string& hashTree, const std::string& hashParentCommit, const std::string& author,
                   const std::string& committer, const std::string& commitMessage)
    {
        _contentsCommitToHash = hashTree;
        _contentsCommit = hashTree + hashParentCommit + author + committer + commitMessage;
    }

    void Commit::SetupCommit(const std::string& hash)
    {
        /// Store the commit
        std::string commitDirName = g_pathToRootRepo + "/.mygit/objects/" + hash.substr(0, 2);
        if (not utils::IsDirExists(commitDirName))
            utils::CreateDir(commitDirName);

        /// Fill in the contents of the commit (compressed file)
        std::string commitFilePath = commitDirName + "/" + hash.substr(2);

        /// Sets the commit metadata
        std::string commit_data = "type=commit\nsize=" + std::to_string(_contentsCommit.size()) + "\n\n" + _contentsCommit;

        utils::WriteFile(commitFilePath, utils::CompressString(commit_data));
    }

    std::string Commit::ToString() const
    {
        return "commit;" + _contentsCommitToHash + ';';
    }

    std::string Commit::ToHash()
    {
        /// The Commit hash is made out of the ToString() result
        std::string commitString = this->ToString();
        return utils::SHA1_Wrapper(commitString);
    }

    std::string ExtractParentCommit (const std::string& input)
    {
        std::string parentStr = "parent ";
        std::string::size_type ind = input.find(parentStr);
        if (ind == std::string::npos)
            return "";
        std::string res;
        for (size_t i = ind + parentStr.size(); i < input.size() and input[i] != '\n'; i++)
        {
            res += input[i];
        }
        return res;
    }
}
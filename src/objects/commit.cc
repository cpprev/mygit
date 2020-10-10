#include "commit.hh"

namespace objects
{
    Commit::Commit(const std::string& hashTree, const std::string& hashParentCommit, const std::string& author,
                   const std::string& committer, const std::string& commitMessage)
    {
        _contentsCommitToHash = hashTree;
        _contentsCommit = hashTree + hashParentCommit + author + committer + commitMessage;
        _hashCommit = ToHash();
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

    std::string Commit::CreateCommit ()
    {
        /// Create the commit and generate the hash from it
        std::string hash = ToHash();

        /// Setup commit (directory and file filling)
        SetupCommit(hash);

        return hash;
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
}
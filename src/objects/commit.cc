#include "commit.hh"
#include "utils/get_paths.hh"

namespace objects
{
    Commit::Commit(const std::string& hashTree, const std::string& hashParentCommit, const std::string& author,
                   const std::string& committer, const std::string& commitMessage, const std::string& date)
    {
        _contentsCommitToHash = hashTree;
        _contentsCommit = hashTree + hashParentCommit + author + committer + date + commitMessage;
    }

    void Commit::SetupCommit(const std::string& hash)
    {
        /// Store the commit
        std::string commitDirName = utils::PathToObjectDir(hash);
        if (not utils::IsDirExists(commitDirName))
            utils::CreateDir(commitDirName);

        /// Fill in the contents of the commit (compressed file)
        std::string commitFilePath = utils::PathToObjectFile(hash);

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

    std::string Commit::ReadCommitToLogFormat(const std::string& input, const std::string& hash)
    {
        std::string res, key, value, message;
        bool hit = false, copy_message = false;
        auto ind = input.find("author ");
        res += "\033[1;33mcommit " + hash + "\033[0m\n";
        for (size_t i = ind; i < input.size(); i++)
        {
            if (copy_message)
                message += input[i];
            if (input[i] == '\n')
            {
                if (key == "date")
                {
                    res += "Date: " + value + "\n";
                }
                else if (key == "author")
                {
                    res += "Author: " + value + "\n";
                }

                key.clear();
                value.clear();
                hit = false;

                if (i + 1 < input.size() and input[i] == '\n' and input[i + 1] == '\n')
                {
                    copy_message = true;
                    i += 1;
                }
            }
            else if (not hit and input[i] == ' ')
            {
                hit = true;
            }
            else
            {
                if (hit)
                    value += input[i];
                else
                    key += input[i];
            }
        }
        res += "\n\t" + message + "\n" + "\033[1;32m___\033[0m\n\n";
        return res;
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
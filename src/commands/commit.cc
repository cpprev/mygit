#include "commands/commands.hh"

#include "objects/commit.hh"

#include "utils/wrappers.hh"
#include "utils/get_paths.hh"
#include "utils/config.hh"
#include "utils/utils.hh"

namespace mygit
{
    void commit (const options::CommitOptions& opt, const std::string& secondParent)
    {
        std::string hashTree = write_tree_wrap();
        std::string headContents = utils::ReadHEAD();
        std::string hashParentCommit = utils::GetMostRecentCommit(headContents);

        std::string hashTreeString = "tree " + hashTree + "\n";
        std::string hashParentCommitString = (not hashParentCommit.empty()) ? ("parent " + hashParentCommit + "\n") : "";
        /// Merge case (mygit merge calls this method)
        if (not secondParent.empty())
            hashParentCommitString += "parent " + secondParent + "\n";
        std::string authorName = g_Config.GetVariable("user.name");
        std::string authorEmail = g_Config.GetVariable("user.email");
        utils::ExitIfTrue(authorName.empty() or authorEmail.empty(), "No username or useremail found, consider using mygit config --add user.name \"...\"");
        std::string dateCommitFormat = utils::GetDateCommitFormat();
        std::string authorString = "author " + authorName + " <" + authorEmail + "> " + dateCommitFormat + "\n";
        std::string committerString = "comitter " + authorName + " <" + authorEmail + "> " + dateCommitFormat + "\n";
        std::string dateString = "date " + utils::GetDate() + "\n";
        std::string commitMessageString = "\n" + opt.commitMessage + "\n";

        objects::Commit commit = objects::Commit(hashTreeString, hashParentCommitString, authorString, committerString, commitMessageString, dateString);
        std::string hashCommit = commit.ToHash();

        /// Error check that there is something to commit
        std::string pathToCommit = utils::PathToObjectFile(hashCommit);
        utils::ExitIfTrue(utils::IsFileExists(pathToCommit), "Nothing to commit.");

        commit.SetupCommit(hashCommit);

        /// First commit case
        if (headContents.empty())
        {
            utils::WriteFile(utils::PathToHEAD(), "ref: refs/heads/master");
            utils::WriteFile(utils::PathToBranch("master"), hashCommit);
        }
        /// Detached case
        else if (headContents.find("ref: ") == std::string::npos)
        {
            utils::WriteFile(utils::PathToHEAD(), hashCommit);
        }
        /// Not first commit case
        else
        {
            std::string branchPath = utils::AppendPathToDotMyGit(utils::ReadBranchPathInHead(headContents));
            utils::WriteFile(branchPath, hashCommit);
        }

        std::cout << "[" << hashCommit << "] " << opt.commitMessage << "\n";
    }
}
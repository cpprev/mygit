#include "commands.hh"

namespace mygit
{
    void commit (const options::CommitOptions& opt)
    {
        std::string hashTree = write_tree_wrap();
        std::string hashParentCommit = utils::ReadHEAD();

        std::string hashTreeString = "tree  " + hashTree + "\n";
        std::string hashParentCommitString = (not hashParentCommit.empty()) ? ("parent  " + hashParentCommit + "\n") : "";
        /// FIXME Author / Committer
        std::string authorString = "author  FIXME Author  <parse.gitconfig@later.com>  Insert time HERE\n";
        std::string committerString = "commiter  FIXME Commiter  <parse.gitconfig@later.com>  Insert time HERE\n";;
        std::string commitMessageString = "\n" + opt.commitMessage + "\n";

        objects::Commit commit = objects::Commit(hashTreeString, hashParentCommitString, authorString, committerString, commitMessageString);
        std::string hashCommit = commit.ToHash();

        /// Error check that there is something to commit
        std::string pathToCommit = g_pathToRootRepo + "/.mygit/objects/" + hashCommit.substr(0, 2) + "/" + hashCommit.substr(2);
        utils::ExitIfTrue(utils::IsFileExists(pathToCommit), "Nothing to commit.");

        commit.SetupCommit(hashCommit);

        /// Update HEAD
        utils::WriteFile(g_pathToRootRepo + "/.mygit/HEAD", hashCommit + "\n");

        std::cout << "[" << hashCommit << "] " << opt.commitMessage << "\n";
    }
}
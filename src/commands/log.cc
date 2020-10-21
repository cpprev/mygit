#include "commands.hh"

namespace mygit
{
    std::string log_str(const options::LogOptions& opt)
    {
        (void) opt;

        std::string headContents = utils::GetMostRecentCommit();
        std::string currentCommit = headContents;
        std::string output;
        while (not currentCommit.empty())
        {
            /// Read commit
            std::string commitPath = g_pathToRootRepo + "/.mygit/objects/" + currentCommit.substr(0, 2) + "/" + currentCommit.substr(2);
            std::string commitRawContent = utils::DecompressString(utils::ReadFile(commitPath));
            std::string contentContent = objects::GetContentBlobDecompressed(commitRawContent);

            output += "\033[1;33mcommit " + currentCommit + "\033[0m\n\033[1;34m" + contentContent +
                    "\033[1;32m___\033[0m\n\n";

            /// Update commit
            currentCommit = objects::ExtractParentCommit(contentContent);
        }
        return output;
    }

    void log(const options::LogOptions& opt)
    {
        std::string logStr = log_str(opt);
        utils::LessCommandWrapper(logStr);
    }
}
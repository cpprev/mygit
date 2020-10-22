#include "commands.hh"
#include "utils/wrappers.hh"
#include "utils/get_paths.hh"

namespace mygit
{
    std::string log_str(const options::LogOptions& opt)
    {
        (void) opt;

        std::string headContents = utils::GetMostRecentCommit();
        std::string currentCommitHash = headContents;
        std::string output;
        while (not currentCommitHash.empty())
        {
            std::string commitPath = utils::PathToObjectFile(currentCommitHash);
            std::string commitRawContent = utils::DecompressString(utils::ReadFile(commitPath));
            std::string contentContent = objects::GetContentBlobDecompressed(commitRawContent);

            std::string commitContent = objects::Commit::ReadCommitToLogFormat(contentContent, currentCommitHash);

            output += commitContent;

            /// Update commit
            currentCommitHash = objects::ExtractParentCommit(contentContent);
        }
        return output;
    }

    void log(const options::LogOptions& opt)
    {
        std::string logStr = log_str(opt);
        utils::LessCommandWrapper(logStr);
    }
}
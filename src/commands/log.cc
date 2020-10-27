#include "commands/commands.hh"

#include "objects/blob.hh"
#include "objects/commit.hh"

#include "utils/wrappers.hh"
#include "utils/get_paths.hh"
#include "utils/zlib.hh"
#include "utils/utils.hh"

namespace mygit
{
    void log(int argc, char *argv[])
    {
        /// Create opt object
        auto opt = options::LogOptions(argc, argv);

        std::string logStr = log_str(opt);
        utils::LessCommandWrapper(logStr);
    }

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
}
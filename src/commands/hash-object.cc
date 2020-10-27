#include "commands/commands.hh"

#include "objects/blob.hh"

#include "utils/wrappers.hh"
#include "utils/get_paths.hh"
#include "utils/utils.hh"

namespace mygit
{
    void hash_object (int argc, char *argv[])
    {
        /// Create opt object
        auto opt = options::HashObjectOptions(argc, argv);

        if (opt.type == objects::BLOB)
        {
            /// FIXME With all params
            /// Error checking
            utils::ExitIfTrue(not utils::IsFileExists(opt.params[0]), "First parameter to hash-object has to be a file.");

            std::string filename = opt.params[0];
            std::string contentsFile = utils::ReadFile(filename);
            std::string pathFileFromDotMyGit = utils::GetPathRelativeToDotMyGit(filename);

            objects::Blob blob = objects::Blob(pathFileFromDotMyGit, utils::AppendPathToRootRepo(pathFileFromDotMyGit));
            std::string hash = blob.ToHash();
            std::cout << hash << "\n";

            if (opt.write)
            {
                objects::SetupBlob(blob, hash);
            }
        }
        else
        {
            utils::ExitProgramWithMessage(1, "FIXME: NOT IMPLEMENTED (HASH OBJECT: COMMIT/TREE)");
        }
    }
}
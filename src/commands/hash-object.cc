#include "commands.hh"

namespace mygit
{
    void hash_object (const options::HashObjectOptions& opt)
    {
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
        else if (opt.type == objects::TREE)
        {

        }
        else if (opt.type == objects::COMMIT)
        {
            utils::ExitProgramWithMessage(1, "FIXME: NOT IMPLEMENTED (HASH OBJECT: COMMIT/TREE)");
        }
    }
}
#include "commands.hh"

namespace mygit
{
    void hash_object (const options::HashObjectOptions& opt)
    {
        /// Error checking
        utils::ExitIfTrue(not utils::IsFileExists(opt.first_param), "First parameter to hash-object has to be a file.");

        std::string filename = opt.first_param;
        std::string contentsFile = utils::ReadFile(filename);
        std::string pathFileFromDotMyGit = utils::GetPathRelativeToDotMyGit(filename);

        if (opt.type == objects::BLOB)
        {
            objects::Blob blob = objects::Blob(pathFileFromDotMyGit, g_pathToRootRepo + '/' + pathFileFromDotMyGit);
            std::string hash = blob.ToHash();
            std::cout << hash << "\n";

            if (opt.write)
            {
                objects::SetupBlob(blob, hash);
            }
        }
        else if (opt.type == objects::COMMIT or opt.type == objects::TREE)
        {
            utils::ExitProgramWithMessage(1, "FIXME: NOT IMPLEMENTED (HASH OBJECT: COMMIT/TREE)");
        }
    }
}
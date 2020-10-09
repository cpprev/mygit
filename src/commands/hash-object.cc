#include "commands.hh"

namespace mygit
{
    void hash_object (const HashObjectOptions& opt)
    {
        if (not utils::IsFileExists(opt.first_param))
        {
            utils::ExitProgramWithMessage(1, "First parameter to hash-object has to be a file.");
        }

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

    HashObjectOptions::HashObjectOptions(int argc, char *argv[])
    {
        for (int i = 2; i < argc; i++)
        {
            if (i == 2)
            {
                first_param = argv[i];
            }
            else if (strcmp(argv[i], "--write") == 0)
            {
                write = true;
            }
            else if (i + 1 < argc and strcmp(argv[i], "--type") == 0)
            {
                type = objects::ObjectTypeFromString(argv[i + 1]);
                i += 1;
            }
        }
    }
}
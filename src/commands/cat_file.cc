#include "commands/commands.hh"

#include "objects/blob.hh"

#include "utils/wrappers.hh"
#include "utils/get_paths.hh"
#include "utils/zlib.hh"
#include "utils/utils.hh"

namespace mygit
{
    void cat_file (int argc, char *argv[])
    {
        /// Create opt object
        auto opt = options::CatFileOptions(argc, argv);

        std::string hash = opt.first_param;
        std::string hashPath = utils::PathToObjectFile(hash);

        /// Error checking
        utils::ExitIfTrue(not utils::IsFileExists(hashPath), "Object is not present in " + g_DB_FILE + "/objects directory.");

        std::string contentsFile = utils::ReadFile(hashPath);
        std::string decompressed = utils::DecompressString(contentsFile);
        if (opt.type or opt.size)
        {
            if (opt.type)
                std::cout << "Type of object : " << objects::GetTypeBlobDecompressed(decompressed) << "\n";
            if (opt.size)
            {
                std::cout << "Size of compressed file : " << contentsFile.size() << "\n";
                std::cout << "Size of decompressed file : " << decompressed.size() << "\n";
            }
        }
        else
        {
            if (opt.raw)
            {
                std::cout << decompressed;
            }
            else
            {
                std::cout << objects::GetContentBlobDecompressed(decompressed);
            }
        }
    }
}
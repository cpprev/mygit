#include "commands.hh"

namespace mygit
{
    void cat_file (const options::CatFileOptions& opt)
    {
        std::string hash = opt.first_param;
        std::string hashPath = utils::PathToObjectFile(hash);

        /// Error checking
        utils::ExitIfTrue(not utils::IsFileExists(hashPath), "Object is not present in .mygit/objects directory.");

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
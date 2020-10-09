#include "commands.hh"

namespace mygit
{
    void cat_file (const CatFileOptions& opt)
    {
        std::string hash = opt.first_param;
        std::string hashPath = g_pathToRootRepo + "/.mygit/objects/" + hash.substr(0, 2) + '/' + hash.substr(2);
        if (not utils::IsFileExists(hashPath))
        {
            utils::ExitProgramWithMessage(1, "Object is not present in .mygit/objects directory.");
        }

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

    CatFileOptions::CatFileOptions(int argc, char *argv[])
    {
        for (int i = 2; i < argc; i++)
        {
            if (i == 2)
                first_param = argv[i];
            else if (strcmp(argv[i], "-s") == 0)
                size = true;
            else if (strcmp(argv[i], "-t") == 0)
                type = true;
            else if (strcmp(argv[i], "-p") == 0)
                content = true;
            else if (strcmp(argv[i], "-r") == 0)
                raw = true;
        }
    }
}
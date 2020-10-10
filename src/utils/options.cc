#include "options.hh"

namespace options
{
    HashObjectOptions::HashObjectOptions(int argc, char *argv[])
    {
        for (int i = 2; i < argc; i++)
        {
            if (argv[i][0] != '-')
                params.push_back(argv[i]);
            else if (strcmp(argv[i], "--write") == 0)
                write = true;
            else if (i + 1 < argc and strcmp(argv[i], "--type") == 0)
            {
                type = objects::ObjectTypeFromString(argv[i + 1]);
                i += 1;
            }
            else
                utils::ExitProgramWithMessage(1, "Unknown option to command 'hash-object': " + std::string(argv[i]));
        }

        utils::ExitIfTrue(params.empty(), "You have to specify a hash to hash-object.");
    }

    CatFileOptions::CatFileOptions(int argc, char *argv[])
    {
        for (int i = 2; i < argc; i++)
        {
            if (argv[i][0] != '-')
                first_param = argv[i];
            else if (strcmp(argv[i], "-s") == 0)
                size = true;
            else if (strcmp(argv[i], "-t") == 0)
                type = true;
            else if (strcmp(argv[i], "-p") == 0)
                content = true;
            else if (strcmp(argv[i], "-r") == 0)
                raw = true;
            else
                utils::ExitProgramWithMessage(1, "Unknown option to command 'cat-file': " + std::string(argv[i]));
        }

        utils::ExitIfTrue(first_param.empty(), "You have to specify a hash to cat-file.");
    }

    AddOptions::AddOptions(int argc, char *argv[])
    {
        for (int i = 2; i < argc; i++)
        {
            if (argv[i][0] != '-')
                pathArguments.push_back(argv[i]);
            else if (strcmp(argv[i], "-f") == 0)
                force = true;
            else
                utils::ExitProgramWithMessage(1, "Unknown option to command 'add': " + std::string(argv[i]));
        }

        utils::ExitIfTrue(pathArguments.empty(), "You have to specify path arguments to add.");
    }
}
#include <cstring>

#include "utils/options.hh"
#include "utils/wrappers.hh"


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

    CommitOptions::CommitOptions(int argc, char *argv[])
    {
        for (int i = 2; i < argc; i++)
        {
            if (i + 1 < argc and strcmp(argv[i], "-m") == 0)
            {
                commitMessage = argv[i + 1];
                i++;
            }
            else
                utils::ExitProgramWithMessage(1, "Unknown option to command 'commit': " + std::string(argv[i]));
        }
    }

    LogOptions::LogOptions(int argc, char *argv[])
    {
        (void) argv;
        for (int i = 2; i < argc; i++)
        {
            /// FIXME
        }
    }

    BranchOptions::BranchOptions(int argc, char *argv[])
    {
        if (argc == 2)
            displayBranches = true;
        for (int i = 2; i < argc; i++)
        {
            if (argv[i][0] != '-')
                branchToCreate = argv[i];
            else if (i + 1 < argc and strcmp(argv[i], "-d") == 0)
            {
                branchToDelete = argv[i + 1];
                i++;
            }
            else
                utils::ExitProgramWithMessage(1, "Unknown option to command 'branch': " + std::string(argv[i]));
        }
    }

    CheckoutOptions::CheckoutOptions(int argc, char *argv[])
    {
        if (argc == 2)
            displayBranches = true;
        for (int i = 2; i < argc; i++)
        {
            if (argv[i][0] != '-')
                commitToCheckoutOn = argv[i];
            else
                utils::ExitProgramWithMessage(1, "Unknown option to command 'checkout': " + std::string(argv[i]));
        }
    }

    ConfigOptions::ConfigOptions(int argc, char *argv[])
    {
        for (int i = 2; i < argc; i++)
        {
            if (strcmp(argv[i], "--local") == 0)
            {
                local = true;
                global = false;
            }
            else if (strcmp(argv[i], "--global") == 0)
            {
                local = false;
                global = true;
            }
            else if (i + 2 < argc and strcmp(argv[i], "--add") == 0)
            {
                add = { argv[i + 1], argv[i + 2] };
                i += 2;
            }
            else if (i + 1 < argc and strcmp(argv[i], "--unset") == 0)
            {
                unset = argv[i + 1];
                i += 1;
            }
            else if (i + 1 < argc and strcmp(argv[i], "--get") == 0)
            {
                get = argv[i + 1];
                i += 1;
            }
            else
                utils::ExitProgramWithMessage(1, "Unknown option to command 'config': " + std::string(argv[i]));
        }
    }

    MergeOptions::MergeOptions(int argc, char *argv[])
    {
        for (int i = 2; i < argc; i++)
        {
            if (argv[i][0] != '-')
                branchToMerge = argv[i];
            else
                utils::ExitProgramWithMessage(1, "Unknown option to command 'merge': " + std::string(argv[i]));
        }
    }
}
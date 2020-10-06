#include <cassert>
#include <cstdlib>

#include "utils/utils.hh"
#include "commands/commands.hh"

std::vector<std::string> g_myGitIgnorePatterns = std::vector<std::string>();

std::string init_test ()
{
    /// Setup test area
    utils::CreateDir("testarea/");
    chdir("testarea/");

    /// Git init
    mygit::init();
    std::string pathToDotMyGit = utils::FindPathToDotMyGit();

    return pathToDotMyGit;
}

void clean_test (std::string& pathToDotMyGit)
{
    std::string command = "rm -rf " + pathToDotMyGit + "/.mygit";
    system(command.c_str());
    std::string cmd2 = "rm -rf " + pathToDotMyGit + "/../testarea/";
    system(cmd2.c_str());
}

void process_test_bool (bool cond, std::string test_name = "")
{
    if (cond)
        std::cout << "\033[1;32m" << "[OK] [" << test_name << "]" << "\033[0m" << "\n";
    else
        std::cout << "\033[1;31m" << "[KO] [" << test_name << "]" << "\033[0m" << "\n";
}

void process_test_findstr (std::string::size_type cond, std::string test_name = "")
{
    if (cond != std::string::npos)
        std::cout << "\033[1;32m" << "[OK] [" << test_name << "]" << "\033[0m" << "\n";
    else
        std::cout << "\033[1;31m" << "[KO] [" << test_name << "]" << "\033[0m" << "\n";
}

void TestStatusOneFileAtRoot (std::string pathToDotMyGit)
{
    (void) pathToDotMyGit;

    std::cout << "TEST 1: 'TestStatusOneFileAtRoot'\n";

    /// setup file
    std::string filename = "test.txt";
    std::string fileContents = "this is some dummy text!!!!z!!z!zz,ikoaejnioazojine\naijajijiazajizz\nauiahuiauhi\n";
    utils::WriteFile(filename, fileContents);

    /// Verify that filename is in mygit status output
    std::string output = mygit::status_str();
    process_test_findstr(output.find('\t' + filename), "filename in status output");
}

void TestStatusOneFileInSubdirWhileInSubDir (std::string pathToDotMyGit)
{
    std::cout << "TEST 2: 'TestStatusOneFileInSubdirWhileInSubDir'\n";

    std::string cwd = utils::GetCwd();
    utils::CreateDir("dummySubDir/");
    chdir("dummySubDir/");

    /// setup file
    std::string filename = "hehehehe.txt";
    std::string fileContents = "ezuhizuhie\nziazijh\nnazhzyhz\n";
    utils::WriteFile(filename, fileContents);


    pathToDotMyGit = utils::FindPathToDotMyGit();

    /// Verify that filename is in mygit status output
    std::string output = mygit::status_str();
    process_test_findstr(output.find('\t' + filename), "filename in status output");

    /// chdir back to origin
    chdir(cwd.c_str());
}


int main()
{
    /// Init test env
    std::string pathToDotMyGit = init_test();

    TestStatusOneFileAtRoot(pathToDotMyGit);

    /// Clear dummy files
    system("rm -rf *");

    TestStatusOneFileInSubdirWhileInSubDir(pathToDotMyGit);

    /// Clear dummy files
    system("rm -rf *");

    /// Clean test
    clean_test(pathToDotMyGit);

    return 0;
}
#include <cassert>
#include <cstdlib>

#include "utils/utils.hh"
#include "commands/commands.hh"

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

void TestStatusOneFileAtRoot ()
{
    /// setup file
    std::string filename = "test.txt";
    std::string fileContents = "this is some dummy text!!!!z!!z!zz,ikoaejnioazojine\naijajijiazajizz\nauiahuiauhi\n";
    utils::WriteFile(filename, fileContents);

    /// Verify that filename is in mygit status output
    std::string output = mygit::status_str();
    process_test_findstr(output.find("\tAdded:  \t" + filename), "TestStatusOneFileAtRoot");
}

void TestStatusOneFileInSubdirWhileInSubDir (std::string pathToDotMyGit)
{
    std::string cwd = utils::GetCwd();
    utils::CreateDir("dummySubDir000/");
    chdir("dummySubDir000/");

    /// setup file
    std::string filename = "heheaaaahehe.txt";
    std::string fileContents = "ezuhizuhie\nziazijh\nnazhzyhz\n";
    utils::WriteFile(filename, fileContents);


    pathToDotMyGit = utils::FindPathToDotMyGit();

    /// Verify that filename is in mygit status output
    std::string output = mygit::status_str();
    process_test_findstr(output.find("\tAdded:  \t" + filename), "TestStatusOneFileInSubdirWhileInSubDir");

    /// chdir back to origin
    chdir(cwd.c_str());
}

void TestStatusOneFileInSubdirWhileNotInSubDir ()
{
    utils::CreateDir("dummySubDir111/");

    /// setup file
    std::string filename = "dummySubDir111/hehehehzzaeezaezaze.txt";
    std::string fileContents = "ezuhizuhizaeaze\nzizzazaazijh\nnazhzyhz\n";
    utils::WriteFile(filename, fileContents);

    /// Verify that filename is in mygit status output
    std::string output = mygit::status_str();
    process_test_findstr(output.find("\tAdded:  \t" + filename), "TestStatusOneFileInSubdirWhileNotInSubDir");
}

void TestStatusOneFileAtRootWhileInSubDir (std::string pathToDotMyGit)
{
    /// setup file
    std::string filename = "hezazazazaeazeazhehehe.txt";
    std::string fileContents = "ezuhizazazuhie\nziaziezazaeezajh\nnazhzyhz\n";
    utils::WriteFile(filename, fileContents);

    std::string cwd = utils::GetCwd();
    utils::CreateDir("dummySubDir102/");
    chdir("dummySubDir102/");

    pathToDotMyGit = utils::FindPathToDotMyGit();

    /// Verify that filename is in mygit status output
    std::string output = mygit::status_str();
    process_test_findstr(output.find("\tAdded:  \t../" + filename), "TestStatusOneFileAtRootWhileInSubDir");

    /// chdir back to origin
    chdir(cwd.c_str());
}

void TestStatusOneFileAtRootDisappearWhileAtRoot ()
{
    /// setup file
    std::string filename = "test.txt";
    std::string fileContents = "this is some dummy text!!!!z!!z!zz,ikoaejnioazojine\naijajijiazajizz\nauiahuiauhi\n";
    utils::WriteFile(filename, fileContents);

    remove(filename.c_str());

    /// Verify that filename is in mygit status output
    std::string output = mygit::status_str();
    process_test_findstr(not output.find("\tAdded:\t" + filename), "TestStatusOneFileAtRootDisappearWhileAtRoot");
}

void TestStatusOneFileAtRootDisappearWhileInSubdir ()
{
    /// setup file
    std::string filename = "tezzzt.txt";
    std::string fileContents = "this is some aazzadummy text!!!!z!!z!zz,ikoaejniazazoazojine\naijajijizazajizz\nauiahuiaukhi\n";
    utils::WriteFile(filename, fileContents);

    std::string cwd = utils::GetCwd();
    utils::CreateDir("dummySubDir101/");
    chdir("dummySubDir101/");

    std::string newPath = "../" + filename;
    remove(newPath.c_str());

    /// Verify that filename is in mygit status output
    std::string output = mygit::status_str();
    process_test_findstr(not output.find("\tAdded:\t" + newPath), "TestStatusOneFileAtRootDisappearWhileInSubdir");

    /// chdir back to origin
    chdir(cwd.c_str());
}

void TestStatusOneFileInSubdirDisappearWhileAtRoot ()
{
    utils::CreateDir("dummySubDir100/");

    /// setup file
    std::string filename = "dummySubDir100/tezzzt.txt";
    std::string fileContents = "this is some aazzaduany text!!!!z!!z!zz,ikoaejniazazoazojine\naijajijizazajizz\nauiahuiaukhi\n";
    utils::WriteFile(filename, fileContents);

    remove(filename.c_str());

    /// Verify that filename is in mygit status output
    std::string output = mygit::status_str();
    process_test_findstr(not output.find("\tAdded:\t" + filename), "TestStatusOneFileInSubdirDisappearWhileAtRoot");
}

void TestStatusOneFileAtRootModifiedWhileAtRoot ()
{
    /// setup file
    std::string filename = "test.txt";
    std::string fileContents = "this is some dummy text!!!!z!!z!zz,ikoaejnioazojine\naijajijiazajizz\nauiahuiauhi\n";
    utils::WriteFile(filename, fileContents);

    const char *argv[] = { "mygit", "add", "test.txt" };
    int argc = sizeof(argv) / sizeof(char *);
    mygit::add(mygit::AddOptions(argc, const_cast<char **>(argv)));

    utils::WriteFile(filename, fileContents + ".");

    /// Verify that filename is in mygit status output
    std::string output = mygit::status_str();
    process_test_findstr(output.find("\tModified:\t" + filename), "TestStatusOneFileAtRootModifiedWhileAtRoot");
}

void TestStatusOneFileAtRootModifiedWhileInSubdir ()
{
    /// setup file
    std::string filename = "teskkkt.txt";
    std::string fileContents = "this is some dummy texazzat!!!!z!!z!zz,ikoaejnioazojine\naijajijiazajizz\nauiahuiauhi\n";
    utils::WriteFile(filename, fileContents);


    std::string cwd = utils::GetCwd();
    utils::CreateDir("dummySubDir90/");
    chdir("dummySubDir90/");

    const char *argv[] = { "mygit", "add", "../teskkkt.txt" };
    int argc = sizeof(argv) / sizeof(char *);
    mygit::add(mygit::AddOptions(argc, const_cast<char **>(argv)));

    utils::WriteFile("../" + filename, fileContents + ".");

    /// Verify that filename is in mygit status output
    std::string output = mygit::status_str();
    process_test_findstr(output.find("\tModified:\t../" + filename), "TestStatusOneFileAtRootModifiedWhileInSubdir");

    /// chdir back to origin
    chdir(cwd.c_str());
}

void TestStatusOneFileInSubdirModifiedWhileAtRoot ()
{
    utils::CreateDir("dummy1/");

    /// setup file
    std::string filename = "dummy1/tez.txt";
    std::string fileContents = "this is some aazzaduany text!!!!z!!z!zz,ikoaejnzzziazazoazojine\naijajijizazajizz\nauiahuiaukhi\n";
    utils::WriteFile(filename, fileContents);

    const char *argv[] = { "mygit", "add", filename.c_str() };
    int argc = sizeof(argv) / sizeof(char *);
    mygit::add(mygit::AddOptions(argc, const_cast<char **>(argv)));

    utils::WriteFile(filename, fileContents + ".z");


    /// Verify that filename is in mygit status output
    std::string output = mygit::status_str();
    process_test_findstr(output.find("\tModified:\t" + filename), "TestStatusOneFileInSubdirModifiedWhileAtRoot");
}

void TestStatusOneFileAtRootDeletedWhileAtRoot ()
{
    /// setup file
    std::string filename = "test.txt";
    std::string fileContents = "this is some dummy text!!!!z!!z!zz,ikoaejnioazojine\naijajijiazajizz\nauiahuiauhi\n";
    utils::WriteFile(filename, fileContents);

    const char *argv[] = { "mygit", "add", "test.txt" };
    int argc = sizeof(argv) / sizeof(char *);
    mygit::add(mygit::AddOptions(argc, const_cast<char **>(argv)));

    remove(filename.c_str());

    /// Verify that filename is in mygit status output
    std::string output = mygit::status_str();
    process_test_findstr(output.find("\tDeleted:\t" + filename), "TestStatusOneFileAtRootDeletedWhileAtRoot");
}

void TestStatusOneFileAtRootDeletedWhileInSubdir ()
{
    /// setup file
    std::string filename = "teskkkt.txt";
    std::string fileContents = "this is some dummy texazzat!!!!z!!z!zz,ikoaejnioazojine\naijajijiazajizz\nauiahuiauhi\n";
    utils::WriteFile(filename, fileContents);

    std::string cwd = utils::GetCwd();
    utils::CreateDir("dummySubDir/");
    chdir("dummySubDir/");

    const char *newPath = "../teskkkt.txt";
    const char *argv[] = { "mygit", "add", newPath };
    int argc = sizeof(argv) / sizeof(char *);
    mygit::add(mygit::AddOptions(argc, const_cast<char **>(argv)));

    remove(newPath);

    /// Verify that filename is in mygit status output
    std::string output = mygit::status_str();
    process_test_findstr(output.find("\tDeleted:\t" + std::string(newPath)), "TestStatusOneFileAtRootDeletedWhileInSubdir");

    /// chdir back to origin
    chdir(cwd.c_str());
}

void TestStatusOneFileInSubdirDeletedWhileAtRoot ()
{
    utils::CreateDir("dummySubDir99/");

    /// setup file
    std::string filename = "dummySubDir99/tezzztzz.txt";
    std::string fileContents = "this is some aazzaduany text!!!!z!!z!zz,ikoaejnzzziazazoazojine\naijajijizazajizz\nauiahuiaukhi\n";
    utils::WriteFile(filename, fileContents);

    const char *argv[] = { "mygit", "add", filename.c_str() };
    int argc = sizeof(argv) / sizeof(char *);
    mygit::add(mygit::AddOptions(argc, const_cast<char **>(argv)));

    remove(filename.c_str());

    /// Verify that filename is in mygit status output
    std::string output = mygit::status_str();
    process_test_findstr(output.find("\tDeleted:\t" + filename), "TestStatusOneFileInSubdirDeletedWhileAtRoot");
}


int main()
{
    /// Init test env (start)
    std::string pathToDotMyGit = init_test();

    TestStatusOneFileAtRoot();
    /// Clear dummy files
    system("rm -rf *");

    TestStatusOneFileInSubdirWhileInSubDir(pathToDotMyGit);
    /// Clear dummy files
    system("rm -rf *");

    TestStatusOneFileInSubdirWhileNotInSubDir();
    /// Clear dummy files
    system("rm -rf *");

    TestStatusOneFileAtRootWhileInSubDir(pathToDotMyGit);
    /// Clear dummy files
    system("rm -rf *");

    TestStatusOneFileAtRootDisappearWhileInSubdir();
    /// Clear dummy files
    system("rm -rf *");

    TestStatusOneFileAtRootDisappearWhileAtRoot();
    /// Clear dummy files
    system("rm -rf *");

    TestStatusOneFileInSubdirDisappearWhileAtRoot();
    /// Clear dummy files
    system("rm -rf *");

    TestStatusOneFileAtRootModifiedWhileAtRoot();
    /// Clear dummy files
    system("rm -rf *");

    TestStatusOneFileInSubdirModifiedWhileAtRoot();
    /// Clear dummy files
    system("rm -rf *");

    TestStatusOneFileAtRootModifiedWhileInSubdir();
    /// Clear dummy files
    system("rm -rf *");

    TestStatusOneFileAtRootDeletedWhileAtRoot();
    /// Clear dummy files
    system("rm -rf *");

    TestStatusOneFileAtRootDeletedWhileInSubdir();
    /// Clear dummy files
    system("rm -rf *");

    TestStatusOneFileInSubdirDeletedWhileAtRoot();
    /// Clear dummy files
    system("rm -rf *");

    /// Clean files (end)
    clean_test(pathToDotMyGit);

    return 0;
}
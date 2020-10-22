#include <cassert>
#include <cstdlib>

#include "utils/utils.hh"
#include "commands/commands.hh"
#include "utils/wrappers.hh"
#include "utils/zlib.hh"

std::string init_test ()
{
    /// Setup test area
    utils::CreateDir("testarea/");
    utils::ChangeDirWrapper("testarea/");

    /// Git init
    mygit::init();
    std::string pathToRootRepo = utils::FindPathToRootRepo();

    return pathToRootRepo;
}

void clean_test (std::string& pathToRootRepo)
{
    std::string command = "rm -rf " + pathToRootRepo + "/.mygit";
    system(command.c_str());
    std::string cmd2 = "rm -rf " + pathToRootRepo + "/../testarea/";
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

    /// Remove files
    remove(filename.c_str());
}

void TestStatusOneFileInSubdirWhileInSubDir (std::string pathToRootRepo)
{
    std::string dirPath = "dummySubDir000/";

    std::string cwd = utils::GetCwd();
    utils::CreateDir(dirPath);
    utils::ChangeDirWrapper(dirPath);

    /// setup file
    std::string filename = "heheaaaahehe.txt";
    std::string fileContents = "ezuhizuhie\nziazijh\nnazhzyhz\n";
    utils::WriteFile(filename, fileContents);


    pathToRootRepo = utils::FindPathToRootRepo();

    /// Verify that filename is in mygit status output
    std::string output = mygit::status_str();
    process_test_findstr(output.find("\tAdded:  \t" + filename), "TestStatusOneFileInSubdirWhileInSubDir");

    /// chdir back to origin
    utils::ChangeDirWrapper(cwd);

    /// Remove files
    remove((dirPath + filename).c_str());
    rmdir(dirPath.c_str());
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

    /// Remove files
    remove("dummySubDir111/hehehehzzaeezaezaze.txt");
    rmdir("dummySubDir111/");
}

void TestStatusOneFileAtRootWhileInSubDir (std::string pathToRootRepo)
{
    /// setup file
    std::string filename = "hezazazazaeazeazhehehe.txt";
    std::string fileContents = "ezuhizazazuhie\nziaziezazaeezajh\nnazhzyhz\n";
    utils::WriteFile(filename, fileContents);

    std::string cwd = utils::GetCwd();
    utils::CreateDir("dummySubDir102/");
    utils::ChangeDirWrapper("dummySubDir102/");

    pathToRootRepo = utils::FindPathToRootRepo();

    /// Verify that filename is in mygit status output
    std::string output = mygit::status_str();
    process_test_findstr(output.find("\tAdded:  \t../" + filename), "TestStatusOneFileAtRootWhileInSubDir");

    /// chdir back to origin
    utils::ChangeDirWrapper(cwd);

    /// Remove files
    remove("hezazazazaeazeazhehehe.txt");
    rmdir("dummySubDir102/");
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
    utils::CreateDir("dummySubDir1011/");
    utils::ChangeDirWrapper("dummySubDir1011/");

    std::string newPath = "../" + filename;
    remove(newPath.c_str());

    /// Verify that filename is in mygit status output
    std::string output = mygit::status_str();
    process_test_findstr(not output.find("\tAdded:\t" + newPath), "TestStatusOneFileAtRootDisappearWhileInSubdir");

    /// chdir back to origin
    utils::ChangeDirWrapper(cwd);

    /// Remove files
    rmdir("dummySubDir1011/");
}

void TestStatusOneFileInSubdirDisappearWhileAtRoot ()
{
    utils::CreateDir("dummySubDir123/");

    /// setup file
    std::string filename = "dummySubDir123/tezzzt.txt";
    std::string fileContents = "this is some aazzaduany text!!!!z!!z!zz,ikoaejniazazoazojine\naijajijizazajizz\nauiahuiaukhi\n";
    utils::WriteFile(filename, fileContents);

    remove(filename.c_str());

    /// Verify that filename is in mygit status output
    std::string output = mygit::status_str();
    process_test_findstr(not output.find("\tAdded:\t" + filename), "TestStatusOneFileInSubdirDisappearWhileAtRoot");

    /// Remove files
    rmdir("dummySubDir123/");
}

void TestStatusOneFileAtRootModifiedWhileAtRoot ()
{
    /// setup file
    std::string filename = "test123.txt";
    std::string fileContents = "this is some dummy text!!!!z!!z!zz,ikoaejnioazojine\naijajijiazajizz\nauiahuiauhi\n";
    utils::WriteFile(filename, fileContents);

    const char *argv[] = { "mygit", "add", "test123.txt" };
    int argc = sizeof(argv) / sizeof(char *);
    mygit::add(options::AddOptions(argc, const_cast<char **>(argv)));

    utils::WriteFile(filename, fileContents + ".");

    /// Verify that filename is in mygit status output
    std::string output = mygit::status_str();
    process_test_findstr(output.find("\tModified:\t" + filename), "TestStatusOneFileAtRootModifiedWhileAtRoot");

    /// Remove files
    remove("test123.txt");
}

void TestStatusOneFileAtRootModifiedWhileInSubdir ()
{
    /// setup file
    std::string filename = "teskkkt.txt";
    std::string fileContents = "this is some dummy texazzat!!!!z!!z!zz,ikoaejnioazojine\naijajijiazajizz\nauiahuiauhi\n";
    utils::WriteFile(filename, fileContents);


    std::string cwd = utils::GetCwd();
    utils::CreateDir("dummySubDir90/");
    utils::ChangeDirWrapper("dummySubDir90/");

    const char *argv[] = { "mygit", "add", "../teskkkt.txt" };
    int argc = sizeof(argv) / sizeof(char *);
    mygit::add(options::AddOptions(argc, const_cast<char **>(argv)));

    utils::WriteFile("../" + filename, fileContents + ".");

    /// Verify that filename is in mygit status output
    std::string output = mygit::status_str();
    process_test_findstr(output.find("\tModified:\t../" + filename), "TestStatusOneFileAtRootModifiedWhileInSubdir");

    /// chdir back to origin
    utils::ChangeDirWrapper(cwd);

    /// Remove files
    remove("teskkkt.txt");
    rmdir("dummySubDir90/");
}

void TestStatusOneFileInSubdirModifiedWhileAtRoot ()
{
    utils::CreateDir("dummy12345/");

    /// setup file
    std::string filename = "dummy12345/tez.txt";
    std::string fileContents = "this is some aazzaduany text!!!!z!!z!zz,ikoaejnzzziazazoazojine\naijajijizazajizz\nauiahuiaukhi\n";
    utils::WriteFile(filename, fileContents);

    const char *argv[] = { "mygit", "add", filename.c_str() };
    int argc = sizeof(argv) / sizeof(char *);
    mygit::add(options::AddOptions(argc, const_cast<char **>(argv)));

    utils::WriteFile(filename, fileContents + ".z");


    /// Verify that filename is in mygit status output
    std::string output = mygit::status_str();
    process_test_findstr(output.find("\tModified:\t" + filename), "TestStatusOneFileInSubdirModifiedWhileAtRoot");

    /// Remove files
    remove("dummy12345/tez.txt");
    rmdir("dummy12345/");
}

void TestStatusOneFileAtRootDeletedWhileAtRoot ()
{
    /// setup file
    std::string filename = "test12.txt";
    std::string fileContents = "this is some dummy text!!!!z!!z!zz,ikoaejnioazojine\naijajijiazajizz\nauiahuiauhi\n";
    utils::WriteFile(filename, fileContents);

    const char *argv[] = { "mygit", "add", "test12.txt" };
    int argc = sizeof(argv) / sizeof(char *);
    mygit::add(options::AddOptions(argc, const_cast<char **>(argv)));

    remove(filename.c_str());

    /// Verify that filename is in mygit status output
    std::string output = mygit::status_str();
    process_test_findstr(output.find("\tDeleted:\t" + filename), "TestStatusOneFileAtRootDeletedWhileAtRoot");

    /// Remove files
    remove("test12.txt");
}

void TestStatusOneFileAtRootDeletedWhileInSubdir ()
{
    /// setup file
    std::string filename = "teskkkt.txt";
    std::string fileContents = "this is some dummy texazzat!!!!z!!z!zz,ikoaejnioazojine\naijajijiazajizz\nauiahuiauhi\n";
    utils::WriteFile(filename, fileContents);

    std::string cwd = utils::GetCwd();
    utils::CreateDir("dummySubDir/");
    utils::ChangeDirWrapper("dummySubDir/");

    const char *newPath = "../teskkkt.txt";
    const char *argv[] = { "mygit", "add", newPath };
    int argc = sizeof(argv) / sizeof(char *);
    mygit::add(options::AddOptions(argc, const_cast<char **>(argv)));
    //std::cout << mygit::status_str() << '\n';

    remove(newPath);

    /// Verify that filename is in mygit status output
    std::string output = mygit::status_str();
    //std::cout << output << '\n';
    process_test_findstr(output.find("\tDeleted:\t" + std::string(newPath)), "TestStatusOneFileAtRootDeletedWhileInSubdir");

    /// chdir back to origin
    utils::ChangeDirWrapper(cwd);

    /// Remove files
    remove("teskkkt.txt");
    rmdir("dummySubDir/");
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
    mygit::add(options::AddOptions(argc, const_cast<char **>(argv)));

    remove(filename.c_str());

    /// Verify that filename is in mygit status output
    std::string output = mygit::status_str();
    process_test_findstr(output.find("\tDeleted:\t" + filename), "TestStatusOneFileInSubdirDeletedWhileAtRoot");

    /// Remove files
    remove("dummySubDir99/tezzztzz.txt");
    rmdir("dummySubDir99/");
}

void tmp ()
{
    for (int i = 0; i < 100000; i++)
    {

    }
}

int main()
{

    /// Init test env (start)
    std::string pathToRootRepo = init_test();

    TestStatusOneFileAtRoot();

    TestStatusOneFileInSubdirWhileInSubDir(pathToRootRepo);

    TestStatusOneFileInSubdirWhileNotInSubDir();

    TestStatusOneFileAtRootWhileInSubDir(pathToRootRepo);

    TestStatusOneFileAtRootDisappearWhileInSubdir();

    TestStatusOneFileAtRootDisappearWhileAtRoot();

    TestStatusOneFileInSubdirDisappearWhileAtRoot();

    TestStatusOneFileAtRootModifiedWhileAtRoot();

    TestStatusOneFileInSubdirModifiedWhileAtRoot();

    TestStatusOneFileAtRootModifiedWhileInSubdir();

    TestStatusOneFileAtRootDeletedWhileAtRoot();

    TestStatusOneFileAtRootDeletedWhileInSubdir();

    TestStatusOneFileInSubdirDeletedWhileAtRoot();

    /// Clean files (end)
    clean_test(pathToRootRepo);

    return 0;
}
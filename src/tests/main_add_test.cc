#include <cstdlib>

#include "utils/utils.hh"
#include "utils/wrappers.hh"
#include "commands/commands.hh"

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

void TestAddOneFileAtRoot (std::string pathToRootRepo)
{
    std::cout << "TEST 1: 'TestAddOneFileAtRoot'\n";

    /// git add
    const char* filename = "test.txt";
    std::string fileContents = "this is some dummy text!!!!z!!z!zz,ikoaejnioazojine\naijajijiazajizz\nauiahuiauhi\n";
    utils::WriteFile(filename, fileContents);
    const char *argv[] = { "mygit", "add", filename };
    int argc = sizeof(argv) / sizeof(char *);
    mygit::add(options::AddOptions(argc, const_cast<char **>(argv)));

    /// Check that .mygit/index exists
    process_test_bool(utils::IsFileExists(pathToRootRepo + "/.mygit/index"), "index file exists");

    /// Decompress ./mygit/index
    std::string pathToIndex = pathToRootRepo + "/.mygit/index";
    std::string indexDecompressed = utils::DecompressString(utils::ReadFile(pathToIndex));

    /// Verify that filename is in index
    process_test_findstr(indexDecompressed.find(utils::GetPathRelativeToDotMyGit(filename)), "is filename in index");

    /// Get hash-object
    std::string pathFileFromDotMyGit = utils::GetPathRelativeToDotMyGit(filename);
    objects::Blob blob = objects::Blob(pathFileFromDotMyGit, pathToRootRepo + '/' + pathFileFromDotMyGit);
    std::string hash = blob.ToHash();
    std::string blobPath = pathToRootRepo + "/.mygit/objects/" + hash.substr(0, 2) + "/" + hash.substr(2);
    process_test_findstr(indexDecompressed.find(hash), "is hash in index");

    /// Decompress the blob
    std::string blobDecompressed = utils::DecompressString(utils::ReadFile(blobPath));

    /// Check that a blob object was created in .mygit/objects/$hash
    process_test_bool(utils::IsFileExists(blobPath), "blob object exists");
    process_test_findstr(blobDecompressed.find("blob"), "blob object right type");

    /// Verify that the decompressed text is equals to the contents of $filename
    process_test_bool(objects::GetContentBlobDecompressed(blobDecompressed) == fileContents, "decompressed contents is correct");
}

void TestAddOneFileInSubdirWhileInSubDir (std::string pathToRootRepo)
{
    std::cout << "TEST 2: 'TestAddOneFileInSubdirWhileInSubDir'\n";

    std::string cwd = utils::GetCwd();
    utils::CreateDir("dummySubDir/");
    utils::ChangeDirWrapper("dummySubDir/");

    pathToRootRepo = utils::FindPathToRootRepo();

    /// git add
    const char* filename = "hehehehe.txt";
    std::string fileContents = "ezuhizuhie\nziazijh\nnazhzyhz\n";
    utils::WriteFile(filename, fileContents);
    const char *argv[] = { "mygit", "add", filename };
    int argc = sizeof(argv) / sizeof(char *);
    mygit::add(options::AddOptions(argc, const_cast<char **>(argv)));

    /// Check that .mygit/index exists
    process_test_bool(utils::IsFileExists(pathToRootRepo + "/.mygit/index"), "index file exists");

    /// Decompress ./mygit/index
    std::string pathToIndex = pathToRootRepo + "/.mygit/index";
    std::string indexDecompressed = utils::DecompressString(utils::ReadFile(pathToIndex));

    /// Verify that filename is in index
    process_test_findstr(indexDecompressed.find(utils::GetPathRelativeToDotMyGit(filename)), "is filename in index");

    /// Get hash-object
    std::string pathFileFromDotMyGit = utils::GetPathRelativeToDotMyGit(filename);
    objects::Blob blob = objects::Blob(pathFileFromDotMyGit, pathToRootRepo + '/' + pathFileFromDotMyGit);
    std::string hash = blob.ToHash();
    std::string blobPath = pathToRootRepo + "/.mygit/objects/" + hash.substr(0, 2) + "/" + hash.substr(2);
    process_test_findstr(indexDecompressed.find(hash), "is hash in index");

    /// Decompress the blob
    std::string blobDecompressed = utils::DecompressString(utils::ReadFile(blobPath));

    /// Check that a blob object was created in .mygit/objects/$hash
    process_test_bool(utils::IsFileExists(blobPath), "blob object exists");
    process_test_findstr(blobDecompressed.find("blob"), "blob object right type");

    /// Verify that the decompressed text is equals to the contents of $filename
    process_test_bool(objects::GetContentBlobDecompressed(blobDecompressed) == fileContents, "decompressed contents is correct");

    /// chdir back to origin
    utils::ChangeDirWrapper(cwd);
}

void TestAddOneFileInSubdirWhileNotInSubDir (std::string pathToRootRepo)
{
    std::cout << "TEST 3: 'TestAddOneFileInSubdirWhileNotInSubDir'\n";

    utils::CreateDir("dummySubDir/");

    /// git add
    const char* filename = "dummySubDir/bonsoir.txt";
    std::string fileContents = "zjiajhizjiajiojioajoiz\n";
    utils::WriteFile(filename, fileContents);
    const char *argv[] = { "mygit", "add", filename };
    int argc = sizeof(argv) / sizeof(char *);
    mygit::add(options::AddOptions(argc, const_cast<char **>(argv)));

    /// Check that .mygit/index exists
    process_test_bool(utils::IsFileExists(pathToRootRepo + "/.mygit/index"), "index file exists");

    /// Decompress ./mygit/index
    std::string pathToIndex = pathToRootRepo + "/.mygit/index";
    std::string indexDecompressed = utils::DecompressString(utils::ReadFile(pathToIndex));

    /// Verify that filename is in index
    process_test_findstr(indexDecompressed.find(utils::GetPathRelativeToDotMyGit(filename)), "is filename in index");

    /// Get hash-object
    std::string pathFileFromDotMyGit = utils::GetPathRelativeToDotMyGit(filename);
    objects::Blob blob = objects::Blob(pathFileFromDotMyGit, pathToRootRepo + '/' + pathFileFromDotMyGit);
    std::string hash = blob.ToHash();
    std::string blobPath = pathToRootRepo + "/.mygit/objects/" + hash.substr(0, 2) + "/" + hash.substr(2);
    process_test_findstr(indexDecompressed.find(hash), "is hash in index");

    /// Decompress the blob
    std::string blobDecompressed = utils::DecompressString(utils::ReadFile(blobPath));

    /// Check that a blob object was created in .mygit/objects/$hash
    process_test_bool(utils::IsFileExists(blobPath), "blob object exists");
    process_test_findstr(blobDecompressed.find("blob"), "blob object right type");

    /// Verify that the decompressed text is equals to the contents of $filename
    process_test_bool(objects::GetContentBlobDecompressed(blobDecompressed) == fileContents, "decompressed contents is correct");
}

void TestAddMultipleFilesAtRoot (std::string pathToRootRepo)
{
    std::cout << "TEST 4: 'TestAddMultipleFilesAtRoot'\n";

    /// git add
    const char* filename1 = "test1.txt";
    std::string fileContents1 = "this is summykoaejnioazojine\naijajijiazajizz\nauiahuiauhi\n";
    utils::WriteFile(filename1, fileContents1);

    const char* filename2 = "test2.txt";
    std::string fileContents2 = "e dummy !z!!z!zz,ikoaejnione\n\nauiahuiauhi\n";
    utils::WriteFile(filename2, fileContents2);

    const char* filename3 = "test3.txt";
    std::string fileContents3 = "this is sommy text!!!!z!!z!zz,\naijajijiazajizz\n";
    utils::WriteFile(filename3, fileContents3);

    std::map<std::string, std::string> map = {{filename1, fileContents1}, {filename2, fileContents2}, {filename3, fileContents3}};

    const char *argv[] = { "mygit", "add", "." };
    int argc = sizeof(argv) / sizeof(char *);
    mygit::add(options::AddOptions(argc, const_cast<char **>(argv)));

    /// Check that .mygit/index exists
    process_test_bool(utils::IsFileExists(pathToRootRepo + "/.mygit/index"), "index file exists");

    /// Decompress ./mygit/index
    std::string pathToIndex = pathToRootRepo + "/.mygit/index";
    std::string indexDecompressed = utils::DecompressString(utils::ReadFile(pathToIndex));

    /// Verify that filename is in index
    for (const auto& pair : map)
    {
        std::string filename = pair.first;
        std::string fileContents = pair.second;
        process_test_findstr(indexDecompressed.find(utils::GetPathRelativeToDotMyGit(filename)), "is filename in index");
        /// Get hash-object
        std::string pathFileFromDotMyGit = utils::GetPathRelativeToDotMyGit(filename);
        objects::Blob blob = objects::Blob(pathFileFromDotMyGit, pathToRootRepo + '/' + pathFileFromDotMyGit);
        std::string hash = blob.ToHash();
        std::string blobPath = pathToRootRepo + "/.mygit/objects/" + hash.substr(0, 2) + "/" + hash.substr(2);
        process_test_findstr(indexDecompressed.find(hash), "is hash in index");

        /// Decompress the blob
        std::string blobDecompressed = utils::DecompressString(utils::ReadFile(blobPath));

        /// Check that a blob object was created in .mygit/objects/$hash
        process_test_bool(utils::IsFileExists(blobPath), "blob object exists");
        process_test_findstr(blobDecompressed.find("blob"), "blob object right type");

        /// Verify that the decompressed text is equals to the contents of $filename
        process_test_bool(objects::GetContentBlobDecompressed(blobDecompressed) == fileContents, "decompressed contents is correct");
    }
}

void TestAddMultipleFilesInSubdirWhileInSubdir(std::string pathToRootRepo)
{
    std::cout << "TEST 5: 'TestAddMultipleFilesInSubdirWhileInSubdir'\n";

    std::string cwd = utils::GetCwd();
    utils::CreateDir("dummySubDir/");
    utils::ChangeDirWrapper("dummySubDir/");

    pathToRootRepo = utils::FindPathToRootRepo();

    /// git add
    const char* filename1 = "test1.txt";
    std::string fileContents1 = "this is summykoaejnioazojine\naijajijiazajizz\nauiahuiauhi\n";
    utils::WriteFile(filename1, fileContents1);

    const char* filename2 = "test2.txt";
    std::string fileContents2 = "e dummy !z!!z!zz,ikoaejnione\n\nauiahuiauhi\n";
    utils::WriteFile(filename2, fileContents2);

    const char* filename3 = "test3.txt";
    std::string fileContents3 = "this is sommy text!!!!z!!z!zz,\naijajijiazajizz\n";
    utils::WriteFile(filename3, fileContents3);

    std::map<std::string, std::string> map = {{filename1, fileContents1}, {filename2, fileContents2}, {filename3, fileContents3}};

    const char *argv[] = { "mygit", "add", "." };
    int argc = sizeof(argv) / sizeof(char *);
    mygit::add(options::AddOptions(argc, const_cast<char **>(argv)));

    /// Check that .mygit/index exists
    process_test_bool(utils::IsFileExists(pathToRootRepo + "/.mygit/index"), "index file exists");

    /// Decompress ./mygit/index
    std::string pathToIndex = pathToRootRepo + "/.mygit/index";
    std::string indexDecompressed = utils::DecompressString(utils::ReadFile(pathToIndex));

    /// Verify that filename is in index
    for (const auto& pair : map)
    {
        std::string filename = pair.first;
        std::string fileContents = pair.second;
        process_test_findstr(indexDecompressed.find(utils::GetPathRelativeToDotMyGit(filename)), "is filename in index");
        /// Get hash-object
        std::string pathFileFromDotMyGit = utils::GetPathRelativeToDotMyGit(filename);
        objects::Blob blob = objects::Blob(pathFileFromDotMyGit, pathToRootRepo + '/' + pathFileFromDotMyGit);
        std::string hash = blob.ToHash();
        std::string blobPath = pathToRootRepo + "/.mygit/objects/" + hash.substr(0, 2) + "/" + hash.substr(2);
        process_test_findstr(indexDecompressed.find(hash), "is hash in index");

        /// Decompress the blob
        std::string blobDecompressed = utils::DecompressString(utils::ReadFile(blobPath));

        /// Check that a blob object was created in .mygit/objects/$hash
        process_test_bool(utils::IsFileExists(blobPath), "blob object exists");
        process_test_findstr(blobDecompressed.find("blob"), "blob object right type");

        /// Verify that the decompressed text is equals to the contents of $filename
        process_test_bool(objects::GetContentBlobDecompressed(blobDecompressed) == fileContents, "decompressed contents is correct");
    }

    /// chdir back to origin
    utils::ChangeDirWrapper(cwd);
}

int main()
{
    /// Init test env
    std::string pathToRootRepo = init_test();

    TestAddOneFileAtRoot(pathToRootRepo);

    TestAddOneFileInSubdirWhileInSubDir(pathToRootRepo);

    TestAddOneFileInSubdirWhileNotInSubDir(pathToRootRepo);

    /// Clear dummy files
    system("rm -rf *");

    TestAddMultipleFilesAtRoot(pathToRootRepo);

    /// Clear dummy files
    system("rm -rf *");

    TestAddMultipleFilesInSubdirWhileInSubdir(pathToRootRepo);

    /// Clean test
    clean_test(pathToRootRepo);

    return 0;
}
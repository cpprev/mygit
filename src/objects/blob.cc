#include "blob.hh"

namespace objects
{
    Blob::Blob(const std::string& pathFileFromDotMyGit, const std::string& fullPath)
    {
        _pathFileFromDotMyGit = pathFileFromDotMyGit;
        _fullPath = fullPath;
        _contentsFile = utils::ReadFile(fullPath);
    }

    std::string Blob::GetContentsFile() const
    {
        return _contentsFile;
    }

    std::string Blob::ToString() const
    {
        return "blob;" + _pathFileFromDotMyGit + ';' + _contentsFile + ';';
    }

    std::string Blob::ToHash()
    {
        /// The Blob hash is made out of the ToString() result
        std::string blobString = this->ToString();
        const char *string = blobString.c_str();
        unsigned char digest[SHA_DIGEST_LENGTH];

        SHA1((unsigned char*) string, strlen(string), (unsigned char*) &digest);

        char res[SHA_DIGEST_LENGTH * 2 + 1];
        for (int i = 0; i < SHA_DIGEST_LENGTH; i++) {
            sprintf(&res[i*2], "%02x", (unsigned int)digest[i]);
        }
        return res;
    }

    void SetupBlob(const objects::Blob& blob, const std::string& hash, const std::string& pathToDotMyGit)
    {
        /// Store the blob
        std::string blobDirName = pathToDotMyGit + "/.mygit/objects/" + hash.substr(0, 2);
        if (not utils::IsDirExists(blobDirName))
            utils::CreateDir(blobDirName);

        /// Fill in the contents of the blob (compressed file)
        std::string blobFilePath = blobDirName + "/" + hash.substr(2);
        utils::WriteFile(blobFilePath, utils::CompressString(blob.GetContentsFile()));
    }

    std::string CreateBlob (const std::string& pathToDotMyGit, const std::string& pathFileFromDotMyGit)
    {
        /// Create the blob and generate the hash from it
        objects::Blob blob = objects::Blob(pathFileFromDotMyGit, pathToDotMyGit + '/' + pathFileFromDotMyGit);
        std::string hash = blob.ToHash();

        /// Setup blob (directory and file filling)
        SetupBlob(blob, hash, pathToDotMyGit);

        return hash;
    }
}
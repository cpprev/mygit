#include "blob.hh"

namespace objects
{
    Blob::Blob(const std::string& pathFileFromDotMyGit, const std::string& fullPath)
    {
        _pathFileFromDotMyGit = pathFileFromDotMyGit;
        _fullPath = fullPath;
        _contentsFile = utils::ReadFile(fullPath);
    }

    std::string Blob::ToString() const
    {
        return _pathFileFromDotMyGit + ';' + _contentsFile + ';';
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
}
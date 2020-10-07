#pragma once

#include <string>
#include <iostream>
#include <openssl/sha.h>

#include "utils/utils.hh"

namespace objects
{
    class Blob
    {
    public:
        std::string ToString () const;
        std::string ToHash();
        std::string GetContentsFile() const;
        std::string GetPathFromDotMyGit() const;
        Blob(const std::string& pathFileFromDotMyGit, const std::string& fullPath);
    private:
        std::string _pathFileFromDotMyGit;
        std::string _fullPath;
        std::string _contentsFile;
    };

    std::string CreateBlob (const std::string& pathToRootRepo, const std::string& pathFileFromDotMyGit);

    void SetupBlob(const objects::Blob& blob, const std::string& hash, const std::string& pathToRootRepo);

    std::string GetTypeBlobDecompressed (const std::string& input);

    std::string GetContentBlobDecompressed (const std::string& input);
}
#pragma once

#include <string>
#include <iostream>

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
        std::string _contentsFile;
    };

    std::string CreateBlob (const std::string& pathFileFromDotMyGit);

    void SetupBlob(const objects::Blob& blob, const std::string& hash);

    std::string GetTypeBlobDecompressed (const std::string& input);

    std::string GetContentBlobDecompressed (const std::string& input);
}
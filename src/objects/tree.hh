#pragma once

#include <vector>
#include <iostream>
#include <map>
#include <set>
#include <string>

#include "utils/utils.hh"

namespace objects
{
    class Tree
    {
    public:
        Tree() = default;
        Tree (const std::string& type, const std::string& hash, const std::string& filename);
        Tree(std::map<std::string, std::string>& map, std::string curDir, int level);
        void PrintTree (int pad = 0);
        void SetupTrees ();
        void SetupTree();
        std::string ToString() const;
        std::string ToHash();
        static void TreeHashToEntryMapRec (const std::string& treeHash, std::map<std::string, std::string>& entries);
        static std::map<std::string, std::string> TreeHashToEntryMap (const std::string& treeHash);
        static std::string ReadTreeFromHash (const std::string& treeHash);

        std::string _type;
        std::string _filename;
        std::string _treeContents;
        std::string _hash;
        std::vector<Tree> _children;
    };
}
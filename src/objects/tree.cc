#include "tree.hh"

namespace objects
{
    Tree::Tree (const std::string& type, const std::string& hash, const std::string& filename)
    {
        _type = type;
        _hash = hash;
        _filename = filename;
    }

    std::string Tree::ToString() const
    {
        return "tree;" + _treeContents + ';';
    }

    std::string Tree::ToHash()
    {
        /// The tree hash is made out of the ToString() result
        std::string treeString = this->ToString();
        return utils::SHA1_Wrapper(treeString);
    }

    void Tree::SetupTree()
    {
        /// Store the tree
        std::string treeDirName = utils::PathToObjectDir(_hash);
        if (not utils::IsDirExists(treeDirName))
            utils::CreateDir(treeDirName);

        /// Fill in the contents of the blob (compressed file)
        std::string treeFilePath = utils::PathToObjectFile(_hash);

        /// Sets the tree metadata
        std::string tree_data = "type=tree\nsize=" + std::to_string(_treeContents.size()) + "\n\n" + _treeContents;

        utils::WriteFile(treeFilePath, utils::CompressString(tree_data));
    }

    void Tree::SetupTrees ()
    {
        if (_type == "tree")
        {
            std::string contents;
            for (auto sub : _children)
            {
                /// Recurse
                sub.SetupTrees();

                contents += sub._type + " " + sub._hash + " " + sub._filename + "\n";
            }
            _treeContents = contents;
            _hash = this->ToHash();
            this->SetupTree();
            //std::cout << "\n" <<"contents:\n"<< contents << "\n";
        }
    }

    Tree::Tree(std::map<std::string, std::string>& map, std::string curDir, int level)
    {
        _type = "tree";
        //std::cout << "cur dir: " << curDir << "\n";
        //std::cout << "level: " << level << "\n";
        std::set<std::string> dirSeen;
        for (auto pair : map)
        {
            std::string filename = pair.first;
            std::string hash = pair.second;

            //std::cout << "file: " << filename << "\n";
            std::string::size_type curDirPos;
            if (curDir.empty())
                curDirPos = filename.find(curDir);
            else
                curDirPos = filename.find(utils::CleanPath(curDir + "/"));
            if (curDirPos != std::string::npos)
            {
                std::string toLevelSlash;
                size_t i = 0;
                int countSlash = 0;
                while (i < filename.size())
                {
                    if (filename[i] == '/')
                        countSlash++;
                    if (countSlash == level)
                        break;
                    else
                        toLevelSlash += filename[i];
                    i++;
                }

                //std::cout << "retreived: " << toLevelSlash << "\n";
                //std::cout << curDirPos << "\n";

                if (utils::IsDirExists(toLevelSlash) and dirSeen.count(toLevelSlash) == 0)
                {
                    dirSeen.insert(toLevelSlash);
                    Tree child = Tree(map, toLevelSlash, level + 1);
                    _children.emplace_back(child);
                }
                else if (utils::IsFileExists(toLevelSlash))
                {
                    //std::cout << "emplaced BLOB: " << filename << "\n";
                    _children.emplace_back(Tree("blob", hash, filename));
                }
            }
        }
    }

    std::string Tree::ReadTreeFromHash (const std::string& treeHash)
    {
        std::string treePath = utils::PathToObjectFile(treeHash);
        std::string rawContents = utils::ReadFile(treePath);
        utils::ExitIfTrue(rawContents.empty(), "Tree contents should not be empty (TreeHashToEntryMap).");
        return objects::GetContentBlobDecompressed(utils::DecompressString(rawContents));
    }

    std::map<std::string, std::string> Tree::TreeHashToEntryMap(const std::string& treeHash)
    {
        std::map<std::string, std::string> entries;
        TreeHashToEntryMapRec(treeHash, entries);
        return entries;
    }

    void Tree::TreeHashToEntryMapRec(const std::string& treeHash, std::map<std::string, std::string>& entries)
    {
        std::string treeContents = ReadTreeFromHash(treeHash);
        //std::cout << treeContents << '\n';
        std::string type, hash, path;
        int count = 0;
        for (size_t i = 0; i < treeContents.size(); i++)
        {
            if (treeContents[i] == '\n' or i == treeContents.size() - 1)
            {
                if (i == treeContents.size() - 1 and treeContents[i] != '\n')
                    path += treeContents[i];

                if (type == "blob")
                    entries[path] = hash;
                else if (type == "tree")
                    TreeHashToEntryMapRec(hash, entries);

                count = 0;
                type.clear();
                hash.clear();
                path.clear();
            }
            else if (treeContents[i] == ' ')
            {
                count += 1;
            }
            else
            {
                char cur = treeContents[i];
                if (count == 0)
                    type += cur;
                else if (count == 1)
                    hash += cur;
                else if (count == 2)
                    path += cur;
            }
        }
    }

    void Tree::PrintTree (int pad)
    {
        std::cout << std::string(pad, ' ' ) << "CUR TREE\n";
        for (auto sub : _children)
        {
            if (sub._type == "tree")
                sub.PrintTree(pad + 10);
            else
                std::cout << std::string(pad, ' ' ) << sub._type << " \\ " << sub._hash << "\n";
        }
    }
}
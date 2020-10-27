#pragma once

#include <iostream>
#include <string>
#include <map>
#include <list>
#include <vector>

namespace utils
{
    void ExitIfTrue (bool assertion, const std::string& message);

    void ExitProgramWithMessage(const int& status, const std::string& message);

    void SetGlobals ();

    bool DoesRequireRepo (const std::string& command);

    bool DoesRequireOneOrMoreArguments (const std::string& command);

    bool ChangeDirWrapper(const std::string& path);

    void GoToCharAfterNextSlash (const std::string& path, size_t& i);

    void AddDiffCharacterBeforeLine (std::string& input, bool plus);

    std::string StripPaddingSpaces (const std::string& str);

    std::string GetDateIntValue ();

    std::string GetDateCommitFormat ();

    void LessCommandWrapper (const std::string& contents);

    std::string GetDate ();

    void AddToNElementsList(const std::string& elm, std::list<std::string>& vect, size_t maxSize);

    std::vector<std::string> GetLinesAsVect (const std::string& input);

    std::vector<std::string> ListBranches ();

    void DisplayBranches ();

    std::string GetCurrentBranch ();

    bool IsCommitAncestorOf (const std::string& c1, const std::string& c2);

    bool CheckRepoHasOneCommit ();

    void UpdateWorkingDirectoryAndIndex (const std::string& hashCommitCurrent, const std::string& hashCommitToCopy);

    bool CheckBranchExists (const std::string& branchName);

    bool IsAlreadyOnCommit (const std::string& commit);

    bool IsCommitObject (const std::string& commitHash);

    std::string GetCommitHash (const std::string& commitOrBranch);

    std::string GetTreeHashFromCommit (const std::string& commitHash);

    std::string GetMostRecentCommit ();

    std::string GetMostRecentCommit (const std::string& headContents);

    std::string GetCommonAncestor (const std::string& c1, const std::string& c2);

    std::string ReadBranchPathInHead ();

    std::string ReadBranchPathInHead (const std::string& headContents);

    std::string GetCwd ();

    void GetDiffBetweenTrees (const std::map<std::string, std::string>& tree1Entries, const std::map<std::string, std::string>& tree2Entries,
                              std::map<std::string, std::string>& inTree1, std::map<std::string, std::string>& inTree1Status);
}
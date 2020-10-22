#pragma once

#include <iostream>
#include <string>
#include <list>
#include <vector>

namespace utils
{
    void ExitIfTrue (bool assertion, const std::string& message);

    void ExitProgramWithMessage(const int& status, const std::string& message);

    void SetGlobalVariables ();

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

    bool CheckRepoHasOneCommit ();

    bool CheckBranchExists (const std::string& branchName);

    bool IsAlreadyOnCommit (const std::string& commit);

    bool IsCommitObject (const std::string& commitHash);

    std::string GetCommitHash (const std::string& commitOrBranch);

    std::string GetTreeHashFromCommit (const std::string& commitHash);

    std::string GetMostRecentCommit ();

    std::string GetMostRecentCommit (const std::string& headContents);

    std::string ReadBranchPathInHead ();

    std::string ReadBranchPathInHead (const std::string& headContents);

    std::string GetCwd ();
}
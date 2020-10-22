#pragma once

#include <iostream>
#include <string>

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
}
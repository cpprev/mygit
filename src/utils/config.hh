#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <map>

namespace utils
{
    class Config
    {
    public:
        Config();
        void ParseConfig(bool local);
        void UpdateConfig(bool local);
        std::string GetPathToConfigFile(bool local);
        std::string ReadConfigFile (bool local);
        void AddVariable (bool local, const std::pair<std::string, std::string>& kvp);
        void UnsetVariable (bool local, const std::string& value);
        std::string GetVariable (bool local, const std::string& value);
        std::string GetVariable (const std::string& value);

    private:
        std::map<std::string, std::map<std::string, std::string>> _values;
    };
}

extern utils::Config g_Config;
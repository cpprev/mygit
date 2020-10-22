#include "utils/config.hh"
#include "utils/wrappers.hh"
#include "utils/get_paths.hh"
#include "utils/utils.hh"

namespace utils
{
    Config::Config()
    {
    }

    std::string Config::GetPathToConfigFile(bool local)
    {
        return local ? utils::PathToLocalConfig()
                     : utils::PathToGlobalConfig();
    }

    std::string Config::ReadConfigFile (bool local)
    {
        std::string path = GetPathToConfigFile(local);
        return utils::ReadFile(path);
    }

    void Config::ParseConfig(bool local)
    {
        std::string contents = ReadConfigFile(local);

        std::map<std::string, std::string> dummy;
        std::string variable, key, value;
        bool hitEquals = false;
        for (size_t i = 0; i < contents.size(); i++)
        {
            if (contents[i] == '[' or i == contents.size() - 1)
            {
                if (i == contents.size() - 1 and contents[i] != '\n' and contents[i] != ' ' and contents[i] != '\t')
                    value += contents[i];
                if (not key.empty())
                {
                    dummy[key] = StripPaddingSpaces(value);
                    hitEquals = false;
                    key.clear();
                    value.clear();
                }
                if (not variable.empty())
                {
                    _values[variable] = dummy;
                    variable.clear();
                    dummy.clear();
                }

                i += 1;
                while (i < contents.size() and contents[i] != ']')
                    variable += contents[i++];
            }
            else if (contents[i] == '\n')
            {
                if (not key.empty())
                {
                    dummy[key] = StripPaddingSpaces(value);
                    hitEquals = false;
                    key.clear();
                    value.clear();
                }
            }
            else if (not hitEquals and (contents[i] == ' ' or contents[i] == '\t'))
                continue;
            else if (contents[i] == '=')
                hitEquals = true;
            else
            {
                if (not hitEquals)
                    key += contents[i];
                else
                    value += contents[i];
            }
        }
    }

    void Config::UpdateConfig(bool local)
    {
        std::string path = GetPathToConfigFile(local);
        std::string contents = utils::ReadFile(path);

        std::string output;
        for (auto var : _values)
        {
            output += "[" + var.first + "]\n";
            for (auto pair : var.second)
            {
                output += "\t" + pair.first + " = " + pair.second + "\n";
            }
        }
        WriteFile(path, output);
    }

    void Config::AddVariable (bool local, const std::pair<std::string, std::string>& kvp)
    {
        /// Parse
        ParseConfig(local);

        auto ind = kvp.first.find(".");
        ExitIfTrue(ind == std::string::npos, "Wrong format of given value : expecting \"variable.key value\"");
        std::string key1 = kvp.first.substr(0, ind);
        std::string key2 = kvp.first.substr(ind + 1);
        std::string val = kvp.second;

        /// Add requested values
        _values[key1][key2] = val;

        /// Update config
        UpdateConfig(local);
    }

    void Config::UnsetVariable (bool local, const std::string& value)
    {
        /// Parse
        ParseConfig(local);

        auto ind = value.find(".");
        /// Value in key dot value format ("hello.world")
        if (ind != std::string::npos)
        {
            std::string key1 = value.substr(0, ind);
            std::string key2 = value.substr(ind + 1);
            _values[key1].erase(key2);
        }
        /// Value in reg string format ("hello")
        else
        {
            _values.erase(value);
        }

        /// Update config
        UpdateConfig(local);
    }

    std::string Config::GetVariable (bool local, const std::string& value)
    {
        /// Parse
        ParseConfig(local);

        auto ind = value.find(".");
        ExitIfTrue(ind == std::string::npos, "Wrong format of given value : expecting \"variable.key value\"");
        std::string key1 = value.substr(0, ind);
        std::string key2 = value.substr(ind + 1);

        return _values[key1][key2];
    }

    std::string Config::GetVariable (const std::string& value)
    {
        /// Check in local config file (.mygit/config)
        std::string res1 = GetVariable(true, value);
        if (not res1.empty())
            return res1;
        /// Check in global file (~/.mygitconfig)
        std::string res2 = GetVariable(false, value);
        return res2;
    }
}
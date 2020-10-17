#include "commands.hh"
#include "utils/config.hh"

namespace mygit
{
    void config (const options::ConfigOptions& opt)
    {
        /// mygit config --add case
        if (opt.add.has_value())
        {
            g_Config.AddVariable(opt.local, opt.add.value());
        }
        /// mygit config --unset case
        else if (not opt.unset.empty())
        {
            g_Config.UnsetVariable(opt.local, opt.unset);
        }
        /// mygit config --get case
        else if (not opt.get.empty())
        {
            std::cout << g_Config.GetVariable(opt.local, opt.get) << '\n';
        }
    }
}
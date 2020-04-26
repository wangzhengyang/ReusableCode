#include "config.h"


int main(void)
{
    std::fstream fs;
    ConfigFile::OpenConfigFile(fs, "./param.config");
    ConfigFile::ReadConfigParam(fs, configparam, PARAMLISTS);
    ConfigFile::CloseConfigPram(fs);
    ConfigFile::PrintConfigParam(configparam, PARAMLISTS); 
}

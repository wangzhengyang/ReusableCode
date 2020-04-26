#ifndef _CONFIG_H
#define _CONFIG_H

#include <string>
#include <fstream>
#include <unistd.h>
#include <iostream>

struct ConfigParam{
    std::string key;
    std::string value;
};

#define PARAMLISTS      3

class ConfigFile{
public:
    static int OpenConfigFile(std::fstream &fs, const char *filename);
    static int ReadConfigParam(std::fstream &fs, struct ConfigParam *configparam, int num);
    static int CloseConfigPram(std::fstream &fs);
    static int PrintConfigParam(struct ConfigParam *configparam, int num);

    
};

extern struct ConfigParam configparam[];



#endif

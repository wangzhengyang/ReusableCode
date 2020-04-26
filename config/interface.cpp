#include "config.h"


int ConfigFile::OpenConfigFile(std::fstream &fs, const char *filename)
{
    fs.open(filename, std::fstream::in);
    if(!fs.is_open()){
        /*TODO*/
        return -1;
    }
}

int ConfigFile::ReadConfigParam(std::fstream &fs, struct ConfigParam *config, int num)
{
    char linetemp[1000] = { 0 };
    int i = 0;
    std::string line;
    while(!fs.eof()){
        fs.getline(linetemp, 1000);
        line.clear();
        line = linetemp;
        size_t pos = line.find('=');
        if(pos == std::string::npos) return -1;
        std::string tempKey = line.substr(0, pos);
        for(i = 0; i < num; i++){
            if(config[i].key == tempKey){
                config[i].value = line.substr(pos+1);
                break;
            }
        }
    }
}

int ConfigFile::CloseConfigPram(std::fstream &fs)
{
    fs.close();
}

int ConfigFile::PrintConfigParam(struct ConfigParam *param, int num)
{
    int i;
    for(i = 0; i < num; i++){
        std::cout << "key:" << param[i].key << " value:" << param[i].value << std::endl;
    }
}

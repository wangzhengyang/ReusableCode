#ifndef MODBUS_RULE_H
#define MODBUS_RULE_H

#include <vector>

struct Modbus_Rule
{
    unsigned char m_funcode = 0; //for rule
    unsigned short m_address = 0;
    unsigned short m_regnum = 0;

    unsigned short m_interval = 0; //for timer
    unsigned short m_current = 0;

    unsigned short *m_pdata = nullptr; //for data pointer
    bool m_update_oneormore = false;
    unsigned short m_setting = 0; //setting
    std::vector<unsigned short> m_settings;
    unsigned short m_settingoffset = 0; //settingoffset from startaddress
};

#endif
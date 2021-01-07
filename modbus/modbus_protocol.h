#ifndef MODBUS_PROTOCOL_H
#define MODBUS_PROTOCOL_H

#include <string>
#include <iostream>
#include <iomanip>
#include <vector>

class Modbus_Protocol
{
public:
    Modbus_Protocol() {}
    virtual ~Modbus_Protocol() {}

public:
    virtual int Generate(std::vector<unsigned char> &data) = 0;
    virtual int Parse(std::vector<unsigned char> &data) = 0;
};

#endif
#ifndef MODBUS_RTU_H
#define MODBUS_RTU_H

#include "modbus_protocol.h"

class Modbus_RTU final : public Modbus_Protocol
{
public:
    Modbus_RTU() {}
    ~Modbus_RTU() {}
    int Generate(std::vector<unsigned char> &data) override;
    int Parse(std::vector<unsigned char> &data) override;

private:
    unsigned short crc16(const std::vector<unsigned char> &data);
};

#endif
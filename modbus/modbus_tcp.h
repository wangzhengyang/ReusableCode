#ifndef MODBUS_TCP_H
#define MODBUS_TCP_H

#include "modbus_protocol.h"

class Modbus_TCP final : public Modbus_Protocol
{
public:
    Modbus_TCP(unsigned short count = 1) : m_count(count) {}
    ~Modbus_TCP() {}
    int Generate(std::vector<unsigned char> &str) override;
    int Parse(std::vector<unsigned char> &str) override;

private:
    unsigned short m_count;
};

#endif
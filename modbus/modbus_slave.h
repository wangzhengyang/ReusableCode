#ifndef MODBUS_SLAVE_H
#define MODBUS_SLAVE_H

#include "modbus_base.h"

class Modbus_Slave final : public Modbus_Base
{
public:
    Modbus_Slave(unsigned char address) : Modbus_Base(address) {}
    ~Modbus_Slave() {}

public:
    virtual int Send(unsigned char *pbuf, unsigned short &len) override;
    virtual int Recv(unsigned char *pbuf, unsigned short len) override;

private:
    int Analysis();
    int Analysis0x03_04(unsigned short funcode, unsigned short address, unsigned short regnum);
    int Analysis0x06(unsigned short address, unsigned short data);
    int Analysis0x10(unsigned short address, unsigned short regnum, std::vector<unsigned short> &data);
private:
    std::queue<std::vector<unsigned char>> m_recvqueue;
    std::queue<std::vector<unsigned char>> m_sendqueue;
};

#endif
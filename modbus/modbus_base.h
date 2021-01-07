#ifndef MODBUS_BASE_H
#define MODBUS_BASE_H

#include <vector>
#include <string>
#include <iostream>
#include <map>
#include <iomanip>
#include <memory>
#include <queue>
#include <cstring>
#include <initializer_list>

#include "modbus_registers.h"
#include "modbusprotocol_factory.h"
#include "modbus_rule.h"

class Modbus_Base
{
public:
    typedef int (*Timer)();

public:
    Modbus_Base(unsigned char address, Timer t = nullptr) : m_address(address), m_timer(t) {}
    virtual ~Modbus_Base() {}
    Modbus_Base(Modbus_Base &) = delete;
    Modbus_Base &operator=(const Modbus_Base &) = delete;

    void Address(unsigned char address) { m_address = address; }
    unsigned char Address() const { return m_address; }
    void SetTime(Timer timer) { m_timer = timer; }
    void Mount_RegisterMap(std::shared_ptr<Modbus_Registers> reg) { m_registers = reg;}
    void Mount_Protocol(std::shared_ptr<Modbus_Protocol> pro) { m_protocol = pro; }
    int GetData(unsigned char funcode, unsigned short address, unsigned short &data);
    int GetData(unsigned char funcode, unsigned short address, short &data);
    int GetData(unsigned char funcode, unsigned short address, unsigned int &data);
    int GetData(unsigned char funcode, unsigned short address, int &data);
    int GetData(unsigned char funcode, unsigned short address, unsigned short regnum, std::vector<unsigned short> &data);
public:
    virtual int Send(unsigned char *pbuf, unsigned short &len) = 0;
    virtual int Recv(unsigned char *pbuf, unsigned short len) = 0;
    virtual void Add_Rules(unsigned char funcode, unsigned short startaddr, unsigned short regnum = 1, unsigned short interval = 0) {}
    virtual void Add_Rules(unsigned char funcode, unsigned short startaddr, unsigned short regnum, std::initializer_list<unsigned short> init){}
    virtual int UpdateData(unsigned short address, unsigned short data); //just for 4x registers
    virtual int UpdateData(unsigned short address, short data);
    virtual int UpdateData(unsigned short address, unsigned short regnum, std::initializer_list<unsigned short> init);

protected:
    std::shared_ptr<Modbus_Registers> RegistersMap() const { return m_registers; }
    std::shared_ptr<Modbus_Protocol> Protocol() const { return m_protocol; }
    int DataPointer(unsigned char funcode, unsigned short address, unsigned short regnum, unsigned short **p);
    int UpdateTime();

private:
    std::shared_ptr<Modbus_Registers> m_registers;
    std::shared_ptr<Modbus_Protocol> m_protocol;
    //std::vector<Modbus_Rule> m_rules;
    unsigned char m_address;
    Timer m_timer; //t
};

#endif
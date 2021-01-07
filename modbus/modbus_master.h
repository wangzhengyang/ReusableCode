#ifndef MODBUS_MASTER_H
#define MODBUS_MASTER_H

#include "modbus_base.h"

class Modbus_Master final : public Modbus_Base
{
public:
    Modbus_Master(unsigned char address) : Modbus_Base(address) {}
    ~Modbus_Master() {}

public:
    void Add_Rules(unsigned char funcode, unsigned short startaddr, unsigned short regnum , unsigned short interval = 0) override;
    void Add_Rules(unsigned char funcode, unsigned short startaddr, unsigned short regnum , std::initializer_list<unsigned short> init) override;
    int Send(unsigned char *pbuf, unsigned short &len) override;
    int Recv(unsigned char *pbuf, unsigned short len) override;
    int UpdateData(unsigned short address, unsigned short data) override;
    int UpdateData(unsigned short address, short data) override;
    int UpdateData(unsigned short address, unsigned short regnum, std::initializer_list<unsigned short> init) override;
private:
    int NextRule(Modbus_Rule &rule);
    int RuleToMsg(const Modbus_Rule &rule, std::vector<unsigned char> &data);
    int Rule0x03_04ToMsg(const Modbus_Rule &rule, std::vector<unsigned char> &data); //for 4x registers
    int Rule0x06ToMsg(const Modbus_Rule &rule, std::vector<unsigned char> &data);    //for 4x registers
    int Rule0x10ToMsg(const Modbus_Rule &rule, std::vector<unsigned char> &data);    //for 3x registers
    int UpdateData(const Modbus_Rule &rule, const std::vector<unsigned char> &data);
    int Register_Rule(unsigned char funcode, unsigned short startaddr, unsigned short regnum, unsigned short interval);
    int Register_Rule(unsigned char funcode, unsigned short startaddr, unsigned short regnum, std::initializer_list<unsigned short> &init);
private:
    std::vector<Modbus_Rule> m_rules;
    std::queue<Modbus_Rule> m_rulequeue;
};

#endif
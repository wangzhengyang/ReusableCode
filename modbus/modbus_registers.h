#ifndef MODBUS_REGISTERS_H
#define MODBUS_REGISTERS_H

#include <vector>
#include <iostream>
#include <iomanip>
#include <memory>

struct Registers
{
    Registers() = default;
    unsigned short m_startaddress;
    unsigned short m_endaddress;
    std::vector<unsigned short> m_placeholders;
};

class Modbus_Registers
{
public:
    static std::shared_ptr<Modbus_Registers> Build();
    ~Modbus_Registers() {}
    Modbus_Registers(const Modbus_Registers &) = delete;
    Modbus_Registers &operator=(const Modbus_Registers &) = delete;

public:
    int Add_RegisterPlaceHolder(unsigned short funcode, unsigned short startaddress, unsigned short registernum);
    unsigned short *Retrun_RegisterPlaceHolder(unsigned short funcode, unsigned short address, unsigned short regnum);

private:
    Modbus_Registers() {}
    int Add_PlaceHolder(std::vector<Registers> &reg, unsigned short startaddress, unsigned short registernum);
    int Return_PlaceHolder(std::vector<Registers> &reg, unsigned short address, unsigned short regnum, unsigned short **data);

private:
    std::vector<Registers> m_3xRegisters;
    std::vector<Registers> m_4xRegisters;
};

#endif
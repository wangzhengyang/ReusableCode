#include "modbus_registers.h"

using namespace std;

int Modbus_Registers::Add_PlaceHolder(vector<Registers> &reg, unsigned short startaddress, unsigned short registernum)
{
    unsigned short endaddress = startaddress + registernum - 1;
    for (auto iter = reg.cbegin(); iter != reg.cend(); ++iter)
    {
        //cout << "startaddress:" << (int)(startaddress) << " endaddress:" << (int)(endaddress) << "iter->startaddress:" << (int)(iter->m_startaddress) << "iter->endaddress:" << (int)(iter->m_endaddress) << endl;
        if (startaddress <= iter->m_endaddress && endaddress >= iter->m_startaddress)
        {
            return -1;
        }
    }
    //can insert
    Registers temp;
    temp.m_startaddress = startaddress;
    temp.m_endaddress = endaddress;
    vector<unsigned short> temp1(registernum, 0);
    temp.m_placeholders.swap(temp1);
    reg.push_back(std::move(temp));
    //reg.push_back(temp);
    //cout << "regmap add success! size:" << (int)(reg.size()) << endl;
    return 0;
}

int Modbus_Registers::Add_RegisterPlaceHolder(unsigned short funcode, unsigned short startaddress, unsigned short registernum)
{
    if (funcode == 0x03)
    {
        if (Add_PlaceHolder(m_4xRegisters, startaddress, registernum) < 0)
        {
            cout << "funcode:" << (int)(funcode) << " startaddress:" << (int)(startaddress) << " registernum:" << (int)(registernum) << " insert error!" << endl;
            return -1;
        }
    }
    else if (funcode == 0x04)
    {
        if (Add_PlaceHolder(m_3xRegisters, startaddress, registernum) < 0)
        {
            cout << "funcode:" << (int)(funcode) << " startaddress:" << (int)(startaddress) << " registernum:" << (int)(registernum) << " insert error!" << endl;
            return -1;
        }
    }
    else
    {
        cout << "funcode:" << (int)(funcode) << " not existed!" << endl;
    }
    return 0;
}

int Modbus_Registers::Return_PlaceHolder(vector<Registers> &reg, unsigned short address, unsigned short regnum, unsigned short **data)
{
    auto iter = reg.cbegin();
    for (; iter != reg.cend(); ++iter)
    {
        if (address >= iter->m_startaddress && address <= iter->m_endaddress)
        {
            break;
        }
    }

    if (iter == reg.cend())
    {
        cout << "regmap not find address!" << endl;
        return -1;
    }

    if((iter->m_endaddress - address + 1) < regnum)
    {
        cout << "regmap regnum is larger!" << endl;
        return -1;
    }

    *data = const_cast<unsigned short *>(&(iter->m_placeholders[address - iter->m_startaddress]));
    return 0;
}

unsigned short *Modbus_Registers::Retrun_RegisterPlaceHolder(unsigned short funcode, unsigned short address, unsigned short regnum)
{
    unsigned short *p = nullptr;
    if (funcode == 0x03 || funcode == 0x06 || funcode == 0x10)
    {
        if (Return_PlaceHolder(m_4xRegisters, address, regnum, &p) < 0)
        {
            cout << "funcode:" << (int)(funcode) << " address:" << (int)(address) << " not existed" << endl;
            return nullptr;
        }
    }
    else if (funcode == 0x04)
    {
        if (Return_PlaceHolder(m_3xRegisters, address, regnum, &p) < 0)
        {
            cout << "funcode:" << (int)(funcode) << " address:" << (int)(address) << " not existed" << endl;
            return nullptr;
        }
    }
    else
    {
        cout << "funcode:" << (int)(funcode) << " not existed!" << endl;
    }
    return p;
}

shared_ptr<Modbus_Registers> Modbus_Registers::Build()
{
    return shared_ptr<Modbus_Registers>(new Modbus_Registers());
}
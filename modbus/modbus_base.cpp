#include "modbus_base.h"

using namespace std;

int Modbus_Base::UpdateData(unsigned short address, unsigned short data)
{
    auto ptr = RegistersMap();

    if(!ptr)
    {
        return -1;
    }

    unsigned short *pdata = ptr->Retrun_RegisterPlaceHolder(0x04, address, 1);

    if (pdata == nullptr)
    {
        cout << "updatedata address not find!" << endl;
        return -1;
    }

    *pdata = data;
    return 0;
}

int Modbus_Base::UpdateData(unsigned short address, short data)
{
    return (UpdateData(address, (unsigned short)(data)));
}

int Modbus_Base::UpdateData(unsigned short address, unsigned short regnum, std::initializer_list<unsigned short> init)
{
    auto ptr = RegistersMap();

    if(!ptr)
    {
        return -1;
    }

    unsigned short *pdata = ptr->Retrun_RegisterPlaceHolder(0x03, address, regnum);

    if(pdata == nullptr){
        cout << "updatedata address regnum not find!" << endl;
        return -1;
    }

    for(int i = 0; i < regnum; i++)
    {
        *(pdata + i) = *(init.begin() + i);
    }
    return 0;
}

int Modbus_Base::UpdateTime()
{
    if (m_timer == nullptr)
    {
        return -1;
    }

    return m_timer();
}

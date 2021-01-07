#include "modbus_base.h"

using namespace std;
    
int Modbus_Base::DataPointer(unsigned char funcode, unsigned short address, unsigned short regnum, unsigned short **p)
{
    *p = nullptr;

    auto ptr = RegistersMap();

    if(!ptr){
        return -1;
    }

    *p = ptr->Retrun_RegisterPlaceHolder(funcode, address, regnum); 

    if(*p == nullptr)
    {
        cout << "data pointer not find!" << endl;
        return -1;
    }
    return 0;
}

int Modbus_Base::UpdateData(unsigned short address, unsigned short data)
{
    unsigned short *p = nullptr;

    if(DataPointer(0x03, address, 1, &p) < 0)
    {
        return -1;
    }
    
    *p = data;
    return 0;
}

int Modbus_Base::UpdateData(unsigned short address, short data)
{
    return (UpdateData(address, (unsigned short)(data)));
}

int Modbus_Base::UpdateData(unsigned short address, unsigned short regnum, std::initializer_list<unsigned short> init)
{
    unsigned short *p = nullptr;

    if(DataPointer(0x03, address, regnum, &p) < 0)
    {
        return -1;
    }
    
    for(int i = 0; i < regnum; i++)
    {
        *(p + i) = *(init.begin() + i);
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

int Modbus_Base::GetData(unsigned char funcode, unsigned short address, unsigned short &data)
{
    data = 0; 
    unsigned short *p = nullptr;

    if(DataPointer(funcode, address, 1, &p) < 0)
    {
        return -1;
    }

    data = *p;
    return 0;
}

int Modbus_Base::GetData(unsigned char funcode, unsigned short address, short &data)
{
    data = 0;
    unsigned short *p = nullptr;
    if(DataPointer(funcode, address, 1, &p) < 0)
    {
        return -1;
    }

    data = (short)(*p);
    return 0;
}

int Modbus_Base::GetData(unsigned char funcode, unsigned short address, unsigned int &data)
{
    data = 0;
    unsigned short *p = nullptr;
    if(DataPointer(funcode, address, 2, &p) < 0)
    {
        return -1;
    }

    data = (unsigned int)(((*p) << 8) | *(p + 1));
    return 0;
}

int Modbus_Base::GetData(unsigned char funcode, unsigned short address, int &data)
{
    data = 0;
    unsigned short *p = nullptr;
    if(DataPointer(funcode, address, 2, &p) < 0)
    {
        return -1;
    }

    data = (int)(((*p) << 8) | *(p + 1));
    return 0;
}

int Modbus_Base::GetData(unsigned char funcode, unsigned short address, unsigned short regnum, std::vector<unsigned short> &data)
{ 
    data.clear();
    unsigned short *p = nullptr;
    if(DataPointer(funcode, address, regnum, &p) < 0)
    {
        return -1;
    }

    for(int i = 0; i < regnum; i++)
    {
        data.push_back(*(p + i));
    }
    return 0;
}
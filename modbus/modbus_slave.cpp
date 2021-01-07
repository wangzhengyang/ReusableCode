#include "modbus_slave.h"

using namespace std;

int Modbus_Slave::Send(unsigned char *pbuf, unsigned short &len)
{
    //todo analysis todo other operation
    Analysis();
    if (m_sendqueue.empty())
    {
        cout << "modbusslave sendqueue is empty!" << endl;
        return -1;
    }

    vector<unsigned char> data = m_sendqueue.front();
    auto pro = Modbus_Base::Protocol();
    pro->Generate(data);

    memcpy(pbuf, &(data[0]), data.size());
    len = data.size();
    m_sendqueue.pop();
    return 0;
}

int Modbus_Slave::Analysis0x03_04(unsigned short funcode, unsigned short address, unsigned short regnum)
{
    auto pmap = Modbus_Base::RegistersMap();
    if(!pmap){
        return -1;
    }

    if(regnum > 127){
        return -1;
    }

    auto ptr = pmap->Retrun_RegisterPlaceHolder(funcode, address, regnum);
    if(ptr == nullptr){
        return -1;
    }

    vector<unsigned char> send_data;
    send_data.push_back(Modbus_Base::Address());
    send_data.push_back(funcode);
    send_data.push_back(regnum * 2);
    unsigned short temp;
    for(int i = 0; i < regnum; i++)
    {
        temp = *(ptr + i);
        send_data.push_back((temp >> 8) & 0xff);
        send_data.push_back(temp & 0xff);
    }

    m_sendqueue.push(send_data);
    return 0;
}

int Modbus_Slave::Analysis0x06(unsigned short address, unsigned short data)
{
    auto pmap = Modbus_Base::RegistersMap();
    if(!pmap){
        return -1;
    }

    auto ptr = pmap->Retrun_RegisterPlaceHolder(0x03, address, 1);
    if(ptr == nullptr){
        return -1;
    }
    //todo check data if valid
    *ptr = data;
    vector<unsigned char> send_data;
    send_data.push_back(Modbus_Base::Address());
    send_data.push_back(0x06);
    send_data.push_back((address >> 8) & 0xff);
    send_data.push_back(address & 0xff);
    send_data.push_back((data >> 8) & 0xff);
    send_data.push_back(data & 0xff);

    m_sendqueue.push(send_data);
    return 0;
}

int Modbus_Slave::Analysis0x10(unsigned short address, unsigned short regnum, std::vector<unsigned short> &data)
{
    auto pmap = Modbus_Base::RegistersMap();
    if(!pmap){
        return -1;
    }

    if(regnum > 127){
        return -1;
    }

    auto ptr = pmap->Retrun_RegisterPlaceHolder(0x03, address, regnum);
    if(ptr == nullptr){
        return -1;
    }

    for(int i = 0; i < regnum; i++)
    {
        *(ptr + i) = data[i];
    }

    vector<unsigned char> send_data;
    send_data.push_back(Modbus_Base::Address());
    send_data.push_back(0x10);
    send_data.push_back((address >> 8) & 0xff);
    send_data.push_back(address & 0xff);
    send_data.push_back((regnum >> 8) & 0xff);
    send_data.push_back(regnum & 0xff);

    m_sendqueue.push(send_data);
    return 0;

}

int Modbus_Slave::Analysis()
{
    vector<unsigned char> data = m_recvqueue.front();
    m_recvqueue.pop();

    unsigned char recv_address = data[0];
    if (recv_address != Modbus_Base::Address())
    {
        return -1;
    }

    unsigned char recv_funcode = data[1];
    if (recv_funcode == 0x03 || recv_funcode == 0x04)
    {
        unsigned short recv_startaddress = (unsigned short)((data[2] << 8) | data[3]);
        unsigned short recv_regnum = (unsigned short)((data[4] << 8) | data[5]);
        Analysis0x03_04(recv_funcode, recv_startaddress, recv_regnum);
        //cout << "analysis 0x03_04!" << endl;
    }
    else if (recv_funcode == 0x06)
    {
        unsigned short recv_startaddress = (unsigned short)((data[2] << 8) | data[3]);
        unsigned short recv_data = (unsigned short)((data[4] << 8) | data[5]);
        Analysis0x06(recv_startaddress, recv_data);
    }
    else if (recv_funcode == 0x10)
    {
#if 0
        for(int i = 0; i < data.size(); i++)
        {
            cout << (int)data[i] << " ";
        }
        cout << endl;
#endif
        unsigned short recv_startaddress = (unsigned short)((data[2] << 8) | data[3]);
        unsigned short recv_regnum = (unsigned short)((data[4] << 8) | data[5]);
        vector<unsigned short> recv_data2;
        for (int i = 0; i < recv_regnum; i++)
        {
            recv_data2.push_back((unsigned short)((data[6 + (i << 1)] << 8) | data[7 + (i << 1)]));
        }
        Analysis0x10(recv_startaddress, recv_regnum, recv_data2);
    }

    return 0;
}

int Modbus_Slave::Recv(unsigned char *pbuf, unsigned short len)
{
    vector<unsigned char> data;
    for (int i = 0; i < len; i++)
    {
        data.push_back(*(pbuf + i));
    }

    shared_ptr<Modbus_Protocol> pro = Modbus_Base::Protocol();
    if (pro->Parse(data) < 0)
    {
        cout << "recv data check failed!" << endl;
        return -1;
    }

    m_recvqueue.push(data);
    return 0;
}
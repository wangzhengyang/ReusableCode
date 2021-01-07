#include "modbus_tcp.h"

using namespace std;

int Modbus_TCP::Generate(vector<unsigned char> &data)
{
    vector<unsigned char> temp;
    temp.push_back((m_count >> 8) & 0xff); //communication count
    temp.push_back(m_count & 0xff);

    temp.push_back(0x00); // protocol flag 0x0000 means modbustcp
    temp.push_back(0x00);

    temp.push_back((data.size() >> 8) & 0xff);
    temp.push_back(data.size() & 0xff);

    for (int i = 0; i < data.size(); ++i)
    {
        temp.push_back(data[i]);
    }
    data.swap(temp);
    m_count++;
    return 0;
}

int Modbus_TCP::Parse(vector<unsigned char> &data)
{
    unsigned short protocol_flag = (unsigned short)((data[2] << 8) | data[3]);

    if (protocol_flag != 0x0000)
    {
        cout << "modbustcp protocol flag check failed!" << endl;
        return -1;
    }

    unsigned short recv_len = (unsigned short)((data[4] << 8) | data[5]);
    if (recv_len != (data.size() - 6))
    {
        cout << "modbustcp recv_len check failed!" << endl;
        return -1;
    }

    vector<unsigned char> temp(data.cbegin() + 6, data.cend());
    data.swap(temp);
    return 0;
}
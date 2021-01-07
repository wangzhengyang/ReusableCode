#include "modbus_rtu.h"

using namespace std;

unsigned short Modbus_RTU::crc16(const vector<unsigned char> &data)
{
    unsigned short crc = 0xffff;
    for (int i = 0; i < data.size(); i++)
    {
        crc ^= data[i];
        for (int j = 0; j < 8; j++)
        {
            if (crc & 0x01)
            {
                crc >>= 1;
                crc ^= 0xA001;
            }
            else
            {
                crc >>= 1;
            }
        }
    }
    return crc;
}

int Modbus_RTU::Generate(vector<unsigned char> &data)
{
    unsigned short crc = crc16(data);
    //cout << "crc:" << (int)(crc) << endl;
    data.push_back((crc & 0xff));
    data.push_back((crc >> 8) & 0xff);
    return 0;
}

int Modbus_RTU::Parse(vector<unsigned char> &data)
{
    unsigned short recv_crc = (unsigned short)((data[data.size() - 1] << 8) | data[data.size() - 2]);
    //cout << "recv_crc:" << (int)(recv_crc) << endl;
    data.resize(data.size() - 2);
    unsigned short cal_crc = crc16(data);
    //cout << "cal_crc:" << (int)(cal_crc) << endl;
    if (recv_crc != cal_crc)
    {
        cout << "crc16 check failed!" << endl;
        return -1;
    }
    return 0;
}

#include "modbus_registers.h"
#include "modbusprotocol_factory.h"
#include "modbus_factory.h"

using namespace std;

int SetTime()
{
    return 50;
}

int main()
{
#if 0
    shared_ptr<Modbus_Registers> p = Modbus_Registers::Build();
    p->Add_RegisterPlaceHolder(0x03, 1000, 200);
    p->Add_RegisterPlaceHolder(0x04, 1000, 200);
    p->Add_RegisterPlaceHolder(0x03, 1050, 100);
    p->Add_RegisterPlaceHolder(0x04, 1050, 100);
    p->Add_RegisterPlaceHolder(0x06, 100, 50);
#endif
    ModbusProtocol_Factory modbusprotocol_factory;
    Modbus_Factory modbus_factory;

    shared_ptr<Modbus_Protocol> protocol_rtu = modbusprotocol_factory.Protocol_Build(ModbusProtocol::MODBUS_RTU);
    shared_ptr<Modbus_Protocol> protocol_tcp = modbusprotocol_factory.Protocol_Build(ModbusProtocol::MODBUS_TCP);

    shared_ptr<Modbus_Base> modbus_master = modbus_factory.Modbus_Build(ModbusType::MODBUS_MASTER, 1);
    shared_ptr<Modbus_Base> modbus_slave = modbus_factory.Modbus_Build(ModbusType::MODBUS_SLAVE, 1);

    shared_ptr<Modbus_Registers> master_registers = Modbus_Registers::Build();
    master_registers->Add_RegisterPlaceHolder(0x03, 1000, 200);
    master_registers->Add_RegisterPlaceHolder(0x04, 1000, 200);

    shared_ptr<Modbus_Registers> slave_registers = Modbus_Registers::Build();
    slave_registers->Add_RegisterPlaceHolder(0x03, 1000, 200);
    slave_registers->Add_RegisterPlaceHolder(0x04, 1000, 200);

    modbus_master->Mount_RegisterMap(master_registers);
    modbus_master->Mount_Protocol(protocol_rtu);
    modbus_master->SetTime(SetTime);
    //modbus_master->Mount_Protocol(protocol_tcp);

    modbus_slave->Mount_RegisterMap(slave_registers);
    modbus_slave->Mount_Protocol(protocol_rtu);
    //modbus_slave->Mount_Protocol(protocol_tcp);

    modbus_master->Add_Rules(0x04, 1000, 150, 400);
    modbus_master->Add_Rules(0x03, 1000, 150, 200);
    modbus_master->Add_Rules(0x06, 1100, 1);
    modbus_master->Add_Rules(0x10, 1050, 5, {10, 200, 100, 50, 90});

    modbus_slave->UpdateData(1050, 5, {10, 200, 100, 50, 90});

    unsigned char buff[2048];
    unsigned short length;

    static int count = 10;
    while (count >= 0)
    {
        if(count % 5 == 0)
        {
            modbus_master->UpdateData(1100, (unsigned short)20);
        }

        if(count == 7){
            modbus_master->UpdateData(1050, 5, {1, 1, 1, 1, 1});
        }
        
        if(count == 4)
        {
            modbus_master->UpdateData(1052, (unsigned short)10);
        }

        cout << "count:" << count << endl;
        if(modbus_master->Send(buff, length) < 0)
        {
            count--;
            continue;
        }
#if 1
        cout << "modbusmaster send:";
        for (int i = 0; i < length; i++)
        {
            cout << setfill('0') << setw(2) << std::hex << (int)(buff[i]) << " ";
        }
        cout << std::dec << endl;
#endif
        modbus_slave->Recv(buff, length);
        modbus_slave->Send(buff, length);
#if 1
        cout << "modbusslave send:";
        for (int i = 0; i < length; i++)
        {
            cout << setfill('0') << setw(2) << std::hex << (int)(buff[i]) << " ";
        }
        cout << std::dec << endl;
#endif
        modbus_master->Recv(buff, length);
        //cout << "master recv!" << endl;
        count--;
    }

    unsigned short short_data;
    modbus_master->GetData(0x03, 1100, short_data);
    cout << "address 1100 -> data:" << (int)(short_data) << endl;
    vector<unsigned short> short_data2;
    modbus_master->GetData(0x03, 1050, 5, short_data2);
    cout << "address:1050 - address:1054 -> data:";
    for(int i = 0; i < short_data2.size(); i++)
    {
        cout << short_data2[i] << " ";
    }
    cout << endl;
    return 0;
}
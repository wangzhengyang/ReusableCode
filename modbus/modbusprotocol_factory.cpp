#include "modbusprotocol_factory.h"

using namespace std;

shared_ptr<Modbus_Protocol> ModbusProtocol_Factory::Protocol_Build(ModbusProtocol protocol)
{
    shared_ptr<Modbus_Protocol> p;

    switch (protocol)
    {
    case ModbusProtocol::MODBUS_RTU:
        p = make_shared<Modbus_RTU>();
        break;
    case ModbusProtocol::MODBUS_TCP:
        p = make_shared<Modbus_TCP>();
        break;
    }
    return p;
}
#include "modbus_factory.h"

using namespace std;

shared_ptr<Modbus_Base> Modbus_Factory::Modbus_Build(ModbusType type, unsigned char address)
{
    shared_ptr<Modbus_Base> p;

    switch(type)
    {
    case ModbusType::MODBUS_SLAVE:
        p = make_shared<Modbus_Slave>(address);
    break;
    case ModbusType::MODBUS_MASTER:
        p = make_shared<Modbus_Master>(address);
    break;
    }
    
    return p;
}



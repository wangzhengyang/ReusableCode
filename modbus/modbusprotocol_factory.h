#ifndef MODBUSPROTOCOL_FACTORY_H
#define MODBUSPROTOCOL_FACTORY_H

#include <memory>
#include "modbus_rtu.h"
#include "modbus_tcp.h"

enum ModbusProtocol
{
    MODBUS_RTU = 0,
    MODBUS_TCP = 1,
};

class ModbusProtocol_Factory
{
public:
    ModbusProtocol_Factory() = default;
    ~ModbusProtocol_Factory() {}
    std::shared_ptr<Modbus_Protocol> Protocol_Build(ModbusProtocol protocol);
};

#endif
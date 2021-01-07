#ifndef MODBUS_FACTORY_H
#define MODBUS_FACTORY_H

#include <memory>
#include "modbus_slave.h"
#include "modbus_master.h"

enum ModbusType
{
    MODBUS_SLAVE = 0,
    MODBUS_MASTER = 1,
};

class Modbus_Factory
{
public:
    Modbus_Factory() = default;
    ~Modbus_Factory() {}
    std::shared_ptr<Modbus_Base> Modbus_Build(ModbusType type, unsigned char address);
};

#endif
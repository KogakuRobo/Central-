
#include "can_bus_setting.hpp"

void can_bus_driver(_rx621_CAN_bus *can_bus)
{
	can_bus->SetMode(CANM_OPERATION);
	can_bus->SetMask(0,0x000f,0x0000);
	can_bus->SetMask(1,0x0000,0x0000);
	can_bus->SetMask(2,0x0000,0x0000);
	can_bus->SetMask(3,0x0000,0x0000);
	can_bus->SetMask(4,0x0000,0x0000);
	can_bus->SetMask(5,0x0000,0x0000);
	can_bus->SetMask(6,0x0000,0x0000);
	can_bus->SetMask(7,0x0000,0x0000);
}
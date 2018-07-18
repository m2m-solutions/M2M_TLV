#ifndef __M2M_TLV_ID_H__
#define __M2M_TLV_ID_H__

#include "Arduino.h"

enum class ResourceId : uint16_t {
	Manufacturer = 0,
	ModelNumber = 1,
	SerialNumber = 2,
	FirmwareVersion = 3,
	ErrorCode = 11,
	DeviceType = 17,
	Temperature = 3300,
	notInUse = 65535
};

#endif
#ifndef __M2M_TLV_ID_H__
#define __M2M_TLV_ID_H__

#include "Arduino.h"

enum class ObjectId: uint16_t
{
	Device = 3,
	Connectivity = 4,
	Location = 6,
	// IPSO
	DigitalInput = 3200,
	DigitalOutput = 3201,
	AnalogInput = 3202,
	AnalogOutput = 3203,
	GenericSensor = 3300,
	Illuminance = 3301,
	Presence = 3302,
	Temperature = 3303,
	Humidity = 3304,
	PowerMeasurement = 3305,
	Actuation = 3306,
	SetPoint = 3308,
	LightControl = 3311,
	PowerControl = 3312,
	Accelerometer = 3313,
	Magnetometer = 3314,
	Barometer = 3315,	
	OnOffSwitch = 3342,
	// M2M Solutions
	M2MSolutions = 30000,
};

enum class DeviceResourceId: uint16_t
{
	Manufacturer = 0,
	ModelNumber = 1,
	SerialNumber = 2,
	FirmwareVersion = 3,
	PowerSourceVoltage = 7,
	PowerSourceCurrent = 8,
	BatteryLevel = 9,
	MemoryFree = 10,
	ErrorCode = 11,
	CurrentTime = 13,
	DeviceType = 17,
	HardwareVersion = 18,
	SoftwareVersion = 19,
	BatteryStatus = 20
};

enum class ConnectivityResourceId: uint16_t
{
	NetworkBearer = 0,
	RadioSignalStrength = 2,
	LinkQuality = 3,
	IpAddress = 4,
	RouterIpAddress = 5,
	Apn = 7,
	CellId = 8,
};

enum LocationResourceId: uint16_t
{
	Latitude = 0,
	Longitude = 1,
	Altitude = 2,
	Radius = 3,
	Timestamp = 5,
	Speed = 6,
};

enum class IpsoResourceId: uint16_t
{
	// IPSO
	DigitalInputState = 5500,
	SensorValue = 5700,
	notInUse = 65535
};

enum class M2MResourceId: uint16_t
{
	CellularICCID = 30019,
	CellularIMEI = 30020,
};

#endif
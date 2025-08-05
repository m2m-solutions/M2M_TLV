#ifndef __M2M_TLV_ID_H__
#define __M2M_TLV_ID_H__

#include <cstdint>

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
	Voltage = 3316,
	Current = 3317,
	Frequency = 3318,
	Depth = 3319,
	Percentage = 3320,
	Altitude = 3321,
	Load = 3322,
	Preassure = 3323,
	Loudness = 3324,
	Concentration = 3325,
	Acicidty = 3326,
	Conductivity = 3327,
	Power = 3328,
	PowerFactor = 3329,
	Distance = 3330,
	Energy = 3331,
	Direction = 3332,
	Time = 3333,
	Gyrometer = 3334,
	Color = 3335,
	GPSLocation = 3336,
	Positioner = 3337,
	Buzzer = 3338,
	AudioClip = 3339,
	Timer = 3340,
	AddressableTextDisplay = 3341,	
	OnOffSwitch = 3342,
	LevelControl = 3343,
	UpDownControl = 3344,
	MultipleAxisJoystick = 3345,
	Rate = 3346,
	PushButton = 3347,
	MultistateSelector = 3348,
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
	DigitalInputCounter = 5501,
	DigitalInputPolarity = 5502,
	DigitalInputDebounce = 5503,
	DigitalInputEdgeSelection = 5504,
	DigitalInputCounterReset = 5505,
	CurrentTime = 5506,
	FractionalTime = 5507,
	SensorValue = 5700,
	XValue = 5702,
	YValue = 5703,
	ZValue = 5704,
	Direction = 5705,
	notInUse = 65535
};

enum class M2MResourceId: uint16_t
{
	CellularICCID = 30019,
	CellularIMEI = 30020,
	SignalQuality = 30021,
	DataUrl = 30069,
	SharedHash = 30070,
	DeviceHash = 30071,
	FinishUrl = 30072,
	MessageType = 32768
};

enum class ErrorCode : uint8_t {
	noError = 0,
	cellularInitFailed = 1,
	cellularConnectFailed = 2,
	mqttConnectFailed = 10,
	mqttTooManyRetries = 11,
	mqttTopicOverflow = 12,
	flashInitFailed = 20,
	flashFdiskFailed = 21,
	flashFormatFailed = 22,
	flashMountFailed = 23,
	fwUpdateFailed = 24,
	unknownError = 99
};

#endif

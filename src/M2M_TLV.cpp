#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#include "M2M_TLV.h"

#ifndef INT64_MAX
#define INT64_MAX   0x7fffffffffffffffLL
#define INT64_MIN (-0x7fffffffffffffffLL - 1)
#endif
#ifndef INT32_MAX
#define INT32_MAX   0x7fffffff
#define INT32_MIN (-0x7fffffff - 1)
#endif
#ifndef INT16_MAX
#define INT16_MAX   0x7fff
#define INT16_MIN (-0x7fff - 1)
#endif
#ifndef INT8_MAX
#define INT8_MAX    0x7f
#define INT8_MIN  (-0x7f - 1)
#endif

////////////////////////////////////////////////////////////////////////////////////////////////
//
// ObjectLink constructor
//
M2M_TLV::ObjectLink::ObjectLink(uint16_t oid, uint16_t oiid)
	: objectID{oid}, objectInstanceID{oiid} {}

////////////////////////////////////////////////////////////////////////////////////////////////
//
// Input constructor
//
M2M_TLV::Input::Input(M2M_TLV::IdType t, uint16_t i, M2M_TLV::DataType d, size_t l, int64_t v)
	: type{t}, dataType{d}, id{i}, length{l}, integer{v}
{
	switch (dataType) {
	case BOOLEAN:
		length = 1;
		integer = v ? 1 : 0;
		break;
	case TIME:
	case INTEGER:
		if (length == 0)
		{
			if      (v > INT32_MAX || v < INT32_MIN)
				length = 8;
			else if (v > INT16_MAX || v < INT16_MIN)
				length = 4;
			else if (v > INT8_MAX  || v < INT8_MIN)
				length = 2;
			else
				length = 1;
		}
		break;
	case FLOAT:
	case OBJECT_LINK:
		break;
	default:
		break;
	};
}

M2M_TLV::Input::Input(M2M_TLV::IdType t, uint16_t i, M2M_TLV::DataType d, size_t l, void *v)
	: type{t}, dataType{d}, id{i}, length{l}, opaque{v} {}

M2M_TLV::Input::Input(){}
////////////////////////////////////////////////////////////////////////////////////////////////
//
// Input creation functions
//
M2M_TLV::Input M2M_TLV::objectInstance(uint16_t id)
{
 	return Input {OBJECT_INSTANCE, id, OPAQUE, 0, nullptr};
}

M2M_TLV::Input M2M_TLV::multipleResource(uint16_t id)
{
	return M2M_TLV::Input {MULTIPLE_RESOURCE, id, OPAQUE, 0, nullptr};
}

M2M_TLV::Input M2M_TLV::string(uint16_t id, size_t length, const char *value)
{
	return M2M_TLV::Input {RESOURCE_W_VALUE, id, STRING, length, (void *)value};
}

M2M_TLV::Input M2M_TLV::stringRI(uint16_t id, size_t length, const char *value)
{
	return M2M_TLV::Input {RESOURCE_INSTANCE, id, STRING, length, (void *)value};
}

M2M_TLV::Input M2M_TLV::integer(uint16_t id, int64_t value)
{
	return M2M_TLV::Input {RESOURCE_W_VALUE, id, INTEGER, 0, value};
}

M2M_TLV::Input M2M_TLV::integerRI(uint16_t id, int64_t value)
{
	return M2M_TLV::Input {RESOURCE_INSTANCE, id, INTEGER, 0, value};
}

M2M_TLV::Input M2M_TLV::float32(uint16_t id, float value)
{
	// assume float is 32 bit
	uint32_t t = *(uint32_t *)&value;
	return M2M_TLV::Input {RESOURCE_W_VALUE, id, FLOAT, 4, t};
}

M2M_TLV::Input M2M_TLV::float32RI(uint16_t id, float value)
{
	// assume float is 32 bit
	uint32_t t = *(uint32_t *)&value;
	return M2M_TLV::Input {RESOURCE_INSTANCE, id, FLOAT, 4, t};
}

M2M_TLV::Input M2M_TLV::float64(uint16_t id, double value)
{
	if (sizeof(double) == 4)
	{
		int64_t t64 = toFloat64bits(*(int32_t *)&value);
		return M2M_TLV::Input {RESOURCE_W_VALUE, id, FLOAT, 8, t64};
	}

	// else assume double is 64 bit
	int64_t t = *(int64_t *)&value;
	return M2M_TLV::Input {RESOURCE_W_VALUE, id, FLOAT, 8, t};
}

M2M_TLV::Input M2M_TLV::float64RI(uint16_t id, double value)
{
	if (sizeof(double) == 4)
	{
		int64_t t64 = toFloat64bits(*(int32_t *)&value);
		return M2M_TLV::Input {RESOURCE_INSTANCE, id, FLOAT, 8, t64};
	}

	// else assume double is 64 bit
	int64_t t = *(int64_t *)&value;
	return M2M_TLV::Input {RESOURCE_INSTANCE, id, FLOAT, 8, t};
}

M2M_TLV::Input M2M_TLV::boolean(uint16_t id, bool value)
{
	return M2M_TLV::Input {RESOURCE_W_VALUE, id, BOOLEAN, 1, value};
}

M2M_TLV::Input M2M_TLV::booleanRI(uint16_t id, bool value)
{
	return M2M_TLV::Input {RESOURCE_INSTANCE, id, BOOLEAN, 1, value};
}

M2M_TLV::Input M2M_TLV::opaque(uint16_t id, size_t length, uint8_t *value)
{
	return M2M_TLV::Input {RESOURCE_W_VALUE, id, OPAQUE, length, value};
}

M2M_TLV::Input M2M_TLV::opaqueRI(uint16_t id, size_t length, uint8_t *value)
{
	return M2M_TLV::Input {RESOURCE_INSTANCE, id, OPAQUE, length, value};
}

M2M_TLV::Input M2M_TLV::time(uint16_t id, time_t value)
{
	return M2M_TLV::Input {RESOURCE_W_VALUE, id, TIME, 0, value};
}

M2M_TLV::Input M2M_TLV::timeRI(uint16_t id, time_t value)
{
	return M2M_TLV::Input {RESOURCE_INSTANCE, id, TIME, 0, value};
}

M2M_TLV::Input M2M_TLV::objectLink(uint16_t id, ObjectLink value)
{
	uint32_t t = *(uint32_t *)&value;
	return M2M_TLV::Input {RESOURCE_W_VALUE, id, OBJECT_LINK, 4, t};
}

M2M_TLV::Input M2M_TLV::objectLinkRI(uint16_t id, ObjectLink value)
{
	uint32_t t = *(uint32_t *)&value;
	return M2M_TLV::Input {RESOURCE_INSTANCE, id, OBJECT_LINK, 4, t};
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
// ResourceSpecifier constructor
//
M2M_TLV::ResourceSpecifier::ResourceSpecifier(uint16_t l0, uint16_t l1, uint16_t l2)
	: level0{l0}, level1{l1}, level2{l2} {}

////////////////////////////////////////////////////////////////////////////////////////////////
//
// Constructor
//
M2M_TLV::M2M_TLV(uint8_t *byteBuffer, size_t bufferSize)
	: buffer{byteBuffer}, size{bufferSize}, freeBufferOnDestruction{false} {}

M2M_TLV::M2M_TLV(uint8_t *byteBuffer, size_t bufferSize, Input *array, int nrOfElements)
	: buffer{byteBuffer}, freeBufferOnDestruction{false}
{
	size = 0;

	// Calculate sizes
	for (int i = 0; i < nrOfElements; i++)
	{
		switch (array[i].type) {
		case OBJECT_INSTANCE:
			size += sizeOfObjectInstance(array, nrOfElements, i);
			break;
		case MULTIPLE_RESOURCE:
			size += sizeOfMultipleResource(array, nrOfElements, i);
			break;
		case RESOURCE_W_VALUE:
			size += sizeOfResource(&array[i]);
			break;
		default:
			break;
		};
	}

	if (size > bufferSize)
		return; // bufferSize is too small

	// Write buffer
	size_t pos = 0;

	for (int i = 0; i < nrOfElements; i++)
	{
		buffer[pos] = array[i].type & ID_TYPE;
		int idSize = 1;
		int lengthSize = 0;

		if (array[i].id > 0xFF)
			idSize = 2;

		buffer[pos] |= (idSize - 1) << 5;
		writeBE(&(buffer[pos+1]), array[i].id, idSize);

		if      (array[i].length > 0xFFFF)
			lengthSize = 3;
		else if (array[i].length > 0xFF)
			lengthSize = 2;
		else if (array[i].length > 0x7)
			lengthSize = 1;

		if (lengthSize == 0)
		{
			buffer[pos] |= array[i].length;
		}
		else
		{
			buffer[pos] |= lengthSize << 3;
			writeBE(&(buffer[pos + 1 + idSize]), array[i].length, lengthSize);
		}

		pos += 1 + idSize + lengthSize;

		if (array[i].type == RESOURCE_W_VALUE || array[i].type == RESOURCE_INSTANCE)
		{
			switch (array[i].dataType) {
			case BOOLEAN:
			case TIME:
			case INTEGER:
			case FLOAT:
				writeBE(&buffer[pos], *(uint64_t*)&array[i].integer, array[i].length);
				break;
			case OBJECT_LINK:
				writeBE(&buffer[pos], array[i].integer & 0x0000ffff, 2);
				writeBE(&buffer[pos+2], (array[i].integer & 0xffff0000) >> 16, 2);
				break;
			case STRING:
			case OPAQUE:
				memcpy(&(buffer[pos]), array[i].opaque, array[i].length);
				break;
			};
			pos += array[i].length;
		}
	}
}

M2M_TLV::M2M_TLV(Input *array, int nrOfElements)
{
	size = 0;

	// Calculate sizes
	for (int i = 0; i < nrOfElements; i++)
	{
		switch (array[i].type) {
		case OBJECT_INSTANCE:
			size += sizeOfObjectInstance(array, nrOfElements, i);
			break;
		case MULTIPLE_RESOURCE:
			size += sizeOfMultipleResource(array, nrOfElements, i);
			break;
		case RESOURCE_W_VALUE:
			size += sizeOfResource(&array[i]);
			break;
		default:
			break;
		};
	}

	// Create buffer
	buffer = (uint8_t *)malloc(size);
	freeBufferOnDestruction = true;
	size_t pos = 0;

	for (int i = 0; i < nrOfElements; i++)
	{
		buffer[pos] = array[i].type & ID_TYPE;
		int idSize = 1;
		int lengthSize = 0;

		if (array[i].id > 0xFF)
			idSize = 2;

		buffer[pos] |= (idSize - 1) << 5;
		writeBE(&(buffer[pos+1]), array[i].id, idSize);

		if      (array[i].length > 0xFFFF)
			lengthSize = 3;
		else if (array[i].length > 0xFF)
			lengthSize = 2;
		else if (array[i].length > 0x7)
			lengthSize = 1;

		if (lengthSize == 0)
		{
			buffer[pos] |= array[i].length;
		}
		else
		{
			buffer[pos] |= lengthSize << 3;
			writeBE(&(buffer[pos + 1 + idSize]), array[i].length, lengthSize);
		}

		pos += 1 + idSize + lengthSize;

		if (array[i].type == RESOURCE_W_VALUE || array[i].type == RESOURCE_INSTANCE)
		{
			switch (array[i].dataType) {
			case BOOLEAN:
			case TIME:
			case INTEGER:
			case FLOAT:
				writeBE(&buffer[pos], *(uint64_t*)&array[i].integer, array[i].length);
				break;
			case OBJECT_LINK:
				writeBE(&buffer[pos], array[i].integer & 0x0000ffff, 2);
				writeBE(&buffer[pos+2], (array[i].integer & 0xffff0000) >> 16, 2);
				break;
			case STRING:
			case OPAQUE:
				memcpy(&(buffer[pos]), array[i].opaque, array[i].length);
				break;
			};
			pos += array[i].length;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
// Destructor
//
M2M_TLV::~M2M_TLV()
{
	if (freeBufferOnDestruction)
	{
		free(buffer);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
// Buffer functions
//
uint8_t *M2M_TLV::getBuffer()
{
	return buffer;
}

size_t M2M_TLV::getBufferSize()
{
	return size;
}


////////////////////////////////////////////////////////////////////////////////////////////////
//
// Read functions
//
size_t M2M_TLV::resourceLength(ResourceSpecifier rs)
{
	size_t pos = 0;
	size_t length;

	findResource(rs, pos, length);

	return length;
}


bool M2M_TLV::readUint(uint64_t& value, size_t& length, M2M_TLV::ResourceSpecifier rs)
{
	size_t pos = 0;
	if (!findResource(rs, pos, length))
	{
		return false;
	}

	value = readBE(&buffer[pos], length);
	return true;
}

int64_t M2M_TLV::readInt(M2M_TLV::ResourceSpecifier rs)
{
	size_t pos = 0;
	size_t length;
	if (!findResource(rs, pos, length))
	{
		return INT64_MIN;
	}

	switch (length) {
	case 8:
	{
		uint64_t t64 = readBE(&buffer[pos], length);
		return *(int64_t *)&t64;
	}
	case 4:
	{
		uint32_t t32 = readBE(&buffer[pos], length);
		return *(int32_t *)&t32;
	}
	case 2:
	{
		uint16_t t16 = readBE(&buffer[pos], length);
		return *(int16_t *)&t16;
	}
	case 1:
	{
		uint8_t t8 = readBE(&buffer[pos], length);
		return *(int8_t *)&t8;
	}
	};

	return INT64_MIN;
}

time_t M2M_TLV::readTime(M2M_TLV::ResourceSpecifier rs)
{
	int64_t t = readInt(rs);
	if (t == INT64_MIN)
	{
		return 0;
	}

	return (time_t)t;
}

bool M2M_TLV::readBoolean(M2M_TLV::ResourceSpecifier rs)
{
	int64_t t = readInt(rs);
	if (t == INT64_MIN || t == 0)
	{
		return false;
	}

	return true;
}

double M2M_TLV::readDouble(M2M_TLV::ResourceSpecifier rs)
{
	uint64_t eightBytes;
	size_t   length;

	if (!readUint(eightBytes, length, rs))
		return NAN;

	if (length == 4)
	{
		uint32_t fourBytes = eightBytes;
		return *(float *)&fourBytes;
	}
	if (length != 8)
	{
		return NAN;
	}

	if (sizeof(double) == 4)
	{
		int64_t t64 = *(int64_t *)&eightBytes;
		int64_t sign = t64 >> 63;
		int64_t exponent = (t64 >> 52) & 0x7ff;
		int64_t mantissa = t64 & 0x0fffffffffffff;
		sign <<= 31;
		exponent = (exponent + 127 - 1023);
		if ((exponent == 0xff) || (exponent & 0xff) != exponent)
		{
			return sign ? -INFINITY : INFINITY;
		}
		exponent <<= 23;
		mantissa >>= (52-23);
		uint32_t t32 = sign | exponent | mantissa;

		return *(double *)&t32;
	};

	return *(double *)&eightBytes;
}

float M2M_TLV::readFloat(M2M_TLV::ResourceSpecifier rs)
{
	return readDouble(rs);
}

size_t M2M_TLV::readString(char *dst, size_t dstLength, M2M_TLV::ResourceSpecifier rs)
{
	size_t pos = 0;
	size_t resourceLength;

	findResource(rs, pos, resourceLength);

	if (dstLength < 1)
	{
		return resourceLength;
	}

	if (resourceLength > dstLength - 1)
	{
		memcpy(dst, &buffer[pos], dstLength - 1);
		dst[dstLength - 1] = '\0';
	}
	else
	{
		memcpy(dst, &buffer[pos], resourceLength);
		dst[resourceLength] = '\0';
	}

	return resourceLength;
}

size_t M2M_TLV::readOpaque(uint8_t *dst, size_t dstLength, M2M_TLV::ResourceSpecifier rs)
{
	size_t pos = 0;
	size_t resourceLength;

	findResource(rs, pos, resourceLength);
	
	if (dstLength < 1)
	{
		return resourceLength;
	}

	if (resourceLength > dstLength)
	{
		memcpy(dst, &buffer[pos], dstLength);
	}
	else
	{
		memcpy(dst, &buffer[pos], resourceLength);
	}

	return resourceLength;
}

M2M_TLV::ObjectLink M2M_TLV::readObjectLink(M2M_TLV::ResourceSpecifier rs)
{
	size_t pos = 0;
	size_t length;
	if (!findResource(rs, pos, length) || length != 4)
	{
		return {MAX_ID, MAX_ID};
	}

	return {(uint16_t)readBE(&buffer[pos], 2), (uint16_t)readBE(&buffer[pos+2], 2)};
}


////////////////////////////////////////////////////////////////////////////////////////////////
//
// Private functions
//
int64_t M2M_TLV::toFloat64bits(int32_t f32)
{
	int64_t sign = f32 >> 31;
	int64_t exponent = (f32 >> 23) & 0xff;
	int64_t mantissa = f32 & 0x7fffff;

	sign <<= 63;
	exponent = (exponent + 1023 - 127) << 52;
	mantissa <<= 29;

	return sign | exponent | mantissa;
}

void M2M_TLV::writeBE(uint8_t *to, uint64_t value, uint64_t size)
{
	for (uint64_t i = 0; i < size; i++)
	{
		to[i] = 0xFF & (value >> 8*(size - (i+1)));
	}
}

uint64_t M2M_TLV::readBE(uint8_t *from, uint64_t size)
{
	uint64_t result = 0;

	for (uint64_t i = 0; i < size; i++)
	{
		result |= (uint64_t)from[i] << 8*(size - (i+1));
	}

	return result;
}

size_t M2M_TLV::sizeOfResource(M2M_TLV::Input *resource)
{
	size_t elementSize = 2; // always 1 type and at least 1 id

	if (resource->id > 0xFF)
		elementSize += 1;

	if      (resource->length > 0xFFFF)
		elementSize+=3;
	else if (resource->length > 0xFF)
		elementSize+=2;
	else if (resource->length > 7)
		elementSize+=1;

	return elementSize + resource->length;
}

size_t M2M_TLV::sizeOfMultipleResource(M2M_TLV::Input *array, int nrOfElements, int& i)
{
	int mr = i;
	array[mr].length = 0;

	for (i++; i < nrOfElements && array[i].type == RESOURCE_INSTANCE; i++)
		array[mr].length += sizeOfResource(&array[i]);
	i--;

	return sizeOfResource(&array[mr]);
}

size_t M2M_TLV::sizeOfObjectInstance(M2M_TLV::Input *array, int nrOfElements, int& i)
{
	int oi = i;
	array[oi].length = 0;

	for (i++; i < nrOfElements && array[i].type != OBJECT_INSTANCE; i++)
	{
		switch (array[i].type) {
		case MULTIPLE_RESOURCE:
			array[oi].length += sizeOfMultipleResource(array, nrOfElements, i);
			break;
		case RESOURCE_W_VALUE:
			array[oi].length += sizeOfResource(&array[i]);
			break;
		default:
			break;
		};
	}
	i--;

	return sizeOfResource(&array[oi]);
}

bool M2M_TLV::findResource(M2M_TLV::ResourceSpecifier rs, size_t& pos, size_t& length)
{
	uint16_t id;
	int idSize;
	int lengthSize;
	uint8_t type;

	while (pos < size)
	{
		type = buffer[pos];
		idSize = 1 + ((type & ID_LENGTH) >> 5);
		lengthSize = (type & LENGTH_TYPE) >> 3;

		id = readBE(&buffer[pos+1], idSize);
		if (lengthSize == 0)
			length = type & VALUE_LENGTH;
		else
			length = readBE(&buffer[pos+1+idSize], lengthSize);

		pos += 1 + idSize + lengthSize;

		if (id == rs.level0)
		{
			if (rs.level1 == MAX_ID)
				return true;
			else if (rs.level2 == MAX_ID)
				return findResource({rs.level1}, pos, length);
			else
				return findResource({rs.level1, rs.level2}, pos, length);
		}

		pos += length;
	}

	length = 0;
	return false;
}

#ifndef M2M_TLV_h
#define M2M_TLV_h

class M2M_TLV {
public:
	enum IdType : uint8_t {
		OBJECT_INSTANCE =   0b00000000,
		RESOURCE_INSTANCE = 0b01000000,
		MULTIPLE_RESOURCE = 0b10000000,
		RESOURCE_W_VALUE =  0b11000000
	};

	enum DataType : uint8_t {
		STRING,		// UTF-8
		INTEGER,	// 8, 16, 32 or 64-bit, signed
		FLOAT,		// 32 or 64-bit
		BOOLEAN,	// integer with 0 for false, 1 for true
		OPAQUE,		// sequence of binary octets
		TIME,		// unix time
		OBJECT_LINK	// two concatenated 16-bit unsigned integers (BE)
	};

	struct ObjectLink {
		uint16_t objectID = MAX_ID;
		uint16_t objectInstanceID = MAX_ID;

		ObjectLink(uint16_t oid, uint16_t oiid);
	};

	struct Input {
		IdType   type;
		DataType dataType;
		uint16_t id;
		size_t   length;
		union {
			void   *opaque;
			int64_t integer;
		};

		Input(IdType t, uint16_t i, DataType d, size_t l, int64_t v);
		Input(IdType t, uint16_t i, DataType d, size_t l, void *v);
		Input();
	};

	static Input objectInstance(uint16_t id);
	static Input multipleResource(uint16_t id);
	static Input string(uint16_t id, size_t length, const char *value);
	static Input stringRI(uint16_t id, size_t length, const char *value);
	static Input integer(uint16_t id, int64_t value);
	static Input integerRI(uint16_t id, int64_t value);
	static Input float32(uint16_t id, float value);
	static Input float32RI(uint16_t id, float value);
	static Input float64(uint16_t id, double value);
	static Input float64RI(uint16_t id, double value);
	static Input boolean(uint16_t id, bool value);
	static Input booleanRI(uint16_t id, bool value);
	static Input opaque(uint16_t id, size_t length, uint8_t *value);
	static Input opaqueRI(uint16_t id, size_t length, uint8_t *value);
	static Input time(uint16_t id, time_t value);
	static Input timeRI(uint16_t id, time_t value);
	static Input objectLink(uint16_t id, ObjectLink value);
	static Input objectLinkRI(uint16_t id, ObjectLink value);

	struct ResourceSpecifier {
		uint16_t level0;
		uint16_t level1 = MAX_ID;
		uint16_t level2 = MAX_ID;

		ResourceSpecifier(uint16_t l0, uint16_t l1=MAX_ID, uint16_t l2=MAX_ID);
	};

	M2M_TLV(uint8_t *byteBuffer, size_t bufferSize);
	M2M_TLV(Input *array, int nrOfElements);
	M2M_TLV(uint8_t *byteBuffer, size_t bufferSize, Input *array, int nrOfElements);
	~M2M_TLV();

	uint8_t *getBuffer();
	size_t   getBufferSize();

	size_t   resourceLength(ResourceSpecifier rs);
	bool     readUint(uint64_t& value, size_t& length, ResourceSpecifier rs);
	int64_t  readInt(ResourceSpecifier rs);
	time_t   readTime(ResourceSpecifier rs);
	double   readDouble(ResourceSpecifier rs);
	float    readFloat(ResourceSpecifier rs);
	bool     readBoolean(ResourceSpecifier rs);
	size_t   readString(char *dst, size_t dstLength, ResourceSpecifier rs);
	size_t   readOpaque(uint8_t *dst, size_t dstLength, ResourceSpecifier rs);
	ObjectLink readObjectLink(ResourceSpecifier rs);

private:
	uint8_t *buffer;
	size_t	 size;
	bool	 freeBufferOnDestruction;

	static const int MAX_ID = 65535;

	enum TypeField : uint8_t {
		ID_TYPE =      0b11000000,
		ID_LENGTH =    0b00100000,
		LENGTH_TYPE =  0b00011000,
		VALUE_LENGTH = 0b00000111
	};

	static int64_t toFloat64bits(int32_t f32);
	void writeBE(uint8_t *to, uint64_t value, uint64_t size);
	uint64_t readBE(uint8_t *from, uint64_t size);

	size_t sizeOfObjectInstance(Input *array, int nrOfElements, int& i);
	size_t sizeOfMultipleResource(Input *array, int nrOfElements, int& i);
	size_t sizeOfResource(Input *resource);

	bool  findResource(ResourceSpecifier rs, size_t& pos, size_t& length);
};

#endif

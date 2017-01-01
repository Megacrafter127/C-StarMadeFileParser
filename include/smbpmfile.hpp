#ifndef SMBPMFILE_H_
#define SMBPMFILE_H_

#include "io.hpp"
#include "vector3.hpp"
#include <cstdint>
#include <vector>
#include <string>

typedef std::string string_t ;

struct DockedEntry {
	string_t name;
	vector3i dockingPos;
	vector3f dockingSize;
	int16_t dockingStyle;
	char dockingOrientation;
};

struct DockedBlueprints {
	int32_t dockedCount;
	std::vector<DockedEntry> dockedEntries;
};

typedef std::vector<char> ByteArray;

enum class PayloadType {
	NONE = 0,
	BYTE = 1,
	SHORT = 2,
	INT = 3,
	LONG = 4,
	FLOAT = 5,
	DOUBLE = 6,
	BYTEARRAY = 7,
	STRING = 8,
	FLOATVECTOR = 9,
	INTVECTOR = 10,
	BYTEVECTOR = 11,
	LIST = 12,
	STRUCT = 13,
	FACTORYREGISTRATION = 14,
	FLOAT4VECTOR = 15
};

union Payload;

struct TagList {
	PayloadType listType; // PayloadType
	uint32_t listLength;
	std::vector<Payload> payload; // [listLength], only contains listType
};

struct Tag;

struct TagStructure {
	std::vector<Tag> tags;
};

union Payload {
	Payload() :
	dataByte{'\0'} {}
	Payload(const Payload&) {}
	~Payload() {}
	
	char dataByte;
	int16_t dataShort;
	int32_t dataInt;
	int64_t dataLong;
	float dataFloat;
	double dataDouble;
	ByteArray byteArray;
	string_t dataString;
	vector3f floatVector;
	vector3i intVector;
	vector3b byteVector;
	TagList list;
	TagStructure structure;
	char factoryId;
	vector4f float4Vector;
};

struct Tag {
	Tag() :
	type(PayloadType::NONE),
	name(),
	payload()
	{}
	
	Tag(const Tag& other) :
	type(other.type),
	name(other.name) {
		switch(type) {
		case PayloadType::NONE:
			break;
		case PayloadType::BYTE:
			payload.dataByte = other.payload.dataByte;
			break;
		case PayloadType::SHORT:
			payload.dataShort = other.payload.dataShort;
			break;
		case PayloadType::INT:
			payload.dataInt = other.payload.dataInt;
			break;
		case PayloadType::LONG:
			payload.dataLong = other.payload.dataLong;
			break;
		case PayloadType::FLOAT:
			payload.dataFloat = other.payload.dataFloat;
			break;
		case PayloadType::DOUBLE:
			payload.dataDouble = other.payload.dataDouble;
			break;
		case PayloadType::FACTORYREGISTRATION:
			payload.factoryId = other.payload.factoryId;
			break;
		case PayloadType::FLOATVECTOR:
			new (&payload.floatVector) vector3f(other.payload.floatVector);
			break;
		case PayloadType::INTVECTOR:
			new (&payload.intVector) vector3i(other.payload.intVector);
			break;
		case PayloadType::BYTEVECTOR:
			new (&payload.byteVector) vector3b(other.payload.byteVector);
			break;
		case PayloadType::BYTEARRAY:
			new (&payload.byteArray) ByteArray(other.payload.byteArray);
			break;
		case PayloadType::STRING:
			new (&payload.dataString) string_t(other.payload.dataString);
			break;
		case PayloadType::LIST:
			new (&payload.list) TagList(other.payload.list);
			break;
		case PayloadType::STRUCT:
			new (&payload.structure) TagStructure(other.payload.structure);
			break;
		case PayloadType::FLOAT4VECTOR:
			new (&payload.float4Vector) vector4f(other.payload.float4Vector);
			break;
		}
	}
	
	~Tag() {
		switch(type) {
		case PayloadType::NONE:
		case PayloadType::BYTE:
		case PayloadType::SHORT:
		case PayloadType::INT:
		case PayloadType::LONG:
		case PayloadType::FLOAT:
		case PayloadType::DOUBLE:
		case PayloadType::FACTORYREGISTRATION:
			break;
		case PayloadType::FLOATVECTOR:
			payload.floatVector.~vector3f();
			break;
		case PayloadType::INTVECTOR:
			payload.intVector.~vector3i();
			break;
		case PayloadType::BYTEVECTOR:
			payload.byteVector.~vector3b();
			break;
		case PayloadType::BYTEARRAY:
		payload.byteArray.~ByteArray();
			break;
		case PayloadType::STRING:
		payload.dataString.~string_t();
			break;
		case PayloadType::LIST:
		payload.list.~TagList();
			break;
		case PayloadType::STRUCT:
		payload.structure.~TagStructure();
			break;
		case PayloadType::FLOAT4VECTOR:
		payload.float4Vector.~vector4f();
			break;
		}
	}
	
	PayloadType /*char*/ type;
	string_t name; // if(type > 0)
	Payload payload; // if(type != 0)
};

struct TagRoot {
	uint16_t version;
	Tag tag;
};

struct smbpmfile {
	
	enum {
		Finish = 1,
		SegManager = 2,
		Docking = 3
	};
	unsigned int version;
	std::vector<TagRoot> segmentManager;
	std::vector<DockedBlueprints> dockedBlueprints;
};

std::istream& operator>>(std::istream& i, smbpmfile& f);
std::ostream& operator<<(std::ostream& o, const smbpmfile& f);

#endif // SMBPMFILE_H_

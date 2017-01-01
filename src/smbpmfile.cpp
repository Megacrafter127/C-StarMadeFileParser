#include "smbpmfile.hpp"

static std::istream& operator>>(std::istream& i, string_t& s);
static std::istream& operator>>(std::istream& i, DockedEntry& d);
static std::istream& operator>>(std::istream& i, DockedBlueprints& d);
static std::istream& operator>>(std::istream& i, ByteArray& b);
static std::istream& operator>>(std::istream& i, TagList& t);
static void readPayload(std::istream& i, Payload& p, PayloadType t);
static std::istream& operator>>(std::istream& i, TagStructure& t);
static std::istream& operator>>(std::istream& i, PayloadType& t);
static std::istream& operator>>(std::istream& i, Tag& t);
static std::istream& operator>>(std::istream& i, TagRoot& t);

static std::ostream& operator<<(std::ostream& o, const string_t& s);
static std::ostream& operator<<(std::ostream& o, const DockedEntry& d);
static std::ostream& operator<<(std::ostream& o, const DockedBlueprints& d);
static std::ostream& operator<<(std::ostream& o, const ByteArray& b);
static std::ostream& operator<<(std::ostream& o, const TagList& t);
static void writePayload(std::ostream& o, const Payload& p, PayloadType t);
static std::ostream& operator<<(std::ostream& o, const TagStructure& t);
static std::ostream& operator<<(std::ostream& o, const PayloadType& t);
static std::ostream& operator<<(std::ostream& o, const Tag& t);
static std::ostream& operator<<(std::ostream& o, const TagRoot& t);

static std::istream& operator>>(std::istream& i, string_t& s) {
	int32_t n = readFromNetStream<int32_t>(i);
	std::vector<char> buffer(n+1, '\0');
	i.read(buffer.data(), n);
	s = buffer.data();
	return i;
}

static std::ostream& operator<<(std::ostream& o, const string_t& s) {
	const int32_t n = s.length();
	writeToNetStream<int32_t>(o, n);
	o.write(s.c_str(), n);
	return o;
}

static std::istream& operator>>(std::istream& i, DockedEntry& d) {
	i >> d.name >> d.dockingPos >> d.dockingSize;
	d.dockingStyle = readFromNetStream<int16_t>(i);
	i.read(&d.dockingOrientation, 1);
	return i;
}

static std::ostream& operator<<(std::ostream& o, const DockedEntry& d) {
	o << d.name << d.dockingPos << d.dockingSize;
	writeToNetStream<int16_t>(o, d.dockingStyle);
	o.write(&d.dockingOrientation, 1);
	return o;
}

static std::istream& operator>>(std::istream& i, DockedBlueprints& d) {
	d.dockedCount = readFromNetStream<int32_t>(i);
	for(int k = 0 ; k < d.dockedCount ; ++k)
		i >> d.dockedEntries[k];
	return i;
}

static std::ostream& operator<<(std::ostream& o, const DockedBlueprints& d) {
	writeToNetStream<int32_t>(o, d.dockedCount);
	for(int k = 0 ; k < d.dockedCount ; ++k)
		o << d.dockedEntries[k];
	return o;
}

static std::istream& operator>>(std::istream& i, ByteArray& b) {
	const int32_t n = readFromNetStream<int32_t>(i);
	b.resize(n);
	i.read(b.data(), n);
	return i;
}

static std::ostream& operator<<(std::ostream& o, const ByteArray& b) {
	const int32_t n = b.size();
	writeToNetStream<int32_t>(o, n);
	o.write(b.data(), n);
	return o;
}

static std::istream& operator>>(std::istream& i, TagList& t) {
	i >> t.listType;
	t.listLength = readFromNetStream<uint32_t>(i);
	t.payload.resize(t.listLength);
	for(std::size_t k = 0 ; k < t.listLength ; ++k) {
		t.payload.push_back(Payload());
		readPayload(i, t.payload.back(), t.listType);
	}
	return i;
}

static std::ostream& operator<<(std::ostream& o, const TagList& t) {
	o << t.listType;
	const uint32_t n = t.listLength;
	writeToNetStream<uint32_t>(o, n);
	for(const Payload& payload : t.payload)
		writePayload(o, payload, t.listType);
	return o;
}

static void readPayload(std::istream& i, Payload& p, PayloadType t) {
	switch(t) {
	case PayloadType::BYTE:
		i.read(&p.dataByte, 1);
		break;
	case PayloadType::SHORT:
		p.dataShort = readFromNetStream<int16_t>(i);
		break;
	case PayloadType::INT:
		p.dataInt = readFromNetStream<int32_t>(i);
		break;
	case PayloadType::LONG:
		p.dataLong = readFromNetStream<int64_t>(i);
		break;
	case PayloadType::FLOAT:
		p.dataFloat = readFromNetStream<float>(i);
		break;
	case PayloadType::DOUBLE:
		p.dataDouble = readFromNetStream<double>(i);
		break;
	case PayloadType::BYTEARRAY:
		i >> p.byteArray;
		break;
	case PayloadType::STRING:
		i >> p.dataString;
		break;
	case PayloadType::FLOATVECTOR:
		i >> p.floatVector;
		break;
	case PayloadType::INTVECTOR:
		i >> p.floatVector;
		break;
	case PayloadType::BYTEVECTOR:
		i >> p.byteVector;
		break;
	case PayloadType::LIST:
		i >> p.list;
		break;
	case PayloadType::STRUCT:
		i >> p.structure;
		break;
	case PayloadType::FACTORYREGISTRATION:
		i.read(&p.factoryId, 1);
		break;
	case PayloadType::FLOAT4VECTOR:
		i >> p.float4Vector;
		break;
	case PayloadType::NONE:
		default:
		break;
	}
}

static void writePayload(std::ostream& o, const Payload& p, PayloadType t) {
	switch(t) {
	case PayloadType::BYTE:
		o.write(&p.dataByte, 1);
		break;
	case PayloadType::SHORT:
		writeToNetStream<int16_t>(o, p.dataShort);
		break;
	case PayloadType::INT:
		writeToNetStream<int32_t>(o, p.dataInt);
		break;
	case PayloadType::LONG:
		writeToNetStream<int64_t>(o, p.dataLong);
		break;
	case PayloadType::FLOAT:
		writeToNetStream<float>(o, p.dataFloat);
		break;
	case PayloadType::DOUBLE:
		writeToNetStream<double>(o, p.dataDouble);
		break;
	case PayloadType::BYTEARRAY:
		o << p.byteArray;
		break;
	case PayloadType::STRING:
		o << p.dataString;
		break;
	case PayloadType::FLOATVECTOR:
		o << p.floatVector;
		break;
	case PayloadType::INTVECTOR:
		o << p.floatVector;
		break;
	case PayloadType::BYTEVECTOR:
		o << p.byteVector;
		break;
	case PayloadType::LIST:
		o << p.list;
		break;
	case PayloadType::STRUCT:
		o << p.structure;
		break;
	case PayloadType::FACTORYREGISTRATION:
		o.write(&p.factoryId, 1);
		break;
	case PayloadType::FLOAT4VECTOR:
		o << p.float4Vector;
		break;
	case PayloadType::NONE:
	default:
		break;
	}
}

static std::istream& operator>>(std::istream& i, TagStructure& t) {
	do {
		t.tags.push_back(Tag());
		i >> t.tags.back();
	} while(t.tags.back().type != PayloadType::NONE);
	return i;
}

static std::ostream& operator<<(std::ostream& o, const TagStructure& t) {
	for(const Tag& tag : t.tags)
		o << tag;
	return o;
}

static std::istream& operator>>(std::istream& i, PayloadType& t) {
	char tmp;
	i.read(&tmp, 1);
	tmp = (tmp < 0 || tmp >= 16)?0:tmp;
	t = static_cast<PayloadType>(tmp);
	return i;
}

static std::ostream& operator<<(std::ostream& o, const PayloadType& t) {
	const char tmp = static_cast<char>(t);
	o.write(&tmp, 1);
	return o;
}

static std::istream& operator>>(std::istream& i, Tag& t) {
	i >> t.type;
	if(t.type == PayloadType::NONE)
		return i;
	i >> t.name;
	readPayload(i, t.payload, t.type);
	return i;
}

static std::ostream& operator<<(std::ostream& o, const Tag& t) {
	o << t.type;
	if(t.type == PayloadType::NONE)
		return o;
	o << t.name;
	writePayload(o, t.payload, t.type);
	return o;
}

static std::istream& operator>>(std::istream& i, TagRoot& t) {
	t.version = readFromNetStream<uint16_t>(i);
	return i >> t.tag;
}

static std::ostream& operator<<(std::ostream& o, const TagRoot& t) {
	writeToNetStream<uint16_t>(o, t.version);
	return o << t.tag;
}

std::istream& operator>>(std::istream& i, smbpmfile& f) {
	f.version = readFromNetStream<uint32_t>(i);
	char type;
	do {
		i.read(&type, 1);
		switch(type) {
		case smbpmfile::SegManager:
			f.segmentManager.push_back(TagRoot());
			i >> f.segmentManager.back();
			break;
		case smbpmfile::Docking:
			f.dockedBlueprints.push_back(DockedBlueprints());
			i >> f.dockedBlueprints.back();
		default:
		case smbpmfile::Finish:
			break;
		}
	} while(type != smbpmfile::Finish);
	return i;
}

std::ostream& operator<<(std::ostream& o, const smbpmfile& f) {
	writeToNetStream<uint32_t>(o, f.version);
	for(const TagRoot& tagroot : f.segmentManager)
		o << static_cast<char>(smbpmfile::SegManager) << tagroot;
	for(const DockedBlueprints& dockedBp : f.dockedBlueprints)
		o << static_cast<char>(smbpmfile::Docking) << dockedBp;
	o << static_cast<char>(smbpmfile::Finish);
	return o;
}

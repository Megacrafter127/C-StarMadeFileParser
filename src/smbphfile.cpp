#include "smbphfile.hpp"
#include "io.hpp"

BBox::BBox() {}

BBox::BBox(vector3f min, vector3f max):
min(min),
max(max) {}

BBox& BBox::merge(const BBox& other) {
	min.x = std::min(min.x, other.min.x);
	min.y = std::min(min.y, other.min.y);
	min.z = std::min(min.z, other.min.z);
	max.x = std::max(max.x, other.max.x);
	max.y = std::max(max.y, other.max.y);
	max.z = std::max(max.z, other.max.z);
	return *this;
}

Element::Element(int16_t id, int32_t count):
blockId(id),
count(count) {}

static std::istream& operator>>(std::istream& i, BpType& t) {
	uint32_t tmp;
	tmp = readFromNetStream<uint32_t>(i);
	switch(tmp) {
		case 1:
			t = BpType::Shop;
			break;
		case 2:
			t = BpType::SpaceStation;
			break;
		case 3:
			t = BpType::Asteroid;
			break;
		case 4:
			t = BpType::Planet;
			break;
		case 0:
		default:
			t = BpType::Ship;
	}
	return i;
}

static std::ostream& operator<<(std::ostream& o, BpType t) {
	uint32_t tmp;
	switch(t) {
		case BpType::Shop:
			tmp = 1;
			break;
		case BpType::SpaceStation:
			tmp = 2;
			break;
		case BpType::Asteroid:
			tmp = 3;
			break;
		case BpType::Planet:
			tmp = 4;
			break;
		default:
		case BpType::Ship:
			tmp = 0;
	}
	writeToNetStream<uint32_t>(o, tmp);
	return o;
}

static std::istream& operator>>(std::istream& i, vector3f& v) {
	v.x = readFromNetStream<float>(i);
	v.y = readFromNetStream<float>(i);
	v.z = readFromNetStream<float>(i);
	return i;
}

static std::ostream& operator<<(std::ostream& o, vector3f v) {
	writeToNetStream<float>(o, v.x);
	writeToNetStream<float>(o, v.y);
	writeToNetStream<float>(o, v.z);
	return o;
}

static std::istream& operator>>(std::istream& i, BBox& b) {
	return i >> b.min >> b.max;
}

static std::ostream& operator<<(std::ostream& o, const BBox& b) {
	return o << b.min << b.max;
}

static std::istream& operator>>(std::istream& i, ElementCountMap& elements) {
	int32_t size;
	size = readFromNetStream<int32_t>(i);
	elements.clear();
	elements.reserve(size);
	for( int j = 0 ; j < size ; ++j )
		elements.emplace_back(readFromNetStream<int16_t>(i),
		                      readFromNetStream<int32_t>(i));
	return i;
}

static std::ostream& operator<<(std::ostream& o, const ElementCountMap& elements) {
	writeToNetStream<int32_t>(o, elements.size());
	for(auto e : elements) {
		writeToNetStream<int16_t>(o, e.blockId);
		writeToNetStream<int32_t>(o, e.count);
	}
	return o;
}

std::istream& operator>>(std::istream& i, BpHeader& header) {
	header.version = readFromNetStream<int32_t>(i);
	return i >> header.type >> header.boundingBox >> header.elements;
}

std::ostream& operator<<(std::ostream& o, const BpHeader& header) {
	writeToNetStream<int32_t>(o, header.version);
	return o << header.type << header.boundingBox << header.elements;
}

const BBox& BpHeader::getBoundingBox() const {
	return boundingBox;
}

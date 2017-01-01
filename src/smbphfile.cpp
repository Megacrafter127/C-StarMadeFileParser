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
	const uint32_t tmp = readFromNetStream<uint32_t>(i);
	t = tmp > 6 ? BpType::Ship : static_cast<BpType>(tmp);
	return i;
}

static std::ostream& operator<<(std::ostream& o, BpType t) {
	writeToNetStream<uint32_t>(o, static_cast<uint32_t>(t));
	return o;
}

static std::istream& operator>>(std::istream& i, BpCategory& c) {
	const uint32_t tmp = readFromNetStream<uint32_t>(i);
	c = tmp > 22 ? BpCategory::GeneralShip : static_cast<BpCategory>(tmp);
	return i;
}

static std::ostream& operator<<(std::ostream& o, BpCategory c) {
	writeToNetStream<uint32_t>(o, static_cast<uint32_t>(c));
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

static std::ostream& operator<<(std::ostream& o,
                                const ElementCountMap& elements) {
	writeToNetStream<int32_t>(o, elements.size());
	for(auto e : elements) {
		writeToNetStream<int16_t>(o, e.blockId);
		writeToNetStream<int32_t>(o, e.count);
	}
	return o;
}

ShipScore::ShipScore() :
defensive(-1),
power(-1),
mobility(-1),
danger(-1),
survivability(-1),
offensive(-1),
support(-1),
mining(-1) {}

static std::istream& operator>>(std::istream& i, ShipScore& scores) {
	char c;
	if(!(c = i.get()) || c == EOF)
		return i;
	const short s = readFromNetStream<uint16_t>(i);
	if(s == 0 || s == 1) {
		i.ignore(8); // offensive is read twice for some reason
		scores.defensive = readFromNetStream<double>(i);
		scores.power = readFromNetStream<double>(i);
		scores.mobility = readFromNetStream<double>(i);
		scores.danger = readFromNetStream<double>(i);
		scores.survivability = readFromNetStream<double>(i);
		scores.offensive = readFromNetStream<double>(i);
		scores.support = readFromNetStream<double>(i);
		scores.mining = s==1 ? readFromNetStream<double>(i) : -1;
	};
	return i;
}

static std::ostream& operator<<(std::ostream& o, const ShipScore& scores) {
	if(scores.offensive >= 0) {
		o.put(1);
		writeToNetStream<uint16_t>(o, scores.mining >= 0 ? 1 : 0);
		writeToNetStream<double>(o, scores.offensive);
		writeToNetStream<double>(o, scores.defensive);
		writeToNetStream<double>(o, scores.power);
		writeToNetStream<double>(o, scores.mobility);
		writeToNetStream<double>(o, scores.danger);
		writeToNetStream<double>(o, scores.survivability);
		writeToNetStream<double>(o, scores.offensive);
		writeToNetStream<double>(o, scores.support);
		if(scores.mining >= 0)
			writeToNetStream<double>(o, scores.offensive);
	} else
		o.put(0);
	return o;
}

static BpCategory defaultCategory(BpType t) {
	switch(t) {
	case BpType::Shop:
		return BpCategory::GeneralShop;
	case BpType::SpaceStation:
		return BpCategory::GeneralStation;
	case BpType::FloatingRockManaged:
		return BpCategory::GeneralFloatingRockManaged;
	case BpType::FloatingRock:
		return BpCategory::GeneralFloatingRock;
	case BpType::Planet:
		return BpCategory::GeneralPlanet;
	case BpType::PlanetIco:
		return BpCategory::UnknownPlanetIco;
	default:
		return BpCategory::GeneralShip;
	}
	return BpCategory::GeneralShip;
}

std::istream& operator>>(std::istream& i, BpHeader& header) {
	int32_t version = readFromNetStream<int32_t>(i);
	i >> header.type;
	if(version >= 3)
		i >> header.category;
	else
		header.category = defaultCategory(header.type);
	return i >> header.boundingBox >> header.elements >> header.scores;
}

std::ostream& operator<<(std::ostream& o, const BpHeader& header) {
	writeToNetStream<int32_t>(o, 3);
	return o << header.type << header.category << header.boundingBox
	         << header.elements << header.scores;
}

const BBox& BpHeader::getBoundingBox() const {
	return boundingBox;
}

const ShipScore& BpHeader::getScore() const {
	return scores;
}

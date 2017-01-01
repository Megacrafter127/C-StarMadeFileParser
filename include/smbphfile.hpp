#ifndef SMBPHFILE_H_
#define SMBPHFILE_H_

#include "vector3.hpp"
#include <iostream>
#include <vector>

enum class BpType {
	Ship = 0,
	Shop = 1,
	SpaceStation = 2,
	FloatingRockManaged = 3,
	FloatingRock = 4,
	Planet = 5,
// shouldn't appear in header files, it seems to be partially supported by
// StarMade's source
	PlanetIco = 6
};

enum class BpCategory {
// Ship type
	GeneralShip = 0,
	MiningShip = 1,
	SupportShip = 2,
	CargoShip = 3,
	AttackShip = 4,
	DefenseShip = 5,
	CarrierShip = 6,
	ScoutShip = 7,
	ScavengerShip = 8,
// Station type
	GeneralStation = 9,
	ShipyardStation = 10,
	OutpostStation = 11,
	DefenseStation = 12,
	MiningStation = 13,
	FactoryStation = 14,
	TradeStation = 15,
	WarpGateStation = 16,
	ShoppingStation = 17,
// "FLOATINGROCK" type
	GeneralFloatingRock = 18,
// "FLOATINGROCKMANAGED" type
	GeneralFloatingRockManaged = 19,
// "PLANET" type
	GeneralPlanet = 20,
// "SHOP" type
	GeneralShop = 21,
// "PLANETICO" type
	UnknownPlanetIco = 22
};

struct BBox {
	BBox();
	BBox(vector3f min, vector3f max);
	
	BBox& merge(const BBox& other);
	
	vector3f min;
	vector3f max;
};


struct Element {
	Element(int16_t id, int32_t count);
	
	int16_t blockId;
	int32_t count;
};

typedef std::vector<Element> ElementCountMap;

struct ShipScore {
	ShipScore();
	double defensive;
	double power;
	double mobility;
	double danger;
	double survivability;
	double offensive;
	double support;
	double mining;
};

class BpHeader;

std::istream& operator>>(std::istream&, BpHeader&);
std::ostream& operator<<(std::ostream&, const BpHeader&);

class BpHeader {
	friend std::istream& operator>>(std::istream&, BpHeader&);
	friend std::ostream& operator<<(std::ostream&, const BpHeader&);
public:
	const BBox& getBoundingBox() const;
	const ShipScore& getScore() const;
private:
	//int32_t version;
	BpType type;
	BpCategory category;
	BBox boundingBox;
	ElementCountMap elements;
	ShipScore scores;
};

#endif // SMBPHFILE_H_

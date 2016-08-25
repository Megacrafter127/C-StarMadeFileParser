#ifndef SMBPHFILE_H_
#define SMBPHFILE_H_

#include <iostream>
#include <vector>

enum class BpType {
	Ship = 0,
	Shop = 1,
	SpaceStation = 2,
	Asteroid = 3,
	Planet = 4
};

struct vector3f {
	float x, y, z;
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

class BpHeader;

std::istream& operator>>(std::istream&, BpHeader&);
std::ostream& operator<<(std::ostream&, const BpHeader&);

class BpHeader {
	friend std::istream& operator>>(std::istream&, BpHeader&);
	friend std::ostream& operator<<(std::ostream&, const BpHeader&);
public:
	const BBox& getBoundingBox() const;
	// ...
private:
	int32_t version;
	BpType type;
	BBox boundingBox;
	ElementCountMap elements;
};

#endif // SMBPHFILE_H_

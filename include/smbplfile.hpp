#ifndef SMBPLFILE_H_
#define SMBPLFILE_H_

#include "vector3.hpp"
#include <vector>
#include <cstdint>

struct SlaveGroup {
	int16_t blockId;
	std::vector<vector3s> positions; // numBlocks size
};

struct ControlPair {
	vector3s masterPosition;
	std::vector<SlaveGroup> slaves; //numGroups size
};

struct ControllerMap {
	int32_t numControllers;
	std::vector<ControlPair> entities; // numControllers size
};

std::istream& operator>>(std::istream& i, ControllerMap& cmap);
std::ostream& operator<<(std::ostream& o, const ControllerMap& cmap);

#endif // SMBPLFILE_H_

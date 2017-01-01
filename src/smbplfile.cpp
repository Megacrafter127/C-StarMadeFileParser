#include "smbplfile.hpp"
#include "io.hpp"

static inline int16_t readCoord(std::istream& i, const bool absPos,
                                const int16_t offset) {
	return absPos ? readFromNetStream<int16_t>(i) : i.get() + offset;
}

namespace {
	template<bool>
	struct wrapper2 {
		static void readSlaveGroup(std::istream& i, SlaveGroup& slave,
		                           const int shift) {
			slave.blockId = readFromNetStream<int16_t>(i);
			const int32_t size = readFromNetStream<int32_t>(i);
			slave.positions.resize(size);
			for(int32_t k = 0 ; k < size ; ++k) {
				slave.positions[k].x = readFromNetStream<int16_t>(i) + shift;
				slave.positions[k].y = readFromNetStream<int16_t>(i) + shift;
				slave.positions[k].z = readFromNetStream<int16_t>(i) + shift;
			}
		}
	};

	template<>
	struct wrapper2<true> {
		static void readSlaveGroup(std::istream& i, SlaveGroup& slave,
		                           const int shift) {
			slave.blockId = readFromNetStream<int16_t>(i);
			const int32_t size = readFromNetStream<int32_t>(i);
			const bool absPosX = i.get();
			const bool absPosY = i.get();
			const bool absPosZ = i.get();
			const int16_t offsetX = readFromNetStream<int16_t>(i);
			const int16_t offsetY = readFromNetStream<int16_t>(i);
			const int16_t offsetZ = readFromNetStream<int16_t>(i);
			for(int32_t k = 0 ; k < size ; ++k) {
				slave.positions[k].x = readCoord(i, absPosX, offsetX) + shift;
				slave.positions[k].y = readCoord(i, absPosY, offsetY) + shift;
				slave.positions[k].z = readCoord(i, absPosZ, offsetZ) + shift;
			}
		}
	};
	
	
	template<bool AcceptRelativeMode>
	struct wrapper {
		static void readControlPair(std::istream& i, ControlPair& pair,
		                            const int shift) {
			pair.masterPosition.x = readFromNetStream<int16_t>(i) + shift;
			pair.masterPosition.y = readFromNetStream<int16_t>(i) + shift;
			pair.masterPosition.z = readFromNetStream<int16_t>(i) + shift;
			const int32_t size = readFromNetStream<int32_t>(i);
			pair.slaves.resize(size);
			for(int32_t k = 0 ; k < size ; ++k)
			wrapper2<AcceptRelativeMode>::
			readSlaveGroup(i, pair.slaves[k], shift);
		}
	};
}

static std::ostream& operator<<(std::ostream& o, const SlaveGroup& slave) {
	writeToNetStream<int16_t>(o, slave.blockId);
	writeToNetStream<int32_t>(o, slave.positions.size());
	for(vector3s v : slave.positions) {
		writeToNetStream<int16_t>(o, v.x);
		writeToNetStream<int16_t>(o, v.y);
		writeToNetStream<int16_t>(o, v.z);
	}
	return o;
}

static std::ostream& operator<<(std::ostream& o, const ControlPair& pair) {
	writeToNetStream<int16_t>(o, pair.masterPosition.x);
	writeToNetStream<int16_t>(o, pair.masterPosition.y);
	writeToNetStream<int16_t>(o, pair.masterPosition.z);
	writeToNetStream<int32_t>(o, pair.slaves.size());
	for(auto& cgroup : pair.slaves)
		o << cgroup;
	return o;
}

std::istream& operator>>(std::istream& i, ControllerMap& cmap) {
	// version unused, ignore the bytes instead
	// int32_t version = readFromNetStream<int32_t>(i);
	i.ignore(4);
	const int32_t mode = readFromNetStream<int32_t>(i);
	const int shift = mode >= -1 ? 8 : 0;
	const bool acceptRelativeMode = mode < 0 && mode >= -1024;
	cmap.numControllers = readFromNetStream<int32_t>(i);
	cmap.entities.resize(cmap.numControllers);
	if(acceptRelativeMode)
		for(int32_t k = 0 ; k < cmap.numControllers ; ++k)
			wrapper<true>::readControlPair(i, cmap.entities[k], shift);
	else
		for(int32_t k = 0 ; k < cmap.numControllers ; ++k)
			wrapper<false>::readControlPair(i, cmap.entities[k], shift);
	return i;
}

std::ostream& operator<<(std::ostream& o, const ControllerMap& cmap) {
	writeToNetStream<int32_t>(o, 0);
	writeToNetStream<int32_t>(o, -1026);
	writeToNetStream<int32_t>(o, cmap.numControllers);
	for(auto& entity : cmap.entities)
		o << entity;
	return o;
}

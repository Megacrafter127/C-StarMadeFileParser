#ifndef SEGMENT_H_
#define SEGMENT_H_

#include <map>
#include <array>
#include <memory>
#include <fstream>

typedef char byte;

enum BlockType {
	CORNER,
	LOGIC,
	STANDARD
};

template<class ScalarT>
struct Vector3 {
	ScalarT x, y, z;
};

typedef std::map<unsigned int, BlockType> blockTypeMap;

struct block {
	unsigned int id:11;
	unsigned int hp:9;
	bool active;
	unsigned int orientation:5;
	
	static block fromRaw(const byte* raw, const blockTypeMap& tmap) {
		block b;
		b.id = raw[2]+((raw[1] & 0x7) << 8);
		auto it = tmap.find(b.id);
		if(it != tmap.end() && it->second == CORNER) {
			b.orientation = (raw[0] & 0xF8) >> 3;
			b.hp = ((raw[0] & 0x7) << 5) + ((raw[1] & 0xF8) >> 3);
		} else {
			b.hp = ((raw[0] & 0xF) << 5) + ((raw[1] & 0xF8) >> 3);
			if(it != tmap.end() && it->second == STANDARD)
				b.orientation = (raw[0] & 0xF0) >> 4;
			else {
				b.orientation = (raw[0] & 0xE0) >> 5;
				b.active = raw[0] & 0x10;
			}
		}
		return b;
	}
};

template<std::size_t N>
class Segment {
	typedef std::array<block, N> row;
	typedef std::array<row, N> plane;
	typedef std::array<plane, N> cube;
public:
	static const std::size_t size = N;
	typedef typename cube::iterator iterator;
	typedef typename cube::const_iterator const_iterator;
	
	Segment(int32_t x, int32_t y, int32_t z, int64_t timestamp,
			std::vector<byte>& buffer):
	content(new cube),
	position{x, y, z},
	timestamp(timestamp) {
		std::ofstream dump("dump.bin",
						   std::ios::trunc|std::ios::out|std::ios::binary);
		dump.write(buffer.data(), buffer.size());
		dump.close();
		//assert(buffer.capacity() > 3*N*N*N)
		byte* ptr = buffer.data();
		for(std::size_t  i = 0 ; i < N ; ++i)
			for(std::size_t j = 0 ; j < N ; ++j)
				for(std::size_t k = 0 ; k < N ; ++k, ptr+=3)
					(*this)[k][j][i] = block::fromRaw(ptr, blockTypeMap());
	}
	
	Segment():
	content(nullptr) {}
	
	Segment(Segment&& other):
	content(std::move(other.content)) {}
	Segment& operator=(Segment&& other) {
		using std::swap;
		if(content.get())
			swap(content, other.content);
		else
			content.reset(other.content.release());
		return *this;
	}
	
	Segment(const Segment&)=delete;
	Segment& operator=(const Segment&)=delete;
	
	operator bool() const {
		return content.get();
	}
	
	block& at(int x, int y, int z) {
		return content.get()->at(x).at(y).at(z);
	}
	
	const block& at(int x, int y, int z) const {
		return content.get()->at(x).at(y).at(z);
	}
	
	plane& operator[](std::size_t i) {
		return (*content.get())[i];
	}
	
	const plane& operator[](std::size_t i) const {
		return (*content.get())[i];
	}
	
	iterator begin() {
		return content.get()->begin();
	}
	
	const_iterator begin() const {
		return content.get()->begin();
	}
	
	const_iterator cbegin() const {
		return content.get()->cbegin();
	}
	
	iterator end() {
		return content.get()->end();
	}
	
	const_iterator end() const {
		return content.get()->end();
	}
	
	const_iterator cend() const {
		return content.get()->cend();
	}
	
private:
	std::unique_ptr<cube> content;
	Vector3<int32_t> position;
	int64_t timestamp;
};

typedef Segment<32> Segment32;
typedef Segment<16> Segment16;

#endif // SEGMENT_H_

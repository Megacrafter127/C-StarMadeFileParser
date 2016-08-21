#ifndef SEGMENT_H_
#define SEGMENT_H_

#include <map>
#include <array>
#include <memory>
#include <vector>

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
	
	static block fromRaw(const byte* raw, const blockTypeMap& tmap);
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
			std::vector<byte>& buffer);
	Segment();
	Segment(Segment&& other);
	Segment& operator=(Segment&& other);
	Segment(const Segment&)=delete;
	Segment& operator=(const Segment&)=delete;
	operator bool() const;
	block& at(int x, int y, int z);
	const block& at(int x, int y, int z) const;
	plane& operator[](std::size_t i);
	const plane& operator[](std::size_t i) const;
	iterator begin();
	const_iterator begin() const;
	const_iterator cbegin() const;
	iterator end();
	const_iterator end() const;
	const_iterator cend() const;
	
private:
	std::unique_ptr<cube> content;
	Vector3<int32_t> position;
	int64_t timestamp;
};

typedef Segment<32> Segment32;
typedef Segment<16> Segment16;

#endif // SEGMENT_H_

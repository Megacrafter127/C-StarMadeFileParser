#ifndef SMD3FILE_H_
#define SMD3FILE_H_

#include "io.hpp"
#include <zlib.h>
#include <vector>
#include <array>
#include <memory>
#include <map>
#include <fstream>

typedef char byte;

namespace smd3 {
	template<class ScalarT>
	struct Vector3 {
		ScalarT x, y, z;
	};
	
	enum BlockType {
		CORNER,
		LOGIC,
		STANDARD
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
				std::vector<byte> buffer):
		content(new cube),
		position{x, y, z},
		timestamp(timestamp) {
			std::ofstream dump("dump.bin",
							   std::ios::trunc|std::ios::out|std::ios::binary);
			dump.write(buffer.data(), buffer.size());
			dump.close();
			//assert(buffer.capacity() > 3*N*N*N)
			byte* ptr = &buffer[0];
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
	
	template<class IStream>
	class Smd3FileReader {
		static const unsigned int SEGMENTS = 4096;
		static const unsigned int SEGSIZE = 49152;
		static const unsigned int HEADSIZE = SEGMENTS*2*sizeof(int16_t);
		typedef typename IStream::pos_type pos_type;
	public:
		Smd3FileReader(const Smd3FileReader&)=delete;
		Smd3FileReader& operator=(const Smd3FileReader&)=delete;
		
		template<class... Args>
		Smd3FileReader(Args&&... args) :
		input(std::forward<Args>(args)...),
		index(SEGMENTS) {
			// doesn't check whether there's indeed something to read
			version = readFromNetStream<uint32_t>(input);
			for(std::size_t i = 0 ; i < SEGMENTS ; ++i) {
				index[i].index = readFromNetStream<int16_t>(input)-1;
				index[i].size = readFromNetStream<int16_t>(input);
			}
		}
		
		Segment32 fetchSegment(int x, int y, int z) {
			pos_type pos = input.tellg();
			const std::size_t idx = infIdx(x, y, z);
			input.seekg(index[idx].index*SEGSIZE+HEADSIZE);
			Segment32 s = nextSegment();
			input.seekg(pos);
			return s;
		}
		
		Segment32 nextSegment() {
			byte unknown;
			pos_type pos = input.tellg();
			input.read(&unknown, 1); // I'm not sure what it's used for
			int64_t timestamp = readFromNetStream<int64_t>(input);
			int32_t x = readFromNetStream<int32_t>(input);
			int32_t y = readFromNetStream<int32_t>(input);
			int32_t z = readFromNetStream<int32_t>(input);
			byte filled;
			input.read(&filled, 1);
			if(input && filled) {
				const std::size_t sz = readFromNetStream<int32_t>(input);
				//buffer.reserve(sz);
				buffer.resize(sz);
				if(!input.read(&buffer[0], sz))
					return Segment32();
				input.seekg(pos+(pos_type)SEGSIZE);
				uLongf size = 32*32*32*3;
				//segBuffer.reserve(size);
				segBuffer.resize(size);
				switch(uncompress(reinterpret_cast<Bytef*>(segBuffer.data()), &size,
								  reinterpret_cast<Bytef*>(buffer.data()), sz)) {
					case Z_OK:
						return Segment32(x,y, z, timestamp, segBuffer);
					case Z_MEM_ERROR:
						// zlib couldn't allocate enough memory
					case Z_BUF_ERROR:
						// the uncompressed data can't fit in the output buffer
					case Z_DATA_ERROR:
						// the compressed data is either uncomplete or corrupted
					default:
						return Segment32();
				}
			}
			return Segment32();
		}
		
		void rewind() {
			input.seekg(HEADSIZE);
		}
		operator bool() const {
			return static_cast<bool>(input);
		}
		
	private:
		static std::size_t infIdx(int x, int y, int z) {
			//may be out of range!
			return (x+8)+(y+8)*16+(z+8)*256;
		}
		
		struct seginfo {
			int16_t index;
			int16_t size;
		};
		
		IStream input;
		int32_t version;
		std::vector<seginfo> index;
		std::vector<byte> buffer, segBuffer;
	};
}
#endif /* SMD3FILE_H */

#ifndef SMD3FILE_H_
#define SMD3FILE_H_

#include "segment.hpp"
#include "io.hpp"
#include <zlib.h>
#include <vector>
#include <map>

namespace smd3 {
	template<class IStream>
	class Smd3FileReader {
		static const unsigned int SEGMENTS = 4096;
		static const unsigned int SEGSIZE = 49152;
		static const unsigned int HEADSIZE = SEGMENTS*2*sizeof(int16_t);
		typedef typename IStream::pos_type pos_type;
	public:
		typedef Segment32 segment_t;
		
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
			input.read(&unknown, 1); // I'm not exactly sure what it's used for
			int64_t timestamp = readFromNetStream<int64_t>(input);
			int32_t x = readFromNetStream<int32_t>(input);
			int32_t y = readFromNetStream<int32_t>(input);
			int32_t z = readFromNetStream<int32_t>(input);
			byte filled;
			input.read(&filled, 1);
			if(input && filled) {
				const std::size_t sz = readFromNetStream<int32_t>(input);
				buffer.resize(sz);
				if(!input.read(&buffer[0], sz))
					return Segment32();
				input.ignore(SEGSIZE - (sz + 26));
				uLongf size = 32*32*32*3;
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

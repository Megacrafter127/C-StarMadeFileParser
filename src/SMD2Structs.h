/*
 * SMD2Structs.h
 *
 *  Created on: 02.02.2016
 *      Author: Megacrafter127
 */

#ifndef SMD2STRUCTS_H_
#define SMD2STRUCTS_H_

#include <string.h>
#include <zlib.h>

namespace smd2{
	enum blockType {
		undefined=0,
		logic,
		rotating,
		fullyRotating
	};
	typedef blockType blocktypeList[2048];
	typedef char rawBlock[3];
	struct block {
		unsigned int id:11;
		unsigned int hp:9;
		bool active;
		unsigned int orientation:5;
		block(const struct block*);
		block(const unsigned int, const unsigned int, const bool, const unsigned int);
		block(const rawBlock*, const blocktypeList*);
		block();
		rawBlock *toRaw(rawBlock*, const blocktypeList*);
	};
	typedef rawBlock rawChunkData[16][16][16]; // note: [Z][Y][X]
	typedef struct block chunkData[16][16][16];
	typedef Bytef compressedChunkData[5094], rawCompressedSegment[5120];
	rawChunkData *inflate(rawChunkData*,const compressedChunkData*);
	compressedChunkData *deflate(compressedChunkData*,const rawChunkData*);
	bool isEmpty(const rawCompressedSegment*);
	struct segmentHead {
		unsigned char unknownChar;
		unsigned long long timestamp;
		signed long x,y,z;
		unsigned char type;
		unsigned long inlen;
		segmentHead(const struct segmentHead*);
		segmentHead(const unsigned char, const unsigned long long, const signed long, const signed long, const signed long, const unsigned char, const unsigned long);
		segmentHead(const rawCompressedSegment*,const bool);
		segmentHead();
		rawCompressedSegment *toRaw(rawCompressedSegment*,const bool);
	};
	struct segment {
		struct segmentHead head;
		chunkData blocks;
		segment(const struct segment*);
		segment(const struct segmentHead*,const chunkData*);
		segment(const struct segmentHead*,const rawChunkData*,
				const blocktypeList*);
		segment(const struct segmentHead*,const compressedChunkData*,
				const blocktypeList*);
		segment(const struct rawSegment*,const blocktypeList*);
		segment(const struct compressedSegment*, const blocktypeList*);
		segment(const rawCompressedSegment*, const blocktypeList*);
		rawCompressedSegment *toRawCompressed(rawCompressedSegment*,const bool);
	};
	struct rawSegment {
		struct segmentHead head;
		rawChunkData blocks;
		rawSegment(const struct rawSegment*);
		rawSegment(const struct segmentHead*,const rawChunkData*);
		rawSegment(const struct segmentHead*,const chunkData*);
		rawSegment(const struct segmentHead*,const compressedChunkData*);
		rawSegment(const struct segment*);
		rawSegment(const struct compressedSegment*);
		rawSegment(const rawCompressedSegment*);
		rawCompressedSegment *toRawCompressed(rawCompressedSegment*,const bool);
	};
	struct compressedSegment {
		struct segmentHead head;
		compressedChunkData blocks;
		compressedSegment(const struct compressedSegment*);
		compressedSegment(const struct segmentHead*,const compressedChunkData*);
		compressedSegment(const struct segmentHead*,const chunkData*);
		compressedSegment(const struct segmentHead*,const rawChunkData*);
		compressedSegment(const struct segment*);
		compressedSegment(const struct rawSegment*);
		compressedSegment(const rawCompressedSegment*);
		rawCompressedSegment *toRawCompressed(rawCompressedSegment*,const bool);
	};
	typedef Bytef rawSmd2Head[65540], rawSmd2Index[8];
	typedef rawCompressedSegment smd2Body[4096];
	struct smd2Index {
				signed long id;
				unsigned long inlen;
				smd2Index(const struct smd2Index*);
				smd2Index(const signed long, const unsigned long);
				smd2Index();
				smd2Index(const rawSmd2Index*);
				rawSmd2Index *toRaw(rawSmd2Index*);
				bool isValid();
	};
	typedef struct smd2Index fullSmd2Index[16][16][16];
	typedef unsigned long long fullSmd2TimestampHead[16][16][16];
	struct smd2Head {
		unsigned long version;
		fullSmd2Index index;
		fullSmd2TimestampHead timestamps;
		smd2Head(const struct smd2Head*);
		smd2Head(const unsigned long, const fullSmd2Index*, const fullSmd2TimestampHead*);
		smd2Head(const rawSmd2Head*);
		rawSmd2Head *toRaw(rawSmd2Head*);
	};
	unsigned int getSegmentSlotCountFromSMD2Size(const size_t);
	size_t getSMD2SizeFromSegmentSlotCount(const unsigned int);
	
} // end smd2
#endif /* SMD2STRUCTS_H_ */

/*
 * SMD2Structs.cpp
 *
 *  Created on: 09.02.2016
 *      Author: Megacrafter127
 */

#include "SMD2Structs.h"
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <arpa/inet.h>
#include <zlib.h>

namespace smd2{
	block::block(const struct block *copy) {}; //TODO
	block::block(const unsigned int id, const unsigned int hp, const bool active, const unsigned int orientation) {}; //TODO
	block::block(const rawBlock *raw) {}; //TODO
	block::block() {
		this->id=0;
		this->hp=0;
		this->active=false;
		this->orientation=0;
	};
	rawBlock *block::toRaw(rawBlock *target) {}; //TODO
	
	rawChunkData *inflate(rawChunkData *trg,const compressedChunkData *src) {}; //TODO
	compressedChunkData *deflate(compressedChunkData *trg,const rawChunkData *src) {}; //TODO
	bool isEmpty(const rawCompressedSegment *segment) {}; //TODO
	
	segmentHead::segmentHead(const struct segmentHead *copy) {
		memcpy(this,copy,sizeof(struct segmentHead));
	};
	segmentHead::segmentHead(const unsigned char unknownChar, const unsigned long long timestamp, const signed long x, const signed long y, const signed long z, const unsigned char type, const unsigned long inlen) {
		this->unknownChar=unknownChar;
		this->timestamp=timestamp;
		this->x=x;
		this->y=y;
		this->z=z;
		this->type=type;
		this->inlen=inlen;
	};
	segmentHead::segmentHead(const rawCompressedSegment *raw,const bool offset) {}; //TODO
	segmentHead::segmentHead() {
		this->unknownChar=0;
		this->timestamp=0;
		this->x=0;
		this->y=0;
		this->z=0;
		this->type=0;
		this->inlen=0;
	};
	rawCompressedSegment *segmentHead::toRaw(rawCompressedSegment*,const bool offset) {}; //TODO

	segment::segment(const struct segment *copy) {}; //TODO
	segment::segment(const struct segmentHead *head,const chunkData *blocks) {}; //TODO
	segment::segment(const struct segmentHead *head,const rawChunkData *blocks) {}; //TODO
	segment::segment(const struct segmentHead *head,const compressedChunkData *blocks) {}; //TODO
	segment::segment(const struct rawSegment *src) {}; //TODO
	segment::segment(const struct compressedSegment *src) {}; //TODO
	segment::segment(const rawCompressedSegment *src) {}; //TODO
	rawCompressedSegment *segment::toRawCompressed(rawCompressedSegment *trg,const bool offset) {}; //TODO
			
	rawSegment::rawSegment(const struct rawSegment *copy) {}; //TODO
	rawSegment::rawSegment(const struct segmentHead *head,const rawChunkData*) {}; //TODO
	rawSegment::rawSegment(const struct segmentHead *head,const chunkData*) {}; //TODO
	rawSegment::rawSegment(const struct segmentHead *head,const compressedChunkData*) {}; //TODO
	rawSegment::rawSegment(const struct segment *src) {}; //TODO
	rawSegment::rawSegment(const struct compressedSegment *src) {}; //TODO
	rawSegment::rawSegment(const rawCompressedSegment *src) {}; //TODO
	rawCompressedSegment *rawSegment::toRawCompressed(rawCompressedSegment *trg,const bool offset) {}; //TODO
					
	compressedSegment::compressedSegment(const struct compressedSegment *copy) {}; //TODO
	compressedSegment::compressedSegment(const struct segmentHead *head,const compressedChunkData *blocks) {}; //TODO
	compressedSegment::compressedSegment(const struct segmentHead *head,const chunkData *blocks) {}; //TODO
	compressedSegment::compressedSegment(const struct segmentHead *head,const rawChunkData *blocks) {}; //TODO
	compressedSegment::compressedSegment(const struct segment *src) {}; //TODO
	compressedSegment::compressedSegment(const struct rawSegment *src) {}; //TODO
	compressedSegment::compressedSegment(const rawCompressedSegment *src) {}; //TODO
	rawCompressedSegment *compressedSegment::toRawCompressed(rawCompressedSegment *trg,const bool offset) {}; //TODO
	
	
	smd2Index::smd2Index(const struct smd2Index *copy) {
		this->id=copy->id;
		this->inlen=copy->inlen;
	};
	smd2Index::smd2Index(const signed long id, const unsigned long len) {
		this->id=id;
		this->inlen=len;
	};
	smd2Index::smd2Index() {
		this->id=-1;
		this->inlen=0;
	};
	smd2Index::smd2Index(const rawSmd2Index *raw) {
		uint32_t split[2];
		memcpy(split,raw,sizeof(rawSmd2Index));
		this->id=ntohl(split[0]);
		this->inlen=ntohl(split[1]);
	};
	rawSmd2Index *smd2Index::toRaw(rawSmd2Index *trg) {
		uint32_t split[2];
		split[0]=htonl(this->id);
		split[1]=htonl(this->inlen);
		memcpy(trg,split,sizeof(rawSmd2Index));
		return trg;
	};
	bool smd2Index::isValid() {
		return this->id!=-1;
	};
	
	smd2Head::smd2Head(const struct smd2Head*) {}; //TODO
	smd2Head::smd2Head(const unsigned long version, const fullSmd2Index *index,const fullSmd2TimestampHead *timestamps) {
		this->version=version;
		memcpy(&(this->index),index,sizeof(fullSmd2Index));
		memcpy(&(this->timestamps),timestamps,sizeof(fullSmd2TimestampHead));
	};
	smd2Head::smd2Head(const rawSmd2Head*) {}; //TODO
	rawSmd2Head *smd2Head::toRaw(rawSmd2Head*) {}; //TODO
	
	unsigned int getSegmentSlotCountFromSMD2Size(const size_t) {}; //TODO
	size_t getSMD2SizeFromSegmentSlotCount(const unsigned int) {}; //TODO
}



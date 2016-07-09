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
#include <stdexcept>

namespace smd2{
	block::block(const struct block *copy) {
		memcpy(this,copy,sizeof(struct block));
	};
	block::block(const unsigned int id, const unsigned int hp, const bool active, const unsigned int orientation) {
		this->id=id;
		this->hp=hp;
		this->active=active;
		this->orientation=orientation;
	};
	block::block(const rawBlock *raw, const blocktypeList *list) {
		if(!list) std::__throw_invalid_argument("the provided list may not be NULL");
		uint32_t buff=0;
		memcpy(&buff+sizeof(uint32_t)-sizeof(rawBlock),raw,sizeof(rawBlock));
		if(buff) {
			buff=ntohl(buff);
			this->id=buff;
			buff>>=11;
			blockType type=((blockType*)list)[this->id];
			if(type==undefined) {
				std::__throw_invalid_argument("blocktype not defined");
			}
			this->hp=buff;
			if(type==fullyRotating) {
				this->hp&=0x00FF;
				buff>>=8;
			} else buff>>=9;
			if(type==logic) {
				this->active=buff & 1;
				buff>>=1;
			}
			this->orientation=buff;
		} else {
			this->id=0;
			this->hp=0;
			this->active=false;
			this->orientation=0;
		}
	};
	block::block() {
		this->id=0;
		this->hp=0;
		this->active=false;
		this->orientation=0;
	};
	rawBlock *block::toRaw(rawBlock *target, const blocktypeList *list) {
		if(!list) std::__throw_invalid_argument("the provided list may not be NULL");
		blockType type=((blockType*)list)[this->id];
		if(type==undefined) {
			std::__throw_invalid_argument("blocktype not defined");
		}
		uint32_t buff=this->orientation;
		if(type==logic) {
			buff<<=1;
			if(this->active) buff++;
		}
		if(type==fullyRotating) {
			buff<<=8;
			buff+=(0xFF & this->hp);
		} else {
			buff<<=9;
			buff+=this->hp;
		}
		buff<<=11;
		buff+=id;
		buff=htonl(buff);
		memcpy(target,&buff+sizeof(uint32_t)-sizeof(rawBlock),sizeof(rawBlock));
		return target;
	};
	
	rawChunkData *inflate(rawChunkData *trg,const compressedChunkData *src) {}; //TODO #31
	compressedChunkData *deflate(compressedChunkData *trg,const rawChunkData *src) {}; //TODO #32
	bool isEmpty(const rawCompressedSegment *segment) {
		size_t *chars=(size_t*)segment;
		for(size_t i=0;i<(sizeof(rawCompressedSegment)/sizeof(size_t));i++) {
			if(chars[i]) return false;
		}
		return true;
	};
	
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
	segmentHead::segmentHead(const rawCompressedSegment *raw,const bool offset) {
		size_t i=0;
		char *chars=(char*)raw;
		if(offset) {
			this->unknownChar=chars[i];
			i++;
		}
		uint64_t dbuff64;
		uint32_t *buff32=(uint32_t*)&dbuff64,dbuff32=ntohl(buff32[0]);
		memcpy(buff32,&(chars[i]),sizeof(uint64_t));
		if(ntohl(1)!=1) {
			buff32[0]=ntohl(buff32[1]);
			buff32[1]=dbuff32;
		}
		this->timestamp=dbuff64;
		i+=sizeof(uint64_t);
		
		memcpy(&dbuff32,&(chars[i]),sizeof(int32_t));
		this->x=ntohl(dbuff32);
		i+=sizeof(int32_t);
		
		memcpy(&dbuff32,&(chars[i]),sizeof(int32_t));
		this->y=ntohl(dbuff32);
		i+=sizeof(int32_t);
		
		memcpy(&dbuff32,&(chars[i]),sizeof(int32_t));
		this->z=ntohl(dbuff32);
		i+=sizeof(int32_t);
		
		this->type=chars[i];
		i++;
		
		memcpy(&dbuff32,&(chars[i]),sizeof(uint32_t));
		this->inlen=ntohl(dbuff32);
	};
	segmentHead::segmentHead() {
		this->unknownChar=0;
		this->timestamp=0;
		this->x=0;
		this->y=0;
		this->z=0;
		this->type=0;
		this->inlen=0;
	};
	rawCompressedSegment *segmentHead::toRaw(rawCompressedSegment *target,const bool offset) {
		size_t i=0;
		char *chars=(char*)target;
		if(offset) {
			chars[i]=this->unknownChar;
			i++;
		}
		uint64_t dbuff64=this->timestamp;
		uint32_t *buff32=(uint32_t*)&dbuff64,dbuff32=htonl(buff32[0]);
		if(htonl(1)!=1) {
			buff32[0]=htonl(buff32[1]);
			buff32[1]=dbuff32;
		}
		memcpy(&(chars[i]),buff32,sizeof(uint64_t));
		i+=sizeof(uint64_t);
		
		dbuff32=htonl(this->x);
		memcpy(&(chars[i]),&dbuff32,sizeof(int32_t));
		i+=sizeof(int32_t);
		
		dbuff32=htonl(this->y);
		memcpy(&(chars[i]),&dbuff32,sizeof(int32_t));
		i+=sizeof(int32_t);
		
		dbuff32=htonl(this->z);
		memcpy(&(chars[i]),&dbuff32,sizeof(int32_t));
		i+=sizeof(int32_t);
		
		this->type=chars[i];
		i++;
		
		dbuff32=htonl(this->inlen);
		memcpy(&(chars[i]),&dbuff32,sizeof(uint32_t));
		return target;
	};
	
	segment::segment(const struct segment *copy) {}; //TODO #7
	segment::segment(const struct segmentHead *head,const chunkData *blocks) {}; //TODO #8
	segment::segment(const struct segmentHead *head,const rawChunkData *blocks) {}; //TODO #9
	segment::segment(const struct segmentHead *head,const compressedChunkData *blocks) {}; //TODO #10
	segment::segment(const struct rawSegment *src) {}; //TODO #9
	segment::segment(const struct compressedSegment *src) {}; //TODO #10
	segment::segment(const rawCompressedSegment *src) {}; //TODO #11
	rawCompressedSegment *segment::toRawCompressed(rawCompressedSegment *trg,const bool offset) {}; //TODO #12
	
	rawSegment::rawSegment(const struct rawSegment *copy) {}; //TODO #13
	rawSegment::rawSegment(const struct segmentHead *head,const rawChunkData*) {}; //TODO #14
	rawSegment::rawSegment(const struct segmentHead *head,const chunkData*) {}; //TODO #15
	rawSegment::rawSegment(const struct segmentHead *head,const compressedChunkData*) {}; //TODO #16
	rawSegment::rawSegment(const struct segment *src) {}; //TODO #15
	rawSegment::rawSegment(const struct compressedSegment *src) {}; //TODO #16
	rawSegment::rawSegment(const rawCompressedSegment *src) {}; //TODO #17
	rawCompressedSegment *rawSegment::toRawCompressed(rawCompressedSegment *trg,const bool offset) {}; //TODO #18
	
	compressedSegment::compressedSegment(const struct compressedSegment *copy) {}; //TODO #19
	compressedSegment::compressedSegment(const struct segmentHead *head,const compressedChunkData *blocks) {}; //TODO #20
	compressedSegment::compressedSegment(const struct segmentHead *head,const chunkData *blocks) {}; //TODO #21
	compressedSegment::compressedSegment(const struct segmentHead *head,const rawChunkData *blocks) {}; //TODO #22
	compressedSegment::compressedSegment(const struct segment *src) {}; //TODO #21
	compressedSegment::compressedSegment(const struct rawSegment *src) {}; //TODO #22
	compressedSegment::compressedSegment(const rawCompressedSegment *src) {}; //TODO #23
	rawCompressedSegment *compressedSegment::toRawCompressed(rawCompressedSegment *trg,const bool offset) {}; //TODO #24
	
	
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
	
	smd2Head::smd2Head(const struct smd2Head*) {}; //TODO #25
	smd2Head::smd2Head(const unsigned long version, const fullSmd2Index *index,const fullSmd2TimestampHead *timestamps) {
		this->version=version;
		memcpy(&(this->index),index,sizeof(fullSmd2Index));
		memcpy(&(this->timestamps),timestamps,sizeof(fullSmd2TimestampHead));
	};
	smd2Head::smd2Head(const rawSmd2Head*) {}; //TODO #26
	rawSmd2Head *smd2Head::toRaw(rawSmd2Head*) {}; //TODO #27
	
	unsigned int getSegmentSlotCountFromSMD2Size(const size_t) {}; //TODO #29
	size_t getSMD2SizeFromSegmentSlotCount(const unsigned int) {}; //TODO #30
}



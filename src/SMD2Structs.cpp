/*
 * SMD2Structs.cpp
 *
 *  Created on: 09.02.2016
 *      Author: Megacrafter127
 */

#include "SMD2Structs.h"
#include <string.h>

namespace smd2{
	blockV0::blockV0(union blockV0n arg) {
		this->bigEndian=arg;
	};
	blockV0::blockV0(union blockV0f arg) {
		this->littleEndian=arg;
	};

	segmentHeader::segmentHeader(uint64_t& timestamp, int32_t& x, int32_t& y, int32_t& z, int8_t type, int32_t& inlen) {
		this->timestamp=timestamp;
		this->x=x;
		this->y=y;
		this->z=z;
		this->type=type;
		this->inlen=inlen;
	};
	segmentHeader::segmentHeader() {
		this->timestamp=0;
		this->x=0;
		this->y=0;
		this->z=0;
		this->type=0;
		this->inlen=0;
	};
	segmentV0::operator segmentV1() {
		return segmentV1(0,this->head,(char*)&this->deflated);
	};
	segmentV1::operator segmentV0() {
		return segmentV0(this->head,(char*)&this->deflated);
	};
	segmentV0::segmentV0(struct segmentHeader& head,char* deflated) {
		this->head=head;
		for(size_t i=0;i<5103;i++) {
			this->deflated[i]=deflated[i];
		}
	};
	segmentV1::segmentV1(uint8_t unknown,struct segmentHeader& head,char* deflated) {
		this->unknown=unknown;
		this->head=head;
		for(size_t i=0;i<5102;i++) {
			this->deflated[i]=deflated[i];
		}
	};
	smd2Head::segmentIndex::segmentIndex(int32_t id,int32_t len) {
		this->segmentID=id;
		this->segmentLen=len;
	};
	smd2Head::segmentIndex::segmentIndex() {
		this->segmentID=-1;
		this->segmentLen=0;
	};
	smd2Head::smd2Head(int32_t unknown,struct smd2Head::segmentIndex indizes[16][16][16],int64_t timestamps[16][16][16]) {
		this->unknownInt=unknown;
		for(int i=0;i<16;i++) for(int j=0;j<16;j++) for(int k=0;k<16;k++) {
			this->indizes[i][j][k]=indizes[i][j][k];
			this->timestamps[i][j][k]=timestamps[i][j][k];
		}
	};
	smd2Head::smd2Head() {
		this->unknownInt=0;
		for(int i=0;i<16;i++) for(int j=0;j<16;j++) for(int k=0;k<16;k++) {
			this->indizes[i][j][k]=smd2Head::segmentIndex();
			this->timestamps[i][j][k]=0;
		}
	};
}



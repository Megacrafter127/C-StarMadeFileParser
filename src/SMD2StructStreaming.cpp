/*
 * SMD2StructStreaming.cpp
 *
 *  Created on: 12.02.2016
 *      Author: Megacrafter127
 */
#include "SMD2StructStreaming.h"
#include <iohelp.h>
#include <arpa/inet.h>

using namespace std;

namespace smd2{
	void readIndex(struct smd2Head::segmentIndex* index,istream& in) {
		index->segmentID=readInt(in);
		index->segmentLen=readInt(in);
	}
	inline void readIndex(struct smd2Head::segmentIndex* index,istream* in) {
		readIndex(index,*in);
	}
	
	void writeIndex(struct smd2Head::segmentIndex& index,ostream& out) {
		writeInt(out,index.segmentID);
		writeInt(out,index.segmentLen);
	}
	inline void writeIndex(struct smd2Head::segmentIndex& index,ostream* out) {
		writeIndex(index,*out);
	}
	inline void writeIndex(struct smd2Head::segmentIndex* index,ostream& out) {
		writeIndex(*index,out);
	}
	inline void writeIndex(struct smd2Head::segmentIndex* index,ostream* out) {
		writeIndex(*index,*out);
	}
	
	void readHead(struct smd2Head* head,istream& in){
		head->unknownInt=readInt(in);
		for(int i=0;i<16;i++) for(int j=0;j<16;j++) for(int k=0;k<16;k++) {
			readIndex(&(head->indizes[i][j][k]),in);
		}
		for(int i=0;i<16;i++) for(int j=0;j<16;j++) for(int k=0;k<16;k++) {
			head->timestamps[i][j][k]=readLong(in);
		}
		
	}
	inline void readHead(struct smd2Head* head,istream* in){
		readHead(head,*in);
	}
	
	void writeHead(struct smd2Head& head,ostream& out) {
		writeInt(out,head.unknownInt);
		for(int i=0;i<16;i++) for(int j=0;j<16;j++) for(int k=0;k<16;k++) {
			writeIndex(head.indizes[i][j][k],out);
		}
		for(int i=0;i<16;i++) for(int j=0;j<16;j++) for(int k=0;k<16;k++) {
			writeLong(out,head.timestamps[i][j][k]);
		}
	}
	inline void writeHead(struct smd2Head* head,ostream& out) {
		writeHead(*head,out);
	}
	inline void writeHead(struct smd2Head& head,ostream* out) {
		writeHead(head,*out);
	}
	inline void writeHead(struct smd2Head* head,ostream* out) {
		writeHead(*head,*out);
	}
	
	void readSegmentHeader(struct segmentHeader* head,istream& in) {
		head->timestamp=readLong(in);
		head->x=readInt(in);
		head->y=readInt(in);
		head->z=readInt(in);
		head->type=readByte(in);
		head->inlen=readInt(in);
	}
	inline void readSegmentHeader(struct segmentHeader* head,istream* in) {
		readSegmentHeader(head,*in);
	}
	
	void writeSegmentHeader(struct segmentHeader& head,ostream& out) {
		writeLong(out,head.timestamp);
		writeInt(out,head.x);
		writeInt(out,head.y);
		writeInt(out,head.z);
		writeByte(out,head.type);
		writeInt(out,head.inlen);
	}
	inline void writeSegmentHeader(struct segmentHeader* head,ostream& out) {
		writeSegmentHeader(*head,out);
	}
	inline void writeSegmentHeader(struct segmentHeader& head,ostream* out) {
		writeSegmentHeader(head,*out);
	}
	inline void writeSegmentHeader(struct segmentHeader* head,ostream* out) {
		writeSegmentHeader(*head,*out);
	}
	
	void readSegmentV0(struct segmentV0* segment,istream& in) {
		readSegmentHeader(&segment->head,in);
		in.read(segment->deflated,SEGMENT0MAXINL);
	}
	inline void readSegmentV0(struct segmentV0* segment,istream* in) {
		readSegmentV0(segment,*in);
	}
	
	void writeSegmentV0(struct segmentV0& segment,ostream& out) {
		writeSegmentHeader(segment.head,out);
		out.write(segment.deflated,SEGMENT0MAXINL);
	}
	inline void writeSegmentV0(struct segmentV0* segment,ostream& out) {
		writeSegmentV0(*segment,out);
	}
	inline void writeSegmentV0(struct segmentV0& segment,ostream* out) {
		writeSegmentV0(segment,*out);
	}
	inline void writeSegmentV0(struct segmentV0* segment,ostream* out) {
		writeSegmentV0(*segment,*out);
	}
	
	
	void readSegmentV1(struct segmentV1* segment,istream& in) {
		segment->unknown=readByte(in);
		readSegmentHeader(&segment->head,in);
		in.read(segment->deflated,SEGMENT1MAXINL);
	}
	inline void readSegmentV1(struct segmentV1* segment,istream* in) {
		readSegmentV1(segment,*in);
	}
	
	void writeSegmentV1(struct segmentV1& segment,ostream& out) {
		writeByte(out,segment.unknown);
		writeSegmentHeader(&segment.head,out);
		out.write(segment.deflated,SEGMENT1MAXINL);
	}
	inline void writeSegmentV1(struct segmentV1* segment,ostream& out) {
		writeSegmentV1(*segment,out);
	}
	inline void writeSegmentV1(struct segmentV1& segment,ostream* out) {
		writeSegmentV1(segment,*out);
	}
	inline void writeSegmentV1(struct segmentV1* segment,ostream* out) {
		writeSegmentV1(*segment,*out);
	}
}



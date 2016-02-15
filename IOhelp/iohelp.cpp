/*
 * iohelp.cpp
 *
 *  Created on: 12.02.2016
 *      Author: Megacrafter127
 */

#include "iohelp.h"
#include <arpa/inet.h>
uint64_t ntohll(uint64_t arg) {
	uint32_t *ar,c;
	ar=(uint32_t*)&arg;
	c=ntohl(*ar);
	if(c==*ar) return arg;
	*ar=ntohl(ar[1]);
	ar[1]=c;
	return arg;
}
uint64_t htonll(uint64_t arg) {
	uint32_t *ar,c;
	ar=(uint32_t*)&arg;
	c=htonl(*ar);
	if(c==*ar) return arg;
	*ar=htonl(ar[1]);
	ar[1]=c;
	return arg;
}

namespace std {
	union word {
		uint16_t val;
		char raw[2];
	};
	union longWord {
		uint32_t val;
		char raw[4];
	};
	union longLongWord {
		uint64_t val;
		char raw[8];
	};
	
	uint8_t readByte(istream& in) {
		uint8_t ret;
		in.read((char*)&ret,1);
		return ret;
	}
	uint8_t readByte(istream* in) {
		return readByte(*in);
	}
	uint16_t readShort(istream& in) {
		union word s;
		in.read(s.raw,2);
		return ntohs(s.val);
	}
	uint16_t readShort(istream* in) {
		return readShort(*in);
	}
	uint32_t readInt(istream& in) {
		union longWord s;
		in.read(s.raw,4);
		return ntohl(s.val);
	}
	uint32_t readInt(istream* in) {
		return readInt(*in);
	}
	uint64_t readLong(istream& in) {
		union longLongWord s;
		in.read(s.raw,8);
		return ntohll(s.val);
	}
	uint64_t readLong(istream* in) {
		return readLong(*in);
	}
	void writeByte(ostream& out,uint8_t value) {
		out.write((char*)&value,1);
	}
	void writeByte(ostream* out,uint8_t value) {
		writeByte(*out,value);
	}
	void writeShort(ostream& out,uint16_t value) {
		union word s;
		s.val=htons(value);
		out.write(s.raw,2);
	}
	void writeShort(ostream* out,uint16_t value) {
		writeShort(*out,value);
	}
	void writeInt(ostream& out,uint32_t value) {
		union longWord s;
		s.val=htonl(value);
		out.write(s.raw,4);
	}
	void writeInt(ostream* out,uint32_t value) {
		writeInt(*out,value);
	}
	void writeLong(ostream& out,uint64_t value) {
		union longLongWord s;
		s.val=htonll(value);
		out.write(s.raw,8);
	}
	void writeLong(ostream* out,uint64_t value) {
		writeLong(*out,value);
	}
}


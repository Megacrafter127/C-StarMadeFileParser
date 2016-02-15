/*
 * iohelp.hpp
 *
 *  Created on: 12.02.2016
 *      Author: Megacrafter127
 */

#ifndef IOHELP_HPP_
#define IOHELP_HPP_

#include <stdint.h>
#include <iostream>

uint64_t ntohll(uint64_t);
uint64_t htonll(uint64_t);

namespace std{
	uint8_t readByte(istream&);
	uint8_t readByte(istream*);
	uint16_t readShort(istream&);
	uint16_t readShort(istream*);
	uint32_t readInt(istream&);
	uint32_t readInt(istream*);
	uint64_t readLong(istream&);
	uint64_t readLong(istream*);
	void writeByte(ostream&,uint8_t);
	void writeByte(ostream*,uint8_t);
	void writeShort(ostream&,uint16_t);
	void writeShort(ostream*,uint16_t);
	void writeInt(ostream&,uint32_t);
	void writeInt(ostream*,uint32_t);
	void writeLong(ostream&,uint64_t);
	void writeLong(ostream*,uint64_t);
}


#endif /* IOHELP_HPP_ */

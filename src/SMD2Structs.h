/*
 * SMD2Structs.hpp
 *
 *  Created on: 02.02.2016
 *      Author: Megacrafter127
 */

#ifndef SMD2STRUCTS_HPP_
#define SMD2STRUCTS_HPP_

#include <stdint.h>
#include <string.h>

namespace smd2{
	union blockV0 {
		union blockV0n {
			struct lgc {
				int id : 11;
				int hp : 9;
				bool active : 1;
				int orientation : 3;
			} logic;
			struct nrm {
				int id : 11;
				int hp : 9;
				int orientation : 4;
			} normal;
			struct fRot {
				int id : 11;
				int hp : 8;
				int orientation : 5;
			} fullRotation; //for things like rails, corners and similar blocks
			uint8_t raw[3];
		} bigEndian; //sizeof(union blockV0) == 3
		blockV0(union blockV0n);

		union blockV0f{
			struct lgc {
				int orientation : 3;
				bool active : 1;
				int hp : 9;
				int id : 11;
			} logic;
			struct nrm {
				int orientation : 4;
				int hp : 9;
				int id : 11;
			} normal;
			struct fRot {
				int orientation : 5;
				int hp : 8;
				int id : 11;
			} fullRotation; //for things like rails, corners and similar blocks
			uint8_t raw[3];
		} littleEndian;
		blockV0(union blockV0f);

	};
	
	typedef union blockV0 segmentBlockDataV0[16][16][16];
	
	void inflate(segmentBlockDataV0*,char*,size_t);
	bool deflate(char*,size_t,segmentBlockDataV0*);
	
	struct segmentHeader{
		segmentHeader();
		segmentHeader(uint64_t&, int32_t&, int32_t&, int32_t&, int8_t, int32_t&);
		int64_t timestamp; //when the segment was last edited or created
		int32_t x,y,z; //offset of this segment
		int8_t type; //unknown
		int32_t inlen; //length of the deflated data without padding.
	};

	struct segmentV0 {
		segmentV0();
		segmentV0(struct segmentHeader&,char*);
		struct segmentHeader head;
#define SEGMENT0MAXINL 5103
		char deflated[5103]; //deflated segmentBlockData. 0-padded.
		operator struct segmentV1();
	};

	struct segmentV1 {
		segmentV1();
		segmentV1(uint8_t,struct segmentHeader&,char*);
		uint8_t unknown;
		struct segmentHeader head;
#define SEGMENT1MAXINL 5102
		char deflated[5102]; //deflated segmentBlockData. 0-padded.
		operator struct segmentV0();
	}; //sizeof(struct segmentV0) == sizeof(struct segmentV1)

	struct smd2Head {
		int32_t unknownInt; //unknown effects, likely version number of the format. If it is 0, only segmentV0s will occur.
		struct segmentIndex {
			int32_t segmentID; //the index of the segment
			int32_t segmentLen; //same as the inlen field in segmentHeader, except it includes the length of the header
			segmentIndex();
			segmentIndex(int32_t,int32_t);
		} indizes[16][16][16];
		int64_t timestamps[16][16][16]; //same as the timestamp field in the corrosponding segment.
		smd2Head(int32_t,struct segmentIndex[16][16][16],int64_t[16][16][16]);
		smd2Head();
	};
	
} // end smd2
#endif /* SMD2STRUCTS_HPP_ */

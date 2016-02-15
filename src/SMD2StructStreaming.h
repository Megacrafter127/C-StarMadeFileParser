/*
 * SMD2StructStreaming.hpp
 *
 *  Created on: 12.02.2016
 *      Author: Megacrafter127
 */

#ifndef SMD2STRUCTSTREAMING_HPP_
#define SMD2STRUCTSTREAMING_HPP_

#include "SMD2Structs.h"
#include <iostream>

using namespace std;

namespace smd2{
	void readIndex(struct smd2Head::segmentIndex*,istream&);
	inline void readIndex(struct smd2Head::segmentIndex*,istream*);
	
	void writeIndex(struct smd2Head::segmentIndex&,ostream&);
	inline void writeIndex(struct smd2Head::segmentIndex&,ostream*);
	inline void writeIndex(struct smd2Head::segmentIndex*,ostream&);
	inline void writeIndex(struct smd2Head::segmentIndex*,ostream*);
	
	void readHead(struct smd2Head*,istream&);
	inline void readHead(struct smd2Head*,istream*);
	
	inline void writeHead(struct smd2Head*,ostream&);
	void writeHead(struct smd2Head&,ostream&);
	inline void writeHead(struct smd2Head*,ostream*);
	inline void writeHead(struct smd2Head&,ostream*);
	
	void readSegmentHeader(struct segmentHeader*,istream&);
	inline void readSegmentHeader(struct segmentHeader*,istream*);
	
	inline void writeSegmentHeader(struct segmentHeader*,ostream&);
	void writeSegmentHeader(struct segmentHeader&,ostream&);
	inline void writeSegmentHeader(struct segmentHeader*,ostream*);
	inline void writeSegmentHeader(struct segmentHeader&,ostream*);
	
	void readSegmentV0(struct segmentV0*,istream&);
	inline void readSegmentV0(struct segmentV0*,istream*);
	
	inline void writeSegmentV0(struct segmentV0*,ostream&);
	void writeSegmentV0(struct segmentV0&,ostream&);
	inline void writeSegmentV0(struct segmentV0*,ostream*);
	inline void writeSegmentV0(struct segmentV0&,ostream*);
	
	
	void readSegmentV1(struct segmentV1*,istream&);
	inline void readSegmentV1(struct segmentV1*,istream*);
	
	inline void writeSegmentV1(struct segmentV1*,ostream&);
	void writeSegmentV1(struct segmentV1&,ostream&);
	inline void writeSegmentV1(struct segmentV1*,ostream*);
	inline void writeSegmentV1(struct segmentV1&,ostream*);
}

#endif /* SMD2STRUCTSTREAMING_HPP_ */

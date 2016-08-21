/*
 * SMD2Structs.cpp
 *
 *  Created on: 08.02.2016
 *      Author: Megacrafter127
 */

#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include "SMD2Structs.h"
#include "smd3file.hpp"

using namespace std;

unsigned char f(unsigned int id) {
	if(id==1)
		return 0;
	return 255-(id*255/977);
}

int main(int argc,char* argv[]) {
	if(argc != 2) {
		std::cerr << "usage ./[exec] filename\n";
		return 0;
	}
	smd3::Smd3FileReader<std::ifstream> file(argv[1]);
	smd3::Segment32 s;
	if(file)
		s = file.nextSegment();
	std::ofstream imgtest("img.ppm",
						  std::ios::trunc | std::ios::out | std::ios::binary);
	imgtest << "P6\n32 1056\n255\n";
	if(s)
		for(auto& plane : s) {
			for(auto& row : plane) {
				for(smd3::block b : row)
					imgtest << f(b.id) << f(b.id) << f(b.id);
			}
			for(int i = 0;i<32;++i)
				imgtest << (char) 255 << (char) 0 << (char) 0;
		}
	return 0;
}


#include "smbplfile.hpp"
#include <fstream>
#include <iostream>

int main(int argc,char* argv[]) {
	if(argc != 2) {
		std::cerr << "usage ./[exec] filename\n";
		return 0;
	}
	std::ifstream file(argv[1], std::ios::binary | std::ios::in);
	ControllerMap test;
	file >> test;
	file.close();
	std::ofstream output("dup.smbpl",
	                     std::ios::trunc | std::ios::binary | std::ios::out);
	output << test;
	output.close();
	return 0;
}


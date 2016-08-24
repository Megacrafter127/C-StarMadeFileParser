#include "smentfile.hpp"

int main(int argc,char* argv[]) {
	if(argc != 2) {
		std::cerr << "usage ./[exec] filename\n";
		return 0;
	}
	SmentFileReader r(argv[1]);
	return 0;
}


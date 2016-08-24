#ifndef SMENTFILE_H_
#define SMENTFILE_H_

#include <string>
#include <zip.h>
#include <iostream>

class SmentFileReader {
public:
	
	SmentFileReader(const char* path) {
		int errorp;
		archive = zip_open(path, ZIP_RDONLY, &errorp);
		if(!archive) {
			switch(errorp) {
				//case ZIP_ER_EXISTS: // ZIP_EXCL unset
				case ZIP_ER_INCONS: // inconsistencies in archive
				case ZIP_ER_INVAL: // null path argument
				case ZIP_ER_MEMORY: // couldn't allocate memory
				case ZIP_ER_NOENT: // zip archive not found
				case ZIP_ER_NOZIP: // not a zip archive
				case ZIP_ER_OPEN: // couldn't open file
				case ZIP_ER_READ: // read error, c.f. errno
				case ZIP_ER_SEEK: // opened file doesn't allow seeks
				default:
					break;
			}
		} else {
			int64_t entries = zip_get_num_entries(archive, 0);
			struct zip_stat st;
			for(int64_t i = 0 ; i < entries
			                    && zip_stat_index(archive, i, 0, &st) == 0; ++i)
				if(st.valid & ZIP_STAT_NAME)
					std::clog << st.name << '\n';
			zip_close(archive);
			archive = nullptr;
		}
	}
	
	SmentFileReader(std::string path) : SmentFileReader(path.c_str()) {};
private:
	zip_t* archive;
	//int flags;
};

#endif

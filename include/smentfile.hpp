#ifndef SMENTFILE_H_
#define SMENTFILE_H_

#include <vector3.hpp>
#include <zip.h>
#include <vector>
#include <map>
#include <streambuf>
#include <string>
#include <cstring>
#include <iostream>

struct EntityFiles {
	std::vector<EntityFiles> dockedEntities;
	std::map<vector3i, int64_t> regions;
	int64_t headerIdx;
	int64_t logicIdx;
	int64_t metaIdx;
};

enum class fext {
	SMD,
	SMD2,
	SMD3,
	SMBPH,
	SMBPL,
	SMBPM,
	NONE
};

vector3i region(const std::string& path) {
	vector3i v;
	std::size_t i = path.rfind('.') - 1;
	const char * ptr = path.c_str() + 1;
	i = path.rfind('.', i);
	if(i == std::string::npos)
		return vector3i({0,0,0});
	v.z = std::atoi(ptr + i);
	i = path.rfind('.', --i);
	if(i == std::string::npos)
		return vector3i({0,0,0});
	v.y = std::atoi(ptr + i);
	i = path.rfind('.', --i);
	if(i == std::string::npos)
		return vector3i({0,0,0});
	v.x = std::atoi(ptr + i);
	return v;
}

fext ext(const std::string& path) {
	std::size_t i = path.rfind('.') + 1;
	const void* ptr = path.c_str() + i;
	switch(path.length() - i) {
		case 3:
			return (std::memcmp(ptr, "smd", 3) == 0)?fext::SMD:fext::NONE;
		case 4:
			if(std::memcmp(ptr, "smd", 3) == 0)
				switch(path[path.size()-1]) {
					case 2: return fext::SMD2;
					case 3: return fext::SMD3;
					default: return fext::NONE;
				}
			return fext::NONE;
		case 5:
			if(std::memcmp(ptr, "smbp", 4) == 0)
				switch(path[path.size()-1]) {
					case 'h': return fext::SMBPH;
					case 'l': return fext::SMBPL;
					case 'm': return fext::SMBPM;
					default: return fext::NONE;
				}
		default: return fext::NONE;
	}
	return fext::NONE;
}

void decompose(const std::string& path) {
	std::size_t start = 0, cur;
	while((cur = path.find('/', start)) != std::string::npos) {
		std::clog << '\t' << path.substr(start, cur - start) << '\n';
		start = ++cur;
	}
}

class zip_streambuf : public std::streambuf {
	static constexpr std::size_t pbacksz = 32;
	static constexpr std::size_t buffsz = 512;
	static_assert(buffsz>pbacksz, "buffer size must be greater than or equal to"
	                              " put back size");
public:
	explicit zip_streambuf(zip_file_t* fptr) :
	fptr(fptr),
	buffer(buffsz + pbacksz) {
		char* end = buffer.data() + buffer.size();
		setg(end, end, end);
	}
	
	zip_streambuf(const zip_streambuf&) = delete;
	zip_streambuf& operator=(const zip_streambuf&) = delete;
protected:
	typedef std::streambuf::pos_type pos_type;
	typedef std::streambuf::off_type off_type;
	typedef std::ios_base::openmode openmode;
	
	virtual std::streambuf::int_type underflow() {
		if(gptr() < egptr())
			return traits_type::to_int_type(*gptr());
		char *base = buffer.data();
		char *start = base;
		if(eback() == base) {
			std::memmove(base, egptr() - pbacksz, pbacksz);
			start += pbacksz;
		}
		std::size_t n = zip_fread(fptr, start, buffer.size() - (start - base));
		if(n <= 0)
			return traits_type::eof();
		setg(base, start, start + n);
		return traits_type::to_int_type(*gptr());
	}
	
private:
	
	zip_file_t* fptr;
	std::vector<char> buffer;
};

class SmentFileReader {
public:
	
	SmentFileReader() :
	archive(nullptr) {}
	
	SmentFileReader(const char* path) {
		open(path);
	}
	
	SmentFileReader(const std::string& path) {
		open(path);
	}
	
	bool open(const std::string& path) {
		return open(path.c_str());
	}
	
	bool open(const char* path) {
		int errorp;
		archive = zip_open(path, ZIP_RDONLY, &errorp);
		if(!archive) {
			switch(errorp) {
				case ZIP_ER_EXISTS: // shouldn't happen, ZIP_EXCL unset
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
				if(st.valid & ZIP_STAT_NAME) {
					std::clog << st.name << '\n';
					decompose(st.name);
				}
		}
		return archive != nullptr;
	}
	
	void close() {
		zip_close(archive);
		archive = nullptr;
	}
	
	~SmentFileReader() {
		close();
	}
	
private:
	zip_t* archive;
	//int flags;
};

#endif

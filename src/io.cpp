#include "io.hpp"

#ifdef _WIN32
	#include <winsock2.h>
#else
	#include <arpa/inet.h>
#endif
#include <cstdint>
#include <cstring>
#include <algorithm>

namespace detail {
	static bool isLittleEndian() {
		return ntohl((uint32_t)1)!=1;
	}
	
	template<class ScalarT>
	struct wrapper {
		static ScalarT ntoh(ScalarT in) {
			if(isLittleEndian()) {
				char& dbuff = reinterpret_cast<char&>(in);
				std::reverse(&dbuff, &dbuff + sizeof(ScalarT));
			}
			return in;
		}
		
		static ScalarT hton(ScalarT in) {
			if(isLittleEndian()) {
				char& dbuff = reinterpret_cast<char&>(in);
				std::reverse(&dbuff, &dbuff + sizeof(ScalarT));
			}
			return in;
		}
	};
	
	template<>
	struct wrapper<uint32_t> {
		static uint32_t ntoh(uint32_t in) {
			return ntohl(in);
		}
		
		static uint32_t hton(uint32_t in) {
			return htonl(in);
		}
	};
	
	template<>
	struct wrapper<int32_t> {
		static int32_t ntoh(int32_t in) {
			uint32_t dbuff32 =
			wrapper<uint32_t>::ntoh(reinterpret_cast<uint32_t&>(in));
			return reinterpret_cast<int32_t&>(dbuff32);
		}
		
		static int32_t hton(int32_t in) {
			uint32_t dbuff32 =
			wrapper<uint32_t>::hton(reinterpret_cast<uint32_t&>(in));
			return reinterpret_cast<int32_t&>(dbuff32);
		}
	};
	
	template<>
	struct wrapper<uint64_t> {
		static uint64_t ntoh(uint64_t in) {
			if(isLittleEndian()) {
				uint32_t *split=(uint32_t*)&in,buff;
				buff=wrapper<uint32_t>::ntoh(split[0]);
				split[0]=wrapper<uint32_t>::ntoh(split[1]);
				split[1]=buff;
			}
			return in;
		}
		
		static uint64_t hton(uint64_t in) {
			if(isLittleEndian()) {
				uint32_t *split=(uint32_t*)&in,buff;
				buff=wrapper<uint32_t>::hton(split[0]);
				split[0]=wrapper<uint32_t>::hton(split[1]);
				split[1]=buff;
			}
			return in;
		}
	};
	
	template<>
	struct wrapper<int64_t> {
		static int64_t ntoh(int64_t in) {
			uint64_t dbuff64 =
			wrapper<uint64_t>::ntoh(reinterpret_cast<uint64_t&>(in));
			return reinterpret_cast<int64_t&>(dbuff64);
		}
		
		static int64_t hton(int64_t in) {
			uint64_t dbuff64 =
			wrapper<uint64_t>::hton(reinterpret_cast<uint64_t&>(in));
			return reinterpret_cast<int64_t&>(dbuff64);
		}
	};
}

template<class ScalarT>
ScalarT ntoh(identity_t<ScalarT> in) {
	return detail::wrapper<ScalarT>::ntoh(in);
}

template<class ScalarT>
ScalarT hton(identity_t<ScalarT> in) {
	return detail::wrapper<ScalarT>::hton(in);
}

template<class ScalarT>
ScalarT readFromNetBuffer(const void* src) {
	ScalarT in;
	memcpy(&in, src, sizeof(ScalarT));
	return ntoh<ScalarT>(in);
}

template<class ScalarT>
ScalarT readFromNetStream(std::istream& i) {
	char buf[sizeof(ScalarT)];
	i.read(buf, sizeof(ScalarT));
	return readFromNetBuffer<ScalarT>(static_cast<void*>(buf));
}

template<class ScalarT>
void writeToNetBuffer(void* trg, identity_t<ScalarT> src) {
	src = hton<ScalarT>(src);
	memcpy(trg, &src, sizeof(ScalarT));
}

template<class ScalarT>
void writeToNetStream(std::ostream& o, identity_t<ScalarT> src) {
	char buf[sizeof(ScalarT)];
	writeToNetBuffer<ScalarT>(static_cast<void*>(buf), src);
	o.write(buf, sizeof(ScalarT));
}


template int16_t readFromNetStream<int16_t>(std::istream&);
template uint16_t readFromNetStream<uint16_t>(std::istream&);
template int32_t readFromNetStream<int32_t>(std::istream&);
template uint32_t readFromNetStream<uint32_t>(std::istream&);
template int64_t readFromNetStream<int64_t>(std::istream&);
template uint64_t readFromNetStream<uint64_t>(std::istream&);
template float readFromNetStream<float>(std::istream&);

template void writeToNetStream<int16_t>(std::ostream&, int16_t);
template void writeToNetStream<uint16_t>(std::ostream&, uint16_t);
template void writeToNetStream<int32_t>(std::ostream&, int32_t);
template void writeToNetStream<uint32_t>(std::ostream&, uint32_t);
template void writeToNetStream<int64_t>(std::ostream&, int64_t);
template void writeToNetStream<uint64_t>(std::ostream&, uint64_t);
template void writeToNetStream<float>(std::ostream&, float);

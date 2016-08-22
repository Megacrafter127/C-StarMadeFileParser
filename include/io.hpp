#ifndef IO_H_
#define IO_H_

#include <istream>

namespace detail {
	template<class T>
	struct identity {
		typedef T t;
	};
}

template<class ScalarT>
ScalarT ntoh(typename detail::identity<ScalarT>::t in);

template<class ScalarT>
ScalarT hton(typename detail::identity<ScalarT>::t in);

template<class ScalarT>
ScalarT readFromNetBuffer(const void* src);

template<class ScalarT>
ScalarT readFromNetStream(std::istream& i);

template<class ScalarT>
void writeToNetBuffer(void* trg, typename detail::identity<ScalarT>::t src);

#endif // IO_H_

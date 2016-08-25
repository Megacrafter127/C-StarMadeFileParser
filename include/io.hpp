#ifndef IO_H_
#define IO_H_

#include <istream>

namespace detail {
	template<class T>
	struct identity {
		typedef T type;
	};
}

template<class T>
using identity_t = typename detail::identity<T>::type;

template<class ScalarT>
ScalarT ntoh(identity_t<ScalarT> in);

template<class ScalarT>
ScalarT hton(identity_t<ScalarT> in);

template<class ScalarT>
ScalarT readFromNetBuffer(const void* src);

template<class ScalarT>
ScalarT readFromNetStream(std::istream& i);

template<class ScalarT>
void writeToNetBuffer(void* trg, identity_t<ScalarT> src);

template<class ScalarT>
void writeToNetStream(std::ostream& o, identity_t<ScalarT> src);

#endif // IO_H_

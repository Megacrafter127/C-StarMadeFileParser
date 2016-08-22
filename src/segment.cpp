#include "segment.hpp"

block block::fromRaw(const byte* ptr, const blockTypeMap& tmap) {
	const unsigned char* raw = reinterpret_cast<const unsigned char*>(ptr);
	block b;
	b.id = raw[2]+((raw[1] & 0x7) << 8);
	auto it = tmap.find(b.id);
	if(it != tmap.end() && it->second == CORNER) {
		b.orientation = (raw[0] & 0xF8) >> 3;
		b.hp = ((raw[0] & 0x7) << 5) + ((raw[1] & 0xF8) >> 3);
	} else {
		b.hp = ((raw[0] & 0xF) << 5) + ((raw[1] & 0xF8) >> 3);
		if(it != tmap.end() && it->second == STANDARD)
			b.orientation = (raw[0] & 0xF0) >> 4;
		else {
			b.orientation = (raw[0] & 0xE0) >> 5;
			b.active = raw[0] & 0x10;
		}
	}
	return b;
}

template<std::size_t N>
Segment<N>::Segment(int32_t x, int32_t y, int32_t z, int64_t timestamp,
					std::vector<byte>& buffer):
content(new cube),
position{x, y, z},
timestamp(timestamp) {
	//assert(buffer.size() > 3*N*N*N)
	byte* ptr = buffer.data();
	for(std::size_t  i = 0 ; i < N ; ++i)
		for(std::size_t j = 0 ; j < N ; ++j)
			for(std::size_t k = 0 ; k < N ; ++k, ptr+=3)
				(*this)[k][j][i] = block::fromRaw(ptr, blockTypeMap());
}

template<std::size_t N>
Segment<N>::Segment():
content(nullptr) {}

template<std::size_t N>
Segment<N>::Segment(Segment&& other):
content(std::move(other.content)) {}

template<std::size_t N>
Segment<N>& Segment<N>::operator=(Segment&& other) {
	using std::swap;
	if(content.get())
		swap(content, other.content);
	else
		content.reset(other.content.release());
	return *this;
}

template<std::size_t N>
Segment<N>::operator bool() const {
	return content.get();
}

template<std::size_t N>
block& Segment<N>::at(int x, int y, int z) {
	return content.get()->at(x).at(y).at(z);
}

template<std::size_t N>
const block& Segment<N>::at(int x, int y, int z) const {
	return content.get()->at(x).at(y).at(z);
}

template<std::size_t N>
typename Segment<N>::plane& Segment<N>::operator[](std::size_t i) {
	return (*content.get())[i];
}

template<std::size_t N>
const typename Segment<N>::plane& Segment<N>::operator[](std::size_t i) const {
	return (*content.get())[i];
}

template<std::size_t N>
typename Segment<N>::iterator Segment<N>::begin() {
	return content.get()->begin();
}

template<std::size_t N>
typename Segment<N>::const_iterator Segment<N>::begin() const {
	return content.get()->begin();
}

template<std::size_t N>
typename Segment<N>::const_iterator Segment<N>::cbegin() const {
	return content.get()->cbegin();
}

template<std::size_t N>
typename Segment<N>::iterator Segment<N>::end() {
	return content.get()->end();
}

template<std::size_t N>
typename Segment<N>::const_iterator Segment<N>::end() const {
	return content.get()->end();
}

template<std::size_t N>
typename Segment<N>::const_iterator Segment<N>::cend() const {
	return content.get()->cend();
}

template class Segment<16>;
template class Segment<32>;

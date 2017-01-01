#ifndef VECTOR3_H_
#define VECTOR3_H_

#include <iostream>

template<class T>
struct vector3 {
	vector3() {}
	vector3(T x, T y, T z):
	x(x),
	y(y),
	z(z) {}
	
	bool operator<(const vector3& other) {
		return x < other.x
		       || (x == other.x
		           && (y < other.y || (y==other.y && z < other.z)));
	}
	
	T x, y, z;
};

template<class T>
std::istream& operator>>(std::istream& i, vector3<T>& v) {
	return i >> v.x >> v.y >> v.z;
}

template<class T>
std::ostream& operator<<(std::ostream& o, const vector3<T> v) {
	return o << v.x << v.y << v.z;
}

template<class T>
struct vector4 {
	vector4() {}
	vector4(T x, T y, T z, T w):
	x(x),
	y(y),
	z(z),
	w(w) {}
	
	bool operator<(const vector4& other) {
		return x < other.x
		       || (x == other.x
		           && (y < other.y
		               || (y==other.y
		                   && (z < other.z || (z == other.z && w < other.w))
		                  )
		              )
		          );
	}
	
	T x, y, z, w;
};

template<class T>
std::istream& operator>>(std::istream& i, vector4<T>& v) {
	return i >> v.x >> v.y >> v.z >> v.w;
}

template<class T>
std::ostream& operator<<(std::ostream& o, const vector4<T> v) {
	return o << v.x << v.y << v.z << v.w;
}


typedef vector3<char> vector3b;
typedef vector3<int16_t> vector3s;
typedef vector3<int32_t> vector3i;
typedef vector3<float> vector3f;
typedef vector4<float> vector4f;

#endif // VECTOR3_H_

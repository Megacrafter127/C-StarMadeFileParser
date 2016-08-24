AR       := ar
ARFLAGS  := -r -c -s
CXX      := g++
CXXFLAGS := -Wall -Wextra -pedantic -std=c++11 -fPIC -I$(ROOT_PATH)/include -I/usr/local/include -I/usr/local/lib/libzip/include/
LDFLAGS  := -L /usr/local/lib
LIBS     := -lzip -lz

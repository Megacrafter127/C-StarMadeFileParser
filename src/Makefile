AR       := ar
ARFLAGS  := -r -c -s
CXX      := g++
CXXFLAGS := -Wall -Wextra -pedantic -std=c++11 -fPIC -g -O0
LDFLAGS  :=
LIBS     := -lz
TESTSRC  := $(wildcard ./*Test.cpp)
SRC      := $(filter-out $(TESTSRC),$(wildcard ./*.cpp))
OBJDIR   := ./obj
LIBDIR   := ./lib
BINDIR   := ./bin
BIN      := $(BINDIR)/test

obj_of_src = $(addprefix $(OBJDIR)/, $(1:%.cpp=%.o))

OBJ      := $(call obj_of_src,$(SRC))
LIBNAME  := smfp

STATLIB  := $(LIBDIR)/$(LIBNAME).a
ifeq ($(OS),Windows_NT)
	$(error Windows compilation not supported yet)
else
	UNAME := $(shell uname -s)
	ifeq ($(UNAME),Darwin) # OSX
		SONAME_OPTION := -install_name
		DYNLIB        := $(LIBDIR)/lib$(LIBNAME).dylib
	else ifeq ($(UNAME),Linux) # Linux
		SONAME_OPTION := -soname
		DYNLIB        := $(LIBDIR)/lib$(LIBNAME).so
	else # Unknown
		$(error Makefile couldn't recognize operating system)
	endif
endif

.PHONY: all clean

all: static dynamic test

dynamic: $(DYNLIB)
static: $(STATLIB)
test: $(BIN)

$(DYNLIB): $(OBJ) | $(LIBDIR)
	$(CXX) $(LDFLAGS) -shared -Wl,$(SONAME_OPTION),$(notdir $@) -o $@ $^ $(LIBS)

$(STATLIB): $(OBJ) | $(LIBDIR)
	$(AR) $(ARFLAGS) $@ $?

$(BIN): $(DYNLIB) | $(BINDIR)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $(TESTSRC) -L$(LIBDIR) -l$(LIBNAME) -lz

$(LIBDIR) $(OBJDIR) $(BINDIR):
	mkdir -p $@

define obj_rule_tpl
$(call obj_of_src,$(1)): $(1) | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -o $$@ -c $$<
endef

$(foreach source, $(SRC), \
    $(eval $(call obj_rule_tpl,$(source))) \
 )

clean:
	rm -rf $(OBJDIR) $(LIBDIR) $(BINDIR)

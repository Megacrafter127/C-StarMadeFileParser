AR       := ar
ARFLAGS  := -r -c -s
CXX      := g++
CXXFLAGS := -Wall -Wextra -pedantic -std=c++03 -fPIC
LDFLAGS  :=
LIBS     := -lz
SRC      := $(wildcard ./*.cpp)
OBJDIR   := ./obj
LIBDIR   := ./lib

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
		DYNLIB        := $(LIBDIR)/$(LIBNAME).dylib
	else ifeq ($(UNAME),Linux) # Linux
		SONAME_OPTION := -soname
		DYNLIB        := $(LIBDIR)/$(LIBNAME).so
	else # Unknown
		$(error Makefile couldn't recognize operating system)
	endif
endif

.PHONY: all clean

all: static dynamic

dynamic: $(DYNLIB)
static: $(STATLIB)

$(DYNLIB): $(OBJ) | $(LIBDIR)
	$(CXX) $(LDFLAGS) -shared -Wl,$(SONAME_OPTION),$(notdir $@) -o $@ $^ $(LIBS)

$(STATLIB): $(OBJ) | $(LIBDIR)
	$(AR) $(ARFLAGS) $@ $?

$(LIBDIR) $(OBJDIR):
	mkdir -p $@

define obj_rule_tpl
$(call obj_of_src,$(1)): $(1) | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -o $$@ -c $$<
endef

$(foreach source, $(SRC), \
    $(eval $(call obj_rule_tpl,$(source))) \
 )

clean:
	rm -rf $(OBJDIR) $(LIBDIR)

################################################################################
# Defines the necessary rules to compile the library.                          #
################################################################################

ifeq ($(origin SRC_RULES),undefined) #include guard
SRC_RULES :=

################################################################################
# internal variables                                                           #
################################################################################

ifeq ($(origin DIRECTORY), undefined)
  DIRECTORY := .
endif

SRC      := $(wildcard $(DIRECTORY)/*.cpp)
OBJDIR   := $(DIRECTORY)/obj
LIBDIR   := $(DIRECTORY)/../lib
DEPDIR   := $(DIRECTORY)/dep
LIBNAME  := smfp

dep_of_src = $(addprefix $(DEPDIR)/, $(subst .__,,$(subst /,__,$(1:%.cpp=%.d))))
obj_of_src = $(addprefix $(OBJDIR)/, $(notdir $(1:%.cpp=%.o)))

OBJ      := $(call obj_of_src,$(SRC))

STATLIB  := $(LIBDIR)/$(LIBNAME).a

include $(ROOT_PATH)/tools/build/os_detection.mk

ifeq ($(OPERATING_SYSTEM),Windows)
  $(error Windows compilation not supported yet)
else ifeq ($(OPERATING_SYSTEM),MacOS)
  DYNLIB        := $(LIBDIR)/lib$(LIBNAME).dylib
  LINKER_OPTIONS := -shared -Wl,-install_name,@rpath/$(notdir $(DYNLIB))
else ifeq ($(OPERATING_SYSTEM),Linux)
  DYNLIB        := $(LIBDIR)/lib$(LIBNAME).so
  LINKER_OPTIONS := -shared -Wl,-soname,$(notdir $(DYNLIB))
else
  $(error Unknown operating system)
endif

################################################################################
# Rules                                                                        #
################################################################################

$(DYNLIB): $(OBJ) | $(LIBDIR)
	$(CXX) -o $@ $^ $(LDFLAGS) $(LINKER_OPTIONS) $(LIBS)

$(STATLIB): $(OBJ) | $(LIBDIR)
	$(AR) $(ARFLAGS) $@ $?

$(LIBDIR) $(OBJDIR) $(BINDIR) $(DEPDIR):
	mkdir -p $@

include $(ROOT_PATH)/tools/build/object_rules.mk

################################################################################
# Populate targets                                                             #
################################################################################

TARGETS := $(TARGETS) $(DYNLIB) $(STATLIB)
CLEAN_TARGETS := $(CLEAN_TARGETS) $(DEPDIR) $(OBJDIR) $(LIBDIR)

endif #SRC_RULES
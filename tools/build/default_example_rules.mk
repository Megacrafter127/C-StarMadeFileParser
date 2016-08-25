################################################################################
# Defines the necessary rules to build object and binaries in the target       #
# directory and populates the corresponding target variables.                  #
#                                                                              #
# This file relies on the previous definition of the following variables:      #
# - ROOT_PATH     : the path to the repository's root (either relative or      #
#                   absolute)                                                  #
# - DIRECTORY     : the path to the directory where the source files should be #
#                   looked for, the object and dependency files will also be   #
#                   created in subdirectories of DIRECTORY                     #
#                   if DIRECTORY isn't defined, it is assumed to be '.'        #
# - CXX           : a c++11 compliant compiler which supports the -MM, -MT     #
#                   and -MF options (e.g. gcc or clang), used to compile and   #
#                   to generate dependency files (a default definition is      #
#                   usually provided by make)                                  #
# - CXXFLAGS      : the flags to be passed to the c++ compiler                 #
# - LDFLAGS       : the flags to be passed to the linker                       #
# - LIBS          : the libraries to be linked against the binaries            #
# - BASE_NAME     : the base name of the binary                                #
# - SRC           : the example sources, if undefined and $(DIRECTORY)         #
#                   contains a file named main_sample.cpp, they are assumed    #
#                   to be all the source files in $(DIRECTORY) except files    #
#                   named main_test.cpp or *testsuite.cpp                      #
# - SRC_TEST      : the  unit tests sources, if undefined and $(DIRECTORY)     #
#                   contains a file named main_test.cpp, they are assumed      #
#                   to be all the source files in $(DIRECTORY) except files    #
#                   named main_sample.cpp                                      #
#                                                                              #
# This file updates the following variables:                                   #
# - TARGETS                                                                    #
# - CLEAN_TARGETS                                                              #
# - DIRECTORY                                                                  #
#                                                                              #
# This file invalidates the following variables:                               #
# - DEPDIR                                                                     #
# - OBJDIR                                                                     #
# - BINDIR                                                                     #
# - SRC                                                                        #
# - dep_of_src                                                                 #
# - obj_of_src                                                                 #
# - DEP                                                                        #
# - OBJ                                                                        #
# - TMP_LIBS                                                                   #
# - dep_rule_tpl                                                               #
# - obj_rule_tpl                                                               #
################################################################################

################################################################################
# internal variables                                                           #
################################################################################

include $(ROOT_PATH)/tools/build/os_detection.mk

ifeq ($(origin DIRECTORY),undefined)
  DIRECTORY := .
endif

ifneq ($(flavor SRC),simple)
  SRC := $(wildcard $(DIRECTORY)/*.cpp)
else
  SRC := $(addprefix $(DIRECTORY)/,$(SRC))
endif

TMPTARGETS   := $(TARGETS)
TMPDIRECTORY := $(DIRECTORY)
TMPSRC       := $(SRC)
DIRECTORY    := $(ROOT_PATH)/src
include $(DIRECTORY)/rules.mk
SRC          := $(TMPSRC)
DIRECTORY    := $(TMPDIRECTORY)
TARGETS      := $(TMPTARGETS)

DEPDIR := $(subst ./,,$(subst //,/,$(DIRECTORY)/)dep)
OBJDIR := $(subst ./,,$(subst //,/,$(DIRECTORY)/)obj)
BINDIR := $(subst ./,,$(subst //,/,$(DIRECTORY)/))../bin

dep_of_src = $(addprefix $(DEPDIR)/, $(subst .__,,$(subst /,__,$(1:%.cpp=%.d))))
obj_of_src = $(addprefix $(OBJDIR)/, $(notdir $(1:%.cpp=%.o)))

OBJ := $(call obj_of_src,$(SRC))

################################################################################
# rules                                                                        #
################################################################################

$(OBJDIR) $(DEPDIR) $(BINDIR):
	mkdir -p $@

#ifneq ($(strip $(SRC)),)
  ifeq ($(OPERATING_SYSTEM),MacOS)
    EXTRALDFLAGS := -Wl,-rpath,@executable_path/../../lib/
  else ifeq ($(OPERATING_SYSTEM),Linux)
    EXTRALDFLAGS := -Wl,-rpath,\$$ORIGIN/../../lib
  else ifeq ($(OPERATING_SYSTEM),Windows)
    $(error Windows not supported yet)
  else
    $(error Unknown operating system)
  endif

  $(BINDIR)/$(BASE_NAME) : $(OBJ) $(DYNLIB) | $(BINDIR)
	$(CXX) -o $@ $^ $(LDFLAGS) $(EXTRALDFLAGS) $(LIBS) -L$(LIBDIR) -l$(LIBNAME)

#endif

include $(ROOT_PATH)/tools/build/object_rules.mk

################################################################################
# populate targets                                                             #
################################################################################

TARGETS := $(TARGETS) $(BINDIR)/$(BASE_NAME)
CLEAN_TARGETS := $(CLEAN_TARGETS) $(DEPDIR) $(OBJDIR) $(BINDIR)

################################################################################
# cleanup                                                                      #
################################################################################

SRC =
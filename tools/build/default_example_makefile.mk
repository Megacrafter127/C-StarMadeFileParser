################################################################################
# Local sample Makefile template.                                              #
#                                                                              #
# This file relies on the previous definition of the following variables:      #
# - ROOT_PATH : the path to the repository's root (either relative or          #
#               absolute)                                                      #
################################################################################

#default target
all:

################################################################################
# Rules                                                                        #
################################################################################

include $(ROOT_PATH)/tools/build/compiler_options.mk
include ./rules.mk

################################################################################
# Targets                                                                      #
################################################################################

.PHONY: all clean
all: $(TARGETS)

clean:
	rm -rf $(CLEAN_TARGETS)
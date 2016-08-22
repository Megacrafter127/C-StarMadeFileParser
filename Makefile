################################################################################
# Root Makefile.                                                               #
################################################################################

################################################################################
# Internal variables                                                           #
################################################################################

ROOT_PATH := .

include $(ROOT_PATH)/tools/build/compiler_options.mk

#default target
all:

################################################################################
# Rules                                                                        #
################################################################################

DIRECTORY := ./examples
include ./examples/rules.mk
EXAMPLES_TARGETS := $(TARGETS)

DIRECTORY := ./src
include ./src/rules.mk

################################################################################
# Targets                                                                      #
################################################################################

all: lib examples

lib: dynamic static

dynamic: $(DYNLIB)

static: $(STATLIB)

examples: $(EXAMPLES_TARGETS)

.PHONY: all lib dynamic static examples clean

clean:
	rm -rf $(CLEAN_TARGETS)
################################################################################
# Rules                                                                        #
################################################################################

ifeq ($(origin DIRECTORY),undefined)
  DIRECTORY := .
endif

EXAMPLES := ppm sment smbpm smbph

CURDIR := $(DIRECTORY)
$(foreach example, $(EXAMPLES),                                                \
  $(eval DIRECTORY := $(CURDIR)/$(example))                                    \
  $(eval include $(DIRECTORY)/rules.mk)                                        \
  $(eval LOCAL_TARGETS := $(LOCAL_TARGETS) $(TARGETS))                         \
  $(eval $(example)_TARGETS := $($(example)_TARGETS) $(TARGETS))               \
  $(eval TARGETS := )                                                          \
 )
DIRECTORY := $(CURDIR)

TARGETS := $(LOCAL_TARGETS)
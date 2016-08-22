ROOT_PATH := ..

# default target
all:

include $(ROOT_PATH)/tools/build/compiler_options.mk
include ./rules.mk

all: $(EXAMPLES)

define example_rule_tpl
$(1): $($(1)_TARGETS)
endef

$(foreach example, $(EXAMPLES), \
  $(eval $(call example_rule_tpl,$(example))) \
 )

.PHONY: all clean $(EXAMPLE)

clean:
	rm -rf $(CLEAN_TARGETS)
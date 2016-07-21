CXX      := g++
CXXFLAGS := -Wall -Wextra -pedantic -std=c++03
SRC      :=$(wildcard ./*.cpp)
OBJDIR   := ./obj
BINDIR   := ./bin

obj_of_src = $(addprefix $(OBJDIR)/, $(1:%.cpp=%.o))

OBJ      :=$(call obj_of_src,$(SRC))
BIN      := $(BINDIR)/test

.PHONY: all clean

all: $(BIN)

$(BINDIR) $(OBJDIR):
	mkdir -p $@

define obj_rule_tpl
$(call obj_of_src,$(1)): $(1) | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -o $$@ -c $$<
endef

$(foreach source, $(SRC), \
    $(eval $(call obj_rule_tpl,$(source))) \
 )

$(BIN): $(OBJ) | $(BINDIR)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^

clean:
	rm -rf $(OBJDIR) $(BINDIR)
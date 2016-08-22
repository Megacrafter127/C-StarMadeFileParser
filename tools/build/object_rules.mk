################################################################################
# Defines the necessary rules to build objects and dependency files out of     #
# source files in the corresponding directories.                               #
#                                                                              #
# This file relies on the previous definition of the following variables:      #
# - ROOT_PATH     : the path to the repository's root (either relative or      #
#                   absolute)                                                  #
# - SRC           : the list of source files                                   #
# - DEPDIR        : the path to the directory where the dependency files       #
#                   should be created                                          #
# - OBJDIR        : the path to the directory where the object files should be #
#                   created                                                    #
# - dep_of_src    : a unary function which takes a list of paths to source     #
#                   files, and returns a list of paths to the corresponding    #
#                   dependency files                                           #
# - obj_of_src    : a binary function which takes a list of paths to source    #
#                   files and returns a list of paths to the corresponding     1
#                   object files                                               #
# - CXX           : a c++11 compliant compiler which supports the -MM, -MT     #
#                   and -MF options (e.g. gcc or clang), used to compile and   #
#                   to generate dependency files (a default definition is      #
#                   usually provided by make)                                  #
# - CXXFLAGS      : the flags to be passed to the c++ compiler                 #
# - PASL_SUFFIXES : the pasl suffixes names                                    #
#                                                                              #
# This file updates the following variables:                                   #
# - CLEAN_TARGETS                                                              #
#                                                                              #
# This file invalidates the following variables:                               #
# - obj_rule_tpl                                                               #
# - dep_rule_tpl                                                               #
################################################################################

# note concerning the weird names and possible "duplicates" of dependency files
#
# I chose to not use a recursive makefile system as they tend to cause longer
# build times. They cause multiple shell calls, and will often build an
# incomplete and/or inaccurate depency DAG which may cause builds to either
# - not be triggered when needed
# - triggered several times instead of once
# - or triggered whereas they aren't needed
# In each case, you'll end up having to re-build things to avoid bugs, hence,
# waste time.
#
# However, I didn't want to give up on the possibility to have "local makefiles"
# on top of the root makefile (which should allow already to build anything).
# I think that, while they are inefficient, recursive builds also are easier
# to grasp and use as they are inherently modular (and what's great with modular
# systems is that if you don't understand the whole thing at first, you'll
# probably have better luck with one of its smaller modules).
# Therefore, I also wrote Makefiles in each subdirectory containing source code
# sufficient to build a single standalone example.
# Of course, writing several times slightly different versions of the same rules
# in different places should be avoided as it makes maintaining that code more
# difficult (DRY principle), so I wrote rules depending on the working
# directory.
#
# Unfortunately, $(CXX) -MM will output prerequisites names relatively to the
# working directory (and I don't think I could change that behaviour easily),
# which means that the makefile rules contained in the dependency files won't
# work if you decide to use them from another directory (the relative paths
# wouldn't be accurate anymore).
# A workaround is to change the name of the dependency files depending on your
# current working directory, so that running a makefile from a different
# directory actually use a different set of dependency files. That's certainly
# not perfect as these files would contain redundant information which would
# need to be recomputed when issuing make in another directory. Still that's a
# lesser evil: at least your compilation won't break because of missing
# prerequisites when you issue make commands inside another directory, and
# re-computing those prerequisites once for each different directory you want
# to issue make commands in isn't that expensive anyway.
#
# If you find a cleaner way to solve that problem, feel free to change that
# behaviour.

################################################################################
# rules                                                                        #
################################################################################

dependency_rule_template = $(call dep_of_src,$(1)) : $(1) | $(DEPDIR)

$(foreach source, $(SRC), $(eval $(call dependency_rule_template,$(source))))

define dep_rule_tpl
$(call dep_of_src,$(1)): $(1) | $(DEPDIR)
	$(CXX) $(CXXFLAGS) -MM $$< -MF $$@ -MT $$@ \
	       $(strip -MT $(call obj_of_src,$(1)))
endef

ifneq ($(MAKECMDGOALS),clean)
  -include $(call dep_of_src,$(SRC))
endif

define obj_rule_tpl
$(call obj_of_src,$(1)): $(1) | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -o $$@ -c $$<
endef

$(foreach source, $(SRC), \
  $(eval $(call dep_rule_tpl,$(source))) \
  $(eval $(call obj_rule_tpl,$(source))) \
 )
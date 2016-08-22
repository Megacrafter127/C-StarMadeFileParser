################################################################################
# Detect the operating system.                                                 #
#                                                                              #
# This file updates the following variables:                                   #
# - OPERATING_SYSTEM : contains either "Windows", "MacOS", "Linux" or          #
#                      "Unknown"                                               #
#                                                                              #
# This file invalidates the following variables:                               #
# - UNAME                                                                      #
################################################################################

ifeq ($(OS),Windows_NT)
  OPERATING_SYSTEM := Windows
else
  UNAME := $(shell uname -s)
  ifeq ($(UNAME),Darwin)
    OPERATING_SYSTEM := MacOS
  else ifeq ($(UNAME),Linux)
    OPERATING_SYSTEM := Linux
  else
    OPERATING_SYSTEM := Unknown
  endif
endif

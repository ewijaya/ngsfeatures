#  Author: Paul B. Horton
#  Organization: Computational Biology Research Center, AIST, Japan
#  Copyright (C) 2007, Paul B. Horton. All rights reserved.
#  Creation Date: 2007.10.14
#  Last Modification: $Date: 2009/06/18 23:10:35 $
#  
#  CBRC_CPP_HOME is the root directory of the C++ code. Use environment variable for this:
#  export CBRC_CPP_HOME=~/cvs/cbrc/C++

.SUFFIXES:  # cancel implicit suffix rules

.SECONDEXPANSION:  # allows use of $$@ in prerequisites

# ***************** Compiler Options *************************
# CBRC_OPTIMIZE affects the amount of correctness checking done.
# general rules for CBRC_OPTIMIZE values:
# use CBRC_OPTIMIZE = 1 for checks that are so slow they change the 
#                       big O complexity of the computation.
# use CBRC_OPTIMIZE = 2 for checks that may have significant constant factor overhead
#                       this is intended to be the default.
# use CBRC_OPTIMIZE = 3 for checks that have a trivial overhead.
# use CBRC_OPTIMIZE = 4 for no checking
CBRC_OPTIMIZE = 2

# Add GDB_DEBUG flag, i.e. export CPP_DEBUG="-g -DGDB_DEBUG"; make -e target
# to include code which is convenient for use 
# with gdb. The typical case is the addition of "for(;;)" to
# trap execution so that one can debug before crashing.

CPP_OPTIMIZE = -O3
CPP_DEBUG = -g
CPP_WARN = -Wall
CPP_FLAGS = $(CPP_WARN) $(CPP_DEBUG) $(CPP_OPTIMIZE) -DCBRC_OPTIMIZE=$(CBRC_OPTIMIZE) -combine
CPP_LIBS = -lboost_regex
CPP = g++


# ***************** Target List *************************
PROGRAM_FILES = $(wildcard *.cc)

TARGETS = $(PROGRAM_FILES:.cc=)

.PHONY : clean


# ***************** Production Rules *************************
all : $(TARGETS)

clean :
	rm -f $(TARGETS) 2>/dev/null; true


$(TARGETS) : % : $$(shell cpp -MM $$@.cc -I$(CBRC_CPP_HOME) | sed -e 's/.*://' -e 's/\\//g' | tr -d '\n') \
	$$(shell cpp -MM $$@.cc -I$(CBRC_CPP_HOME) | sed -e 's/.*://' -e 's/\.hh/.cc/' -e 's/\\//g' | xargs ls 2> /dev/null)
	$(CPP) $(CPP_FLAGS) -o $@ \
	$(sort $(shell cpp -MM $@.cc -I$(CBRC_CPP_HOME) | sed -e 's/.*://' -e 's/\.hh/.cc/' -e 's/\\//g' | xargs ls 2> /dev/null))	\
	$(CPP_LIBS) -I$(CBRC_CPP_HOME)

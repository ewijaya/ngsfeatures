#  Author: Paul Horton
#  Organization: Computational Biology Research Center, AIST, Japan
#  Copyright (C) 2008, Paul Horton. All rights reserved.
#  Creation Date: 2008.7.2
#  Last Modification: $Date: 2009/05/05 09:42:43 $
#  
#  CBRC_CPP_HOME is the root directory of the C++ code. Use environment variable for this:
#  export CBRC_CPP_HOME=~/cvs/cbrc/C++
#
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
CPP_FLAGS = $(CPP_WARN) $(CPP_DEBUG) $(CPP_OPTIMIZE) -DCBRC_OPTIMIZE=$(CBRC_OPTIMIZE)
CPP_LIBS = -lboost_regex
CPP = g++

# ***************** Dependencies *************************
ArgvParserDIR = $(CBRC_CPP_HOME)/utils/argvParsing
ArgvParserHH  = $(ArgvParserDIR)/ArgvParser.hh

perlishDIR = $(CBRC_CPP_HOME)/utils/perlish
perlishHH  = $(perlishDIR)/perlish.hh
perlishCC  = $(perlishDIR)/perlish.cc
perlish    = $(perlishHH) $(perlishCC)

FLEArrayDIR = $(CBRC_CPP_HOME)/utils/FLArray
FLEArrayHH  = $(FLEArrayDIR)/FLEArray.hh

gdbUtilsDIR = $(CBRC_CPP_HOME)/utils/gdb
gdbUtilsHH  = $(gdbUtilsDIR)/gdbUtils.hh


TEST_CASE = 0


# ***************** Target List *************************
TARGETS =	tryGdbUtils_die 	\
		tryGdbUtils_dief 	\
		tryGdbUtils_assertf	\
		tryGdbUtils_if_warnf


# *************** Phony Targets ***************
.PHONY : all clean list

all : $(TARGETS)

clean :
	rm -f $(TARGETS) 2>/dev/null; true

list :
	@echo $(TARGETS) | sed 's/\s/\n/g'


# ***************** Production Rules *************************
tryGdbUtils_die : $(gdbUtilsHH)	\
		tryGdbUtils_die.cc
	$(CPP) $(CPP_FLAGS) -o $@ $@.cc

tryGdbUtils_dief : $(gdbUtilsHH)	\
		tryGdbUtils_dief.cc
	$(CPP) $(CPP_FLAGS) -o $@ -DTEST_CASE=$(TEST_CASE) $@.cc

tryGdbUtils_assertf : $(gdbUtilsHH)	\
		tryGdbUtils_assertf.cc
	$(CPP) $(CPP_FLAGS) -o $@ -DTEST_CASE=$(TEST_CASE) $@.cc

tryGdbUtils_if_warnf : $(gdbUtilsHH)	\
		tryGdbUtils_if_warnf.cc
	$(CPP) $(CPP_FLAGS) -o $@ -DTEST_CASE=$(TEST_CASE) $@.cc

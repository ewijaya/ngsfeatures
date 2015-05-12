#  Author: Paul B. Horton
#  Organization: Computational Biology Research Center, AIST, Japan
#  Copyright (C) 2005,2006, Paul B. Horton, All rights reserved.
#  Creation Date: : 2005.12.20
#  Last Modification: $Date: 2007/09/16 01:34:46 $
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
OPTIMIZE = $(CPP_OPTIMIZE) $(CPP_DEBUG) -DCBRC_OPTIMIZE=$(CBRC_OPTIMIZE)
WARN = -Wall
CPP = g++

# ***************** Dependencies *************************
ArgvParserDIR = $(CBRC_CPP_HOME)/utils/argvParsing/
ArgvParserHH = $(ArgvParserDIR)/ArgvParser.hh
ArgvParserCC = $(ArgvParserDIR)/ArgvParser.cc

ResidueIndexMapDIR = $(CBRC_CPP_HOME)/utils/sequence/ResidueIndexMap
ResidueIndexMapHH = $(ResidueIndexMapDIR)/ResidueIndexMap.hh
ResidueIndexMapCC = $(ResidueIndexMapDIR)/ResidueIndexMap.cc
ResidueIndexMap = $(ResidueIndexMapHH) $(ResidueIndexMapCC)

ResidueIndexWindowDIR = $(ResidueIndexMapDIR)/ResidueIndexWindow
ResidueIndexWindowHH = $(ResidueIndexWindowDIR)/ResidueIndexWindow.hh
ResidueIndexWindowCC = $(ResidueIndexWindowDIR)/ResidueIndexWindow.cc
ResidueIndexWindow = $(ResidueIndexWindowHH) $(ResidueIndexWindowCC)

SRCS = $(ArgvParserCC) $(ResidueIndexMapCC)

HDRS = $(ArgvParserHH) $(ResidueIndexMapHH)



# ***************** Target List *************************
all : \
	tryConstResidueIndexSequenceBoundaries  \
	tryResidueIndexWindow



# ***************** Production Rules *************************
tryConstResidueIndexSequenceBoundaries : $(SRCS) $(HDRS) tryConstResidueIndexSequenceBoundaries.cc
	$(CPP) $(WARN) $(OPTIMIZE) -o $@ $@.cc $(SRCS) -I$(CBRC_CPP_HOME)

tryResidueIndexWindow : $(SRCS) $(HDRS) \
		$(ResidueIndexWindow) \
		tryResidueIndexWindow.cc
	$(CPP) $(WARN) $(OPTIMIZE) -o $@ $@.cc $(SRCS) \
		$(ResidueIndexWindowCC) \
		-I$(CBRC_CPP_HOME)


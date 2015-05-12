#  Author: Paul B. Horton
#  Organization: Computational Biology Research Center, Japan
#  Creation Date: : 2006.4.29
#  Last Modification: $Date: 2006/04/29 14:49:37 $
#  Copyright: All rights reserved
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
CPP_OPTIONS = $(CPP_OPTIMZE) $(CPP_DEBUG) $(WARN)
CPP = g++

# ***************** Dependencies *************************
ArgvParserDIR = $(CBRC_CPP_HOME)/utils/argvParsing/
ArgvParserHH = $(ArgvParserDIR)/ArgvParser.hh
ArgvParserCC = $(ArgvParserDIR)/ArgvParser.cc

perlishDIR = $(CBRC_CPP_HOME)/utils/perlish/
perlishHH = $(perlishDIR)/perlish.hh
perlishCC = $(perlishDIR)/perlish.cc

FLEArrayDIR = $(CBRC_CPP_HOME)/utils/FLArray
FLEArrayHH = $(FLEArrayDIR)/FLEArray.hh

HDRS = $(ArgvParserHH)

SRCS = $(ArgvParserCC)

HistogramDIR = $(CBRC_CPP_HOME)/utils/histogram/
TrivialUnsignedHistogramHH = $(HistogramDIR)/TrivialUnsignedHistogram.hh

# ***************** Production Rules *************************
tryTrivialUnsignedHistogram : $(SRCS) $(HDRS) tryTrivialUnsignedHistogram.cc \
	$(TrivialUnsignedHistogramHH)
	$(CPP) $(CPP_OPTIONS) -o $@ $@.cc $(SRCS) -I$(CBRC_CPP_HOME)

#xxx : $(SRCS) $(HDRS) xxx.cc
#	$(CPP) $(WARN) $(OPTIMIZE) -o $@ $@.cc $(SRCS) -I$(CBRC_CPP_HOME)

#  Author: Paul B. Horton
#  Organization: Computational Biology Research Center, AIST, Japan
#  Creation Date: : 2006.1.3
#  Last Modification: $Date: 2009/06/14 03:50:22 $
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
CPP_WARN = -Wall
CPP_FLAGS = $(CPP_WARN) $(CPP_DEBUG) $(CPP_OPTIMIZE) -DCBRC_OPTIMIZE=$(CBRC_OPTIMIZE)
CPP = g++

# CPP_LIBS
LIBBOOST_REGEX = /usr/lib/libboost_regex-gcc41-1_34_1.so
CPP_LIBS = -lboost_regex


# ***************** Dependencies *************************
ArgvParserDIR = $(CBRC_CPP_HOME)/utils/argvParsing/
ArgvParserHH = $(ArgvParserDIR)/ArgvParser.hh
ArgvParserCC = $(ArgvParserDIR)/ArgvParser.cc
ArgvParser = $(ArgvParserHH) $(ArgvParserCC)

perlishDIR = $(CBRC_CPP_HOME)/utils/perlish/
perlishHH = $(perlishDIR)/perlish.hh
perlishCC = $(perlishDIR)/perlish.cc
perlish = $(perlishHH) $(perlishCC)

FLEArrayDIR = $(CBRC_CPP_HOME)/utils/FLArray
FLEArrayHH = $(FLEArrayDIR)/FLEArray.hh

HDRS = $(ArgvParserHH)
SRCS = $(ArgvParserCC)
DEPS = $(ArgvParser)


# ***************** Target List *************************
TARGETS =	tryPerlish_split            \
		tryPerlish_splitOnOneMatch  \
		tryLooksLikeReal            \
		tryToDouble                 \
		tryStlCountIf


all : $(TARGETS)


clean :
	-rm -f $(TARGETS)

list :
	@echo $(TARGETS) | sed 's/\s/\n/g'


# ***************** Production Rules *************************
tryPerlish_split : $(perlish) tryPerlish_split.cc
	$(CPP) $(CPP_FLAGS) -o $@ $@.cc $(perlishCC) $(CPP_LIBS) -I$(CBRC_CPP_HOME)

tryPerlish_regexSplit : $(perlish) tryPerlish_regexSplit.cc
	$(CPP) $(CPP_FLAGS) -o $@ $@.cc $(perlishCC) $(CPP_LIBS) -I$(CBRC_CPP_HOME)


tryPerlish_splitOnOneMatch : $(perlish) tryPerlish_splitOnOneMatch.cc
	$(CPP) $(CPP_FLAGS) -o $@ $@.cc $(perlishCC) $(CPP_LIBS) -I$(CBRC_CPP_HOME)


tryLooksLikeReal : $(perlish) tryLooksLikeReal.cc
	$(CPP) $(CPP_FLAGS) -o $@ $@.cc $(perlishCC) $(CPP_LIBS) -I$(CBRC_CPP_HOME)

tryToDouble : $(DEPS) $(perlish) tryToDouble.cc 
	$(CPP) $(CPP_FLAGS) -o $@ $@.cc	\
		$(perlishCC) 		\
		$(SRCS) $(CPP_LIBS) -I$(CBRC_CPP_HOME) 

tryStlCountIf : $(perlish) tryStlCountIf.cc
	$(CPP) $(CPP_FLAGS) -o $@ $@.cc $(perlishCC) $(CPP_LIBS) -I$(CBRC_CPP_HOME)


#xxx : $(SRCS) $(HDRS) xxx.cc
#	$(CPP) $(CPP_FLAGS) -o $@ $@.cc $(SRCS) -I$(CBRC_CPP_HOME)

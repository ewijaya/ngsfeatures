#  Author: Paul B. Horton
#  Organization: Computational Biology Research Center, AIST, Japan
#  Copyright (C) 2008, Paul B. Horton. All rights reserved.
#  Creation Date: 2008.2.28
#  Last Modification: $Date: 2008/07/04 06:05:21 $
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

SerialNumbersForStringsDIR = $(CBRC_CPP_HOME)/utils/serialNumbers/SerialNumbersForStrings
SerialNumbersForStringsHH  = $(SerialNumbersForStringsDIR)/SerialNumbersForStrings.hh
SerialNumbersForStrings_accessCountingHH = $(SerialNumbersForStringsDIR)/SerialNumbersForStrings_accessCounting.hh



# Files most targets depend on
HDRS = $(ArgvParserHH) $(perlishHH)

SRCS = $(perlishCC)


# ***************** Target List *************************
TARGETS =	trySerialNumbersForStrings 			\
		trySerialNumbersForStrings_stringVecConstructor	\
		trySerialNumbersForStrings_accessCounting

all : $(TARGETS)

clean :
	rm -f -i $(TARGETS)

list :
	echo $(TARGETS) | sed 's/\s/\n/g'


# ***************** Production Rules *************************
trySerialNumbersForStrings : $(SRCS) $(HDRS) \
		$(SerialNumbersForStringsHH) \
		trySerialNumbersForStrings.cc
#
	$(CPP) $(CPP_FLAGS) -o $@ $@.cc \
	$(SRCS) $(CPP_LIBS) -I$(CBRC_CPP_HOME)


trySerialNumbersForStrings_stringVecConstructor : $(SRCS) $(HDRS) \
		$(SerialNumbersForStringsHH) \
		trySerialNumbersForStrings_stringVecConstructor.cc
#
	$(CPP) $(CPP_FLAGS) -o $@ $@.cc \
	$(SRCS) $(CPP_LIBS) -I$(CBRC_CPP_HOME)


trySerialNumbersForStrings_accessCounting : $(SRCS) $(HDRS) \
		$(SerialNumbersForStrings_accessCountingHH) \
		trySerialNumbersForStrings_accessCounting.cc
#
	$(CPP) $(CPP_FLAGS) -o $@ $@.cc \
	$(SRCS) $(CPP_LIBS) -I$(CBRC_CPP_HOME)



# production rule template
#xxx : $(SRCS) $(HDRS) xxx.cc
#	$(CPP) $(CPP_FLAGS) -o $@ $@.cc \
#	$(SRCS) $(CPP_LIBS) -I$(CBRC_CPP_HOME)


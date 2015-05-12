#  Author: Paul B. Horton
#  Organization: Computational Biology Research Center, AIST, Japan
#  Copyright (C) 2006, Paul B. Horton. All rights reserved.
#  Creation Date: 2006.12.3
#  Last Modification: $Date: 2009/06/21 00:56:19 $
#  
#  CBRC_CPP_HOME is the root directory of the C++ code. Use environment variable for this:
#  export CBRC_CPP_HOME=~/cvs/cbrc/C++
#

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
CPP_FLAGS = $(CPP_WARN) $(CPP_DEBUG) $(CPP_OPTIMIZE) -DCBRC_OPTIMIZE=$(CBRC_OPTIMIZE)
CPP_LIBS = -lboost_regex
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

stringUtilsDIR = $(CBRC_CPP_HOME)/utils/stl
stringUtilsHH  = $(stringUtilsDIR)/stringUtils.hh


vectorMatrixHH = $(CBRC_CPP_HOME)/utils/stl/vectorMatrix.hh

stringBinaryIODIR = $(CBRC_CPP_HOME)/utils/stl
stringBinaryIOHH  = $(stringBinaryIODIR)/stringBinaryIO.hh


pushbackLiteralDIR = $(CBRC_CPP_HOME)/utils/stl
pushbackLiteralHH  = $(pushbackLiteralDIR)/pushbackLiteral.hh

pushbackLiteralVariadicDIR = $(CBRC_CPP_HOME)/utils/stl
pushbackLiteralVariadicHH  = $(pushbackLiteralVariadicDIR)/pushbackLiteralVariadic.hh


HDRS = $(ArgvParserHH) $(perlishHH)

SRCS = $(perlishCC)


# ***************** Target List *************************
TARGETS = 	tryPushbackLiteral		\
		tryPushbackLiteralVariadic	\
		tryStringUtils_startsEndsWith	\
		tryUnsignedDoubleMap		\
		tryVectorReadWrite		\
		tryStringBinaryIO		\
		tryStringReplaceAll


all : $(TARGETS)

clean:
	rm -f $(TARGETS) 2>/dev/null; true

list :
	@echo $(TARGETS) | sed 's/\s/\n/g'


# ***************** Production Rules *************************
tryPushbackLiteral: $(SRCS) $(HDRS) $$@.cc	\
		$(pushbackLiteralHH)
	$(CPP) $(CPP_FLAGS) -o $@ $@.cc	\
		$(SRCS) $(CPP_LIBS) -I$(CBRC_CPP_HOME)

tryPushbackLiteralVariadic: $(SRCS) $(HDRS) $$@.cc	\
		$(pushbackLiteralVariadicHH)
	$(CPP) $(CPP_FLAGS) -o $@ $@.cc	\
		$(SRCS) $(CPP_LIBS) -I$(CBRC_CPP_HOME)

tryStringUtils_startsEndsWith : $(SRCS) $(HDRS)	\
		$(stringUtilsHH)	\
		tryStringUtils_startsEndsWith.cc
	$(CPP) $(CPP_FLAGS) -o $@ $@.cc	\
		$(SRCS) $(CPP_LIBS) -I$(CBRC_CPP_HOME)

tryUnsignedDoubleMap : \
		tryUnsignedDoubleMap.cc
	$(CPP) $(CPP_FLAGS) -o $@ $@.cc	\
		-I$(CBRC_CPP_HOME)

tryVectorReadWrite : $(SRCS) $(HDRS) $(vectorMatrixHH) \
		tryVectorReadWrite.cc
	$(CPP) $(CPP_FLAGS) -o $@ $@.cc	\
		$(SRCS) $(CPP_LIBS) -I$(CBRC_CPP_HOME)


tryStringBinaryIO : $(SRCS) $(HDRS)   \
		$(stringBinaryIOHH)   \
		tryStringBinaryIO.cc
	$(CPP) $(CPP_FLAGS) -o $@ $@.cc	\
		$(SRCS) $(CPP_LIBS) -I$(CBRC_CPP_HOME)

tryStringReplaceAll : $(SRCS) $(HDRS)	\
		$(stringUtilsHH)	\
		tryStringReplaceAll.cc
	$(CPP) $(CPP_FLAGS) -o $@ $@.cc	\
		$(SRCS) $(CPP_LIBS) -I$(CBRC_CPP_HOME)


# production rule template
#xxx : $(SRCS) $(HDRS)	\
#		xxx.cc
# 	$(CPP) $(CPP_FLAGS) -o $@ $@.cc	\
#		$(SRCS) $(CPP_LIBS) -I$(CBRC_CPP_HOME)


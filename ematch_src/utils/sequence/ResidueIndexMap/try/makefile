#  Author: Paul B. Horton
#  Organization: Computational Biology Research Center, AIST, Japan
#  Copyright (C) 2005, 2008, Paul B. Horton. All rights reserved.
#  Creation Date: 2005.12.07
#  Last Modification: $Date: 2008/08/13 13:14:35 $
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

ArgvParserDIR = $(CBRC_CPP_HOME)/utils/argvParsing/
ArgvParserHH = $(ArgvParserDIR)/ArgvParser.hh
ArgvParserCC = $(ArgvParserDIR)/ArgvParser.cc

perlishDIR = $(CBRC_CPP_HOME)/utils/perlish
perlishHH  = $(perlishDIR)/perlish.hh
perlishCC  = $(perlishDIR)/perlish.cc
perlish    = $(perlishHH) $(perlishCC)


stringBinaryIODIR = $(CBRC_CPP_HOME)/utils/stl
stringBinaryIOHH  = $(stringBinaryIODIR)/stringBinaryIO.hh


ResidueIndexMapHH = ../ResidueIndexMap.hh
ResidueIndexMapCC = ../ResidueIndexMap.cc
ResidueIndexMap = $(ResidueIndexMapHH) $(ResidueIndexMapCC)

ResidueIndexMapPredefinedHH = ../ResidueIndexMapPredefined.hh

ResiduesIndexMapHH = ../ResiduesIndexMap.hh
ResiduesIndexMapCC = ../ResiduesIndexMap.cc
ResiduesIndexMap = $(ResiduesIndexMapHH) $(ResiduesIndexMapCC)

ResidueIndexMapTransformerDIR = $(CBRC_CPP_HOME)/utils/sequence/ResidueIndexMap
ResidueIndexMapTransformerHH  = $(ResidueIndexMapTransformerDIR)/ResidueIndexMapTransformer.hh
ResidueIndexMapTransformerCC  = $(ResidueIndexMapTransformerDIR)/ResidueIndexMapTransformer.cc
ResidueIndexMapTransformer    = $(ResidueIndexMapTransformerHH) $(ResidueIndexMapTransformerCC)


ResidueIndexMapTranslatorDIR = $(CBRC_CPP_HOME)/utils/sequence/ResidueIndexMap
ResidueIndexMapTranslatorHH  = $(ResidueIndexMapTranslatorDIR)/ResidueIndexMapTranslator.hh
ResidueIndexMapTranslatorCC  = $(ResidueIndexMapTranslatorDIR)/ResidueIndexMapTranslator.cc
ResidueIndexMapTranslator    = $(ResidueIndexMapTranslatorHH) $(ResidueIndexMapTranslatorCC)



# headers and source files common to most targets
HDRS = $(ArgvParserHH) $(perlishHH) $(ResidueIndexMapHH) $(stringBinaryIOHH)
SRCS = $(ArgvParserCC) $(perlishCC) $(ResidueIndexMapCC)


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


# ***************** Target List *************************
TARGETS = \
	tryResidueIndexMap					\
	tryResidueIndexMap_binaryIO				\
	tryResidueIndexMap_constructorArgumentChecks		\
	tryResidueIndexMap_extendAlphabet			\
	tryExtendedDNAAlphabetSizeMethods			\
	tryResidueIndexMap_toResidueIndices			\
        tryResidueIndexMap_splitIntoProperOnlySubstrings	\
	tryResiduesIndexMap					\
	tryResidueIndexMapTransformer				\
	tryResidueIndexMapTranslator

all : $(TARGETS)

clean :
	rm -f -i $(TARGETS)

list :
	echo $(TARGETS) | sed 's/\s/\n/g'



# ***************** Production Rules *************************
tryResidueIndexMap : $(SRCS) $(HDRS)			\
		$(ResidueIndexMapPredefinedAlphabetsHH)	\
		tryResidueIndexMap.cc
	$(CPP) $(CPP_FLAGS) -o $@ $@.cc		\
		$(SRCS) $(CPP_LIBS) -I$(CBRC_CPP_HOME)


tryResidueIndexMap_binaryIO : $(SRCS) $(HDRS) 		\
		$(ResidueIndexMapPredefinedAlphabetsHH) \
		tryResidueIndexMap_binaryIO.cc
	$(CPP) $(CPP_FLAGS) -o $@ $@.cc	\
		$(SRCS) $(CPP_LIBS) -I$(CBRC_CPP_HOME)


tryResidueIndexMap_constructorArgumentChecks : $(SRCS) $(HDRS) 	\
		$(ResidueIndexMapPredefinedAlphabetsHH) 	\
		tryResidueIndexMap_constructorArgumentChecks.cc
	$(CPP) $(CPP_FLAGS) -o $@ $@.cc	\
		$(SRCS) $(CPP_LIBS) -I$(CBRC_CPP_HOME)


tryResidueIndexMap_extendAlphabet : $(SRCS) $(HDRS)		\
		$(ResidueIndexMapPredefinedAlphabetsHH) 	\
		tryResidueIndexMap_extendAlphabet.cc
	$(CPP) $(CPP_FLAGS) -o $@ $@.cc	\
		$(SRCS) $(CPP_LIBS) -I$(CBRC_CPP_HOME)


tryExtendedDNAAlphabetSizeMethods : $(SRCS) $(HDRS) 	\
		tryExtendedDNAAlphabetSizeMethods.cc
	$(CPP) $(CPP_FLAGS) -o $@ $@.cc	\
		$(SRCS) $(CPP_LIBS) -I$(CBRC_CPP_HOME)


tryResidueIndexMap_toResidueIndices : $(SRCS) $(HDRS) \
		tryResidueIndexMap_toResidueIndices.cc
	$(CPP) $(CPP_FLAGS) -o $@ $@.cc \
		$(SRCS) $(CPP_LIBS) -I$(CBRC_CPP_HOME)


tryResidueIndexMap_splitIntoProperOnlySubstrings : $(SRCS) $(HDRS) \
		tryResidueIndexMap_splitIntoProperOnlySubstrings.cc
	$(CPP) $(CPP_FLAGS) -o $@ $@.cc	\
		$(SRCS) $(CPP_LIBS) -I$(CBRC_CPP_HOME)


tryResiduesIndexMap : $(SRCS) $(HDRS)	\
		$(ResiduesIndexMap)	\
		tryResiduesIndexMap.cc
	$(CPP) $(CPP_FLAGS) -o $@ $@.cc		\
		$(ResiduesIndexMapCC)		\
		$(SRCS) $(CPP_LIBS) -I$(CBRC_CPP_HOME)


tryResidueIndexMapTransformer : $(SRCS) $(HDRS) \
		$(ResidueIndexMapTransformer)	\
		tryResidueIndexMapTransformer.cc
	$(CPP) $(CPP_FLAGS) -o $@ $@.cc 	\
		$(ResidueIndexMapTransformerCC)	\
		$(SRCS) $(CPP_LIBS) -I$(CBRC_CPP_HOME)


tryResidueIndexMapTranslator : $(SRCS) $(HDRS) \
		$(ResidueIndexMapTranslator)	\
		tryResidueIndexMapTranslator.cc
	$(CPP) $(CPP_FLAGS) -o $@ $@.cc 	\
		$(ResidueIndexMapTranslatorCC)	\
		$(SRCS) $(CPP_LIBS) -I$(CBRC_CPP_HOME)

#xxx : $(SRCS) $(HDRS) 		\
# 		xxx.cc
# 	$(CPP) $(CPP_FLAGS) -o $@ $@.cc 	\
# 		$(SRCS) $(CPP_LIBS) -I$(CBRC_CPP_HOME)


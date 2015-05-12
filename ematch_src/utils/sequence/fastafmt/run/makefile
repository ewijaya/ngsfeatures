#  Author: Paul B. Horton
#  Organization: Computational Biology Research Center, AIST, Japan
#  Copyright (C) 2007, Paul B. Horton. All rights reserved.
#  Creation Date: 2007.10.13
#  Last Modification: $Date: 2009/05/21 13:14:51 $
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

FastaReaderDIR = $(CBRC_CPP_HOME)/utils/sequence/readers
FastaReaderHH  = $(FastaReaderDIR)/FastaReader.hh
FastaReaderCC  = $(FastaReaderDIR)/FastaReader.cc
FastaReader    = $(FastaReaderHH) $(FastaReaderCC)

fastafmtFunctionsDIR = $(CBRC_CPP_HOME)/utils/sequence/fastafmt
fastafmtFunctionsHH  = $(fastafmtFunctionsDIR)/fastafmtFunctions.hh
fastafmtFunctionsCC  = $(fastafmtFunctionsDIR)/fastafmtFunctions.cc
fastafmtFunctions    = $(fastafmtFunctionsHH) $(fastafmtFunctionsCC)


ResidueIndexMapDIR = $(CBRC_CPP_HOME)/utils/sequence/ResidueIndexMap
ResidueIndexMapHH  = $(ResidueIndexMapDIR)/ResidueIndexMap.hh
ResidueIndexMapCC  = $(ResidueIndexMapDIR)/ResidueIndexMap.cc
ResidueIndexMap    = $(ResidueIndexMapHH) $(ResidueIndexMapCC)

ResidueIndexMapPredefinedAlphabetsDIR = $(CBRC_CPP_HOME)/utils/sequence/ResidueIndexMap
ResidueIndexMapPredefinedAlphabetsHH  = $(ResidueIndexMapPredefinedAlphabetsDIR)/ResidueIndexMapPredefinedAlphabets.hh

LabeledSequenceDIR = $(CBRC_CPP_HOME)/utils/sequence
LabeledSequenceHH  = $(LabeledSequenceDIR)/LabeledSequence.hh
LabeledSequenceCC  = $(LabeledSequenceDIR)/LabeledSequence.cc
LabeledSequence    = $(LabeledSequenceHH) $(LabeledSequenceCC)



# Files most targets depend on
HDRS =  $(ArgvParserHH) $(perlishHH)		\
	$(ResidueIndexMapPredefinedAlphabetsHH)	\
	$(ResidueIndexMapHH)                    \
	$(LabeledSequenceHH)			\
	$(FastaReaderHH)

SRCS =  $(perlishCC)		\
	$(FastaReaderCC)	\
n	$(LabeledSequenceCC)	\
	$(ResidueIndexMapCC)


# ***************** Target List *************************
TARGETS =	fastafmtCountRecordsAndBases


all : $(TARGETS)

clean :
	rm -f $(TARGETS) 2>/dev/null; true

list :
	echo $(TARGETS) | sed 's/\s/\n/g'

# ***************** Production Rules *************************




fastafmtCountRecordsAndBases : $(SRCS) $(HDRS) fastafmtCountRecordsAndBases.cc
	$(CPP) $(CPP_FLAGS) -o $@ $@.cc $(SRCS) $(CPP_LIBS) -I$(CBRC_CPP_HOME)

# production rule template
#xxx : $(SRCS) $(HDRS) xxx.cc
#	$(CPP) $(CPP_FLAGS) -o $@ $@.cc $(SRCS) -I$(CBRC_CPP_HOME)

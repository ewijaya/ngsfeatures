#  Author: Paul Horton
#  Organization: Computational Biology Research Center, AIST, Japan
#  Copyright (C) 2009, Paul Horton. All rights reserved.
#  Creation Date: 2009.5.1
#  Last Modification: $Date: 2009/05/17 10:49:34 $
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
ArgvParserDIR = $(CBRC_CPP_HOME)/utils/argvParsing
ArgvParserHH  = $(ArgvParserDIR)/ArgvParser.hh

perlishDIR = $(CBRC_CPP_HOME)/utils/perlish
perlishHH  = $(perlishDIR)/perlish.hh
perlishCC  = $(perlishDIR)/perlish.cc
perlish    = $(perlishHH) $(perlishCC)

FLEArrayDIR = $(CBRC_CPP_HOME)/utils/FLArray
FLEArrayHH  = $(FLEArrayDIR)/FLEArray.hh


# Files most targets depend on
HDRS = $(ArgvParserHH) $(perlishHH)

SRCS = $(perlishCC)


# ********** FASTA DEPENDENCIES **********
ResidueIndexMapDIR = $(CBRC_CPP_HOME)/utils/sequence/ResidueIndexMap
ResidueIndexMapHH  = $(ResidueIndexMapDIR)/ResidueIndexMap.hh
ResidueIndexMapCC  = $(ResidueIndexMapDIR)/ResidueIndexMap.cc
ResidueIndexMap    = $(ResidueIndexMapHH) $(ResidueIndexMapCC)

ResidueIndexMapPredefinedDIR = $(CBRC_CPP_HOME)/utils/sequence/ResidueIndexMap
ResidueIndexMapPredefinedHH  = $(ResidueIndexMapPredefinedDIR)/ResidueIndexMapPredefined.hh

ResidueIndexMapPredefinedDEPS = $(ResidueIndexMap) $(ResidueIndexMapPredefinedHH)


LabeledSequenceDIR = $(CBRC_CPP_HOME)/utils/sequence
LabeledSequenceHH  = $(LabeledSequenceDIR)/LabeledSequence.hh
LabeledSequenceCC  = $(LabeledSequenceDIR)/LabeledSequence.cc
LabeledSequence    = $(LabeledSequenceHH) $(LabeledSequenceCC)

LabeledSequenceDEPS = $(ResidueIndexMapDEPS) $(LabeledSequence)
LabeledSequenceSRCS = $(ResidueIndexMapCC) $(LabeledSequenceCC)

LabeledAsciiSeqDIR = $(CBRC_CPP_HOME)/utils/sequence
LabeledAsciiSeqHH  = $(LabeledAsciiSeqDIR)/LabeledAsciiSeq.hh


FastaClassesDIR = $(CBRC_CPP_HOME)/utils/sequence/readers/fasta
FastaInputOptionsHH  = $(FastaClassesDIR)/FastaInputOptions.hh

FastaRecordHH  = $(FastaClassesDIR)/FastaRecord.hh
FastaRecordCC  = $(FastaClassesDIR)/FastaRecord.cc
FastaRecord    = $(FastaRecordHH) $(FastaRecordCC)

FastaRecordReaderHH   = $(FastaClassesDIR)/FastaRecordReader.hh
FastaRecordReaderCC   = $(FastaClassesDIR)/FastaRecordReader.cc
FastaRecordReader     = $(FastaRecordReaderHH) $(FastaRecordReaderCC)
FastaRecordReaderSRCS = $(FastaRecordCC) $(FastaRecordReaderCC)
FastaRecordReaderDEPS = $(FastaRecord) $(FastaRecordReader) $(fastafmtHH)


FastaSeqSlurperDIR = $(CBRC_CPP_HOME)/utils/sequence/readers/fasta
FastaSeqSlurperHH  = $(FastaSeqSlurperDIR)/FastaSeqSlurper.hh
FastaSeqSlurperCC  = $(FastaSeqSlurperDIR)/FastaSeqSlurper.cc
FastaSeqSlurper    = $(FastaSeqSlurperHH) $(FastaSeqSlurperCC)

FastaSeqSlurperDEPS = 	$(FastaRecordReaderDEPS)	\
			$(LabeledAsciiSeqHH)		\
			$(LabeledSequenceDEPS)		\
			$(FastaSeqSlurper)

FastaSeqSlurperSRCS = 	$(FastaRecordReaderSRCS)	\
			$(LabeledSequenceSRCS)		\
			$(FastaSeqSlurperCC)



# ***************** Target List *************************
TARGETS =	tryFastaInputOptions				\
		tryFastaRecordReader				\
		tryFastaRecordReader_customizeSeqCharFilter	\
		tryFastaSeqSlurper


# *************** Phony Targets ***************
.PHONY : all clean list

all : $(TARGETS)

clean :
	rm -f $(TARGETS) 2>/dev/null; true

list :
	@echo $(TARGETS) | sed 's/\s/\n/g'



# ***************** Production Rules *************************

tryFastaInputOptions : $(SRCS) $(HDRS)	$$@.cc	\
		$(FastaInputOptionsHH)
	$(CPP) $(CPP_FLAGS) -o $@ $@.cc		\
		$(SRCS) $(CPP_LIBS) -I$(CBRC_CPP_HOME)


tryFastaRecordReader : $(SRCS) $(HDRS)	$$@.cc	\
		$(FastaRecordReaderDEPS)
	$(CPP) $(CPP_FLAGS) -o $@ $@.cc		\
		$(FastaRecordReaderSRCS)	\
		$(SRCS) $(CPP_LIBS) -I$(CBRC_CPP_HOME)


tryFastaRecordReader_customizeSeqCharFilter : $(SRCS) $(HDRS) $$@.cc	\
		$(FastaRecordReaderDEPS)
	$(CPP) $(CPP_FLAGS) -o $@ $@.cc		\
		$(FastaRecordReaderSRCS)	\
		$(SRCS) $(CPP_LIBS) -I$(CBRC_CPP_HOME)


tryFastaSeqSlurper : $(SRCS) $(HDRS) $$@.cc	\
		$(FastaSeqSlurperDEPS)
	$(CPP) $(CPP_FLAGS) -o $@ $@.cc		\
		$(FastaSeqSlurperSRCS)		\
		$(SRCS) $(CPP_LIBS) -I$(CBRC_CPP_HOME)


# production rule template
#xxx : $(SRCS) $(HDRS) $@@.cc			\
#		prerequisites...
#	$(CPP) $(CPP_FLAGS) -o $@ $@.cc 	\
#		$(SRCS) $(CPP_LIBS) -I$(CBRC_CPP_HOME)


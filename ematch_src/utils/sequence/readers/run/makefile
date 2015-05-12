# Author: Paul Horton
# Created: 2003 or 2004.
# Last Modified: $Date: 2009/05/21 11:57:46 $
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

CPP_OPTIMIZE = -O3
CPP_DEBUG = -g
CPP_WARN = -Wall
CPP_FLAGS = $(CPP_WARN) $(CPP_DEBUG) $(CPP_OPTIMIZE) -DCBRC_OPTIMIZE=$(CBRC_OPTIMIZE)
CPP_LIBS = -lboost_regex
CPP=g++



# ***************** Dependencies *************************


# *** Files most targets depend on ***
perlishDIR = $(CBRC_CPP_HOME)/utils/perlish
perlishHH  = $(perlishDIR)/perlish.hh
perlishCC  = $(perlishDIR)/perlish.cc

ArgvParserDIR = $(CBRC_CPP_HOME)/utils/argvParsing/
ArgvParserHH = $(ArgvParserDIR)/ArgvParser.hh
ArgvParserCC = $(ArgvParserDIR)/ArgvParser.cc

FLEArrayDIR = $(CBRC_CPP_HOME)/utils/FLArray
FLEArrayHH  = $(FLEArrayDIR)/FLEArray.hh

UniformScalerDIR = $(CBRC_CPP_HOME)/classifiers
UniformScalerHH  = $(UniformScalerDIR)/UniformScaler.hh
UniformScalerCC  = $(UniformScalerDIR)/UniformScaler.cc

HDRS =  $(perlishHH) $(ArgvParserHH) $(FLEArrayHH) \
	$(UniformScalerHH)

SRCS =  $(perlishCC) $(ArgvParserCC)	\
	$(UniformScalerCC)


Blast8ResultReaderDIR = $(CBRC_CPP_HOME)/utils/sequence/readers
Blast8ResultReaderHH  = $(Blast8ResultReaderDIR)/Blast8ResultReader.hh
Blast8ResultReaderCC  = $(Blast8ResultReaderDIR)/Blast8ResultReader.cc
Blast8ResultReader    = $(Blast8ResultReaderHH) $(Blast8ResultReaderCC)

BlastResultHDRS = $(Blast8ResultReaderHH) $(BlastEValuesByClassHH)
BlastResultSRCS = $(Blast8ResultReaderCC) $(BlastEValuesByClassCC)
BlastResultDEPS = $(BlastResultHDRS) $(BlastResultSRCS)

BlastEValuesByClassDIR = $(CBRC_CPP_HOME)/utils/sequence/readers
BlastEValuesByClassHH =  $(BlastEValuesByClassDIR)/BlastEValuesByClass.hh
BlastEValuesByClassCC =  $(BlastEValuesByClassDIR)/BlastEValuesByClass.cc

BlastEValuesByClassSRCS = $(BlastEValuesByClassCC) $(BlastResultSRCS)
BlastEValuesByClassDEPS = $(BlastResultDEPS) $(LabeledInstancesDEPS)

ResidueIndexMapDIR = $(CBRC_CPP_HOME)/utils/sequence/ResidueIndexMap
ResidueIndexMapHH  = $(ResidueIndexMapDIR)/ResidueIndexMap.hh
ResidueIndexMapCC  = $(ResidueIndexMapDIR)/ResidueIndexMap.cc
ResidueIndexMap    = $(ResidueIndexMapHH) $(ResidueIndexMapCC)

ResidueIndexMapPredefinedHH   = $(ResidueIndexMapDIR)/ResidueIndexMapPredefined.hh
ResidueIndexMapPredefinedHDRS = $(ResidueIndexMapHH) $(ResidueIndexMapPredefinedHH)
ResidueIndexMapPredefinedSRCS = $(ResidueIndexMapCC)
ResidueIndexMapPredefinedDEPS = $(ResidueIndexMap) $(ResidueIndexMapPredefinedHH)

LabeledSequenceDIR  = $(CBRC_CPP_HOME)/utils/sequence
LabeledSequenceHH   = $(LabeledSequenceDIR)/LabeledSequence.hh
LabeledSequenceCC   = $(LabeledSequenceDIR)/LabeledSequence.cc
LabeledSequence     = $(LabeledSequenceHH) $(LabeledSequenceCC)
LabeledSequenceSRCS = $(ResidueIndexMapPredefinedSRCS) $(LabeledSequenceCC)
LabeledSequenceDEPS = $(ResidueIndexMapPredefinedDEPS) $(LabeledSequence)

LabeledAsciiSeqDIR = $(CBRC_CPP_HOME)/utils/sequence
LabeledAsciiSeqHH  = $(LabeledAsciiSeqDIR)/LabeledAsciiSeq.hh

LabeledAsciiSeqPairPredicateDIR = $(CBRC_CPP_HOME)/utils/sequence
LabeledAsciiSeqPairPredicateHH  = $(LabeledAsciiSeqPairPredicateDIR)/LabeledAsciiSeqPairPredicate.hh


FeatureNamesDIR = $(CBRC_CPP_HOME)/classifiers
FeatureNamesHH  = $(FeatureNamesDIR)/FeatureNames.hh
FeatureNamesCC  = $(FeatureNamesDIR)/FeatureNames.cc
FeatureNames    = $(FeatureNamesHH) $(FeatureNamesCC)


FeatureScalersDIR  = $(CBRC_CPP_HOME)/classifiers
FeatureScalersHH   = $(FeatureScalersDIR)/FeatureScalers.hh
FeatureScalersCC   = $(FeatureScalersDIR)/FeatureScalers.cc
FeatureScalersSRCS = $(FeatureScalersCC)
FeatureScalersDEPS = $(FeatureScalersHH) $(FeatureScalersCC)

SortedStringSetDIR = $(CBRC_CPP_HOME)/utils/SortedStringSet
SortedStringSetHH  = $(SortedStringSetDIR)/SortedStringSet.hh


LabeledInstanceDIR    = $(CBRC_CPP_HOME)/classifiers
UnlabeledInstanceCC   = $(LabeledInstanceDIR)/UnlabeledInstance.cc
UnlabeledInstance     = $(LabeledInstanceHH) $(UnlabeledInstanceCC)

UnlabeledInstanceHDRS = $(UnlabeledInstanceHH)
UnlabeledInstanceSRCS = $(UnlabeledInstanceCC)

LabeledInstanceHH    = $(LabeledInstanceDIR)/LabeledInstance.hh
LabeledInstanceCC    = $(LabeledInstanceDIR)/LabeledInstance.cc
LabeledInstance      = $(LabeledInstanceHH) $(LabeledInstanceCC)

LabeledInstanceHDRS = $(UnlabeledInstanceHDRS) $(LabeledInstanceHH)
LabeledInstanceSRCS = $(UnlabeledInstanceSRCS) $(LabeledInstanceCC)
LabeledInstanceDEPS = $(LabeledInstanceHDRS) $(LabeledInstanceSRCS)


FLEMatrixDIR = $(CBRC_CPP_HOME)/utils/FLArray
FLEMatrixHH  = $(FLEMatrixDIR)/FLEMatrix.hh

SerialNumbersForStringsDIR = $(CBRC_CPP_HOME)/utils/serialNumbers/SerialNumbersForStrings
SerialNumbersForStringsHH  = $(SerialNumbersForStringsDIR)/SerialNumbersForStrings.hh

SerialNumbersForStrings_accessCountingHH	\
	= $(SerialNumbersForStringsDIR)/SerialNumbersForStrings_accessCounting.hh

SerialNumbersForStrings_accessCountingHDRS	\
	= $(SerialNumbersForStringsHH) $(SerialNumbersForStrings_accessCountingHH)


SortedStringSetDIR = $(CBRC_CPP_HOME)/utils/SortedStringSet
SortedStringSetHH  = $(SortedStringSetDIR)/SortedStringSet.hh


LabeledInstancesDIR = $(CBRC_CPP_HOME)/classifiers
LabeledInstancesHH  = $(LabeledInstancesDIR)/LabeledInstances.hh
LabeledInstancesCC  = $(LabeledInstancesDIR)/LabeledInstances.cc
LabeledInstances    = $(LabeledInstancesHH) $(LabeledInstancesCC)

LabeledInstancesHDRS =	$(FLEMatrixHH)					\
			$(FeatureNamesHH)				\
			$(SerialNumbersForStrings_accessCountingHDRS)	\
			$(SortedStringSetHH)				\
			$(LabeledInstanceHDRS)				\
			$(FeatureScalersHH)


LabeledInstancesSRCS =  $(FeatureNamesCC)				\
			$(LabeledInstanceSRCS)				\
			$(FeatureScalersSRCS)				\
			$(LabeledInstancesCC)

LabeledInstancesDEPS = 	$(LabeledInstanceDEPS) $(LabeledInstances)


LabeledAsciiSeqDIR = $(CBRC_CPP_HOME)/utils/sequence
LabeledAsciiSeqHH  = $(LabeledAsciiSeqDIR)/LabeledAsciiSeq.hh


smoothingDIR = $(CBRC_CPP_HOME)/utils/geometry
smoothingHH  = $(smoothingDIR)/smoothing.hh
smoothingCC  = $(smoothingDIR)/smoothing.cc
smoothing    = $(smoothingHH) $(smoothingCC)

fastaDIR = $(CBRC_CPP_HOME)/utils/sequence/readers/fasta

fastafmtHH  = $(fastaDIR)/fastafmt.hh

FastaWriterHH   = $(fastaDIR)/FastaWriter.hh
FastaWriterCC   = $(fastaDIR)/FastaWriter.cc
FastaWriter     = $(FastaWriterHH) $(FastaWriterCC)
FastaWriterDEPS = $(FastaWriter) $(fastafmtHH)

SprotReaderDIR = $(CBRC_CPP_HOME)/utils/sequence/readers/
SprotReaderHH  = $(SprotReaderDIR)/SprotReader.hh
SprotReaderCC  = $(SprotReaderDIR)/SprotReader.cc
SprotReader    = $(SprotReaderHH) $(SprotReaderCC)



ConstantLabeledSquareDoubleFLENumMatrixDIR = $(CBRC_CPP_HOME)/utils/FLArray
ConstantLabeledSquareDoubleFLENumMatrixHH  = $(ConstantLabeledSquareDoubleFLENumMatrixDIR)/ConstantLabeledSquareDoubleFLENumMatrix.hh
ConstantLabeledSquareDoubleFLENumMatrixCC  = $(ConstantLabeledSquareDoubleFLENumMatrixDIR)/ConstantLabeledSquareDoubleFLENumMatrix.cc
ConstantLabeledSquareDoubleFLENumMatrix    = $(ConstantLabeledSquareDoubleFLENumMatrixHH) $(ConstantLabeledSquareDoubleFLENumMatrixCC)




# ***************** Target List *************************
TARGETS =   	sprotToFasta				\
		sprotGrepOC				\
		printBlast8ResultNegLgEValues		\
		printNormalizedBlastSimilarity		\
		filterBlast8ResultEValueThreshold	\
		filterSameClassHighBlast		\
		labeledBlastToOddsRatio			\
		runDumpBlastNNUtility


# *************** Phony Targets ***************
.PHONY : all clean list

all : $(TARGETS)

clean :
	rm -f $(TARGETS) 2>/dev/null; true

list :
	@echo $(TARGETS) | sed 's/\s/\n/g'



# ***************** Production Rules *************************

sprotToFasta : $(SRCS) $(HDRS) $$@.cc	\
		$(SprotReader)		\
		$(FastaWriterDEP)
	$(CPP) $(CPP_FLAGS) -o $@ $@.cc \
		$(SprotReaderCC)	\
		$(FastaWriterCC)	\
		$(SRCS) $(CPP_LIBS) -I$(CBRC_CPP_HOME)

sprotGrepOC : $(SRCS) $(HDRS) $$@.cc	\
		$(SprotReader)		\
		$(FastaWriterDEP)
	$(CPP) $(CPP_FLAGS) -o $@ $@.cc	\
		$(SprotReaderCC)	\
		$(FastaWriterCC)	\
		$(SRCS) $(CPP_LIBS) -I$(CBRC_CPP_HOME)


printBlast8ResultNegLgEValues : $(SRCS)	$$@.cc	\
		$(Blast8ResultReader)
	g++ $(CPP_FLAGS) -o $@ $@.cc		\
		$(Blast8ResultReaderCC) 	\
		$(SRCS) $(CPP_LIBS) -I$(CBRC_CPP_HOME)


printNormalizedBlastSimilarity : $(SRCS) $$@.cc	\
		$(Blast8ResultReader)
	g++ $(CPP_FLAGS) -o $@ $@.cc		\
		$(Blast8ResultReaderCC)		\
		$(SRCS) $(CPP_LIBS) -I$(CBRC_CPP_HOME)


filterBlast8ResultEValueThreshold : $(SRCS) $$@.cc	\
		$(Blast8ResultReader)
	g++ $(CPP_FLAGS) -o $@ $@.cc		\
		$(Blast8ResultReaderCC)		\
		$(SRCS) $(CPP_LIBS) -I$(CBRC_CPP_HOME)


filterSameClassHighBlast : $(SRCS) $(HDRS) $$@.cc	\
		$(Blast8ResultReader)			\
		$(LabeledInstancesDEPS)			\
		$(CBRC_CPP_HOME)/utils/math/StayFiniteLog.hh
	g++ $(CPP_FLAGS) -o $@ $@.cc		\
		$(Blast8ResultReaderCC)		\
		$(LabeledInstancesSRCS)		\
		$(SRCS) $(CPP_LIBS) -I$(CBRC_CPP_HOME)


labeledBlastToOddsRatio : $(SRCS) $(HDRS) $$@.cc			\
		$(BlastResultDEPS) $(LabeledInstancesDEPS) 		\
		$(CBRC_CPP_HOME)/utils/geometry/WeightedPoint2D.hh 	\
		$(CBRC_CPP_HOME)/utils/geometry/Point2D.hh         	\
		$(smoothing) $(CBRC_CPP_HOME)/utils/math/StayFiniteLog.hh
	g++ $(CPP_FLAGS) -o $@ $@.cc				\
		$(LabeledInstancesSRCS) $(smoothingCC)		\
		$(SRCS) $(BlastResultSRCS) $(CPP_LIBS) -I$(CBRC_CPP_HOME)


runDumpBlastNNUtility : $(SRCS) $(HDRS) $$@.cc				\
		$(BlastResultDEPS) $(LabeledInstancesDEPS) 		\
		$(ConstantLabeledSquareDoubleFLENumMatrix)
	$(CPP) $(CPP_FLAGS) -o $@ $@.cc				\
		$(ConstantLabeledSquareDoubleFLENumMatrixCC) 	\
		$(BlastResultSRCS) $(LabeledInstancesSRCS)	\
		$(SRCS) $(CPP_LIBS) -I$(CBRC_CPP_HOME)



# production rule template
#xxx : $(SRCS) $(HDRS) $$@.cc			\
#		prerequisites...
#	$(CPP) $(CPP_FLAGS) -o $@ $@.cc 	\
#		$(SRCS) $(CPP_LIBS) -I$(CBRC_CPP_HOME)

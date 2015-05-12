#  Author: Paul B. Horton
#  Organization: Computational Biology Research Center, AIST, Japan
#  Copyright (C) 2007, Paul B. Horton. All rights reserved.
#  Creation Date: 2007
#  Last Modification: $Date: 2009/04/20 21:46:32 $
#  
#  CBRC_CPP_HOME is the root directory of the C++ code. Use environment variable for this:
#  export CBRC_CPP_HOME=~/cvs/cbrc/C++

FLArrayDIR  = $(CBRC_CPP_HOME)/utils/FLArray
FLArrayHH   = $(FLArrayDIR)/FLArray.hh
FLEArrayHH  = $(FLArrayDIR)/FLEArray.hh

ConstantLabeledSquareDoubleFLENumMatrixDIR = $(CBRC_CPP_HOME)/utils/FLArray
ConstantLabeledSquareDoubleFLENumMatrixHH  = $(ConstantLabeledSquareDoubleFLENumMatrixDIR)/ConstantLabeledSquareDoubleFLENumMatrix.hh
ConstantLabeledSquareDoubleFLENumMatrixCC  = $(ConstantLabeledSquareDoubleFLENumMatrixDIR)/ConstantLabeledSquareDoubleFLENumMatrix.cc
ConstantLabeledSquareDoubleFLENumMatrix    = $(ConstantLabeledSquareDoubleFLENumMatrixHH) $(ConstantLabeledSquareDoubleFLENumMatrixCC)

FLSortedFunctionsDIR = $(CBRC_CPP_HOME)/utils/FLArray
FLSortedFunctionsHH  = $(FLSortedFunctionsDIR)/FLSortedFunctions.hh
FLSortedFunctionsCC  = $(FLSortedFunctionsDIR)/FLSortedFunctions.cc
FLSortedFunctions    = $(FLSortedFunctionsHH) $(FLSortedFunctionsCC)

FLSortedFunctionsDIR = $(CBRC_CPP_HOME)/utils/FLArray
FLSortedFunctionsHH  = $(FLSortedFunctionsDIR)/FLSortedFunctions.hh
FLSortedFunctionsCC  = $(FLSortedFunctionsDIR)/FLSortedFunctions.cc
FLSortedFunctions    = $(FLSortedFunctionsHH) $(FLSortedFunctionsCC)

FLArrayFunctionsDIR = $(CBRC_CPP_HOME)/utils/FLArray
FLArrayFunctionsHH  = $(FLArrayFunctionsDIR)/FLArrayFunctions.hh

FLESortedDIR = $(CBRC_CPP_HOME)/utils/FLArray
FLESortedHH  = $(FLESortedDIR)/FLESorted.hh



SRCS = $(CBRC_CPP_HOME)/utils/perlish/perlish.cc 		\
	$(CBRC_CPP_HOME)/utils/argvParsing/ArgvParser.cc

HDRS = 	$(CBRC_CPP_HOME)/utils/perlish/perlish.hh 		\
	$(CBRC_CPP_HOME)/utils/argvParsing/ArgvParser.hh


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


CPP = g++
CPP_OPTIMIZE = -O3
CPP_DEBUG = -g
CPP_WARN = -Wall
CPP_FLAGS = $(CPP_WARN) $(CPP_DEBUG) $(CPP_OPTIMIZE) -DCBRC_OPTIMIZE=$(CBRC_OPTIMIZE)
CPP_LIBS = -lboost_regex



# ***************** Target Lists *************************
tryFLArray_TARGETS = 	tryFLArray

tryFLEArray_TARGETS = 	tryFLEArray 				\
			tryFLEArray_constructFromString		\
			tryFLEArrayLiteralConstructors		\
			tryFLEArray_print			\
			tryFLEArray_printOperator		\
			tryFLEArray_resize			\
			tryFLEArray_fill			\
			tryFLEArray_fillWithByte		\
			tryFLEArray_find			\
			tryFLEArray_replace			\
			tryFLENumArray_operator_assign		\
			timeFLENumArray_operator_assign		\
			tryFLENumArray_operator_equal           \
			tryFLENumArray_max			\
			tryFLENumArray_sqrtTransform


TARGETS = \
	$(tryFLEArray_TARGETS)						\
	tryFLSorted                                                     \
	tryFLESorted                                                    \
	tryFLSorted2TypePairSet                                         \
	tryFLSorted2TypePairSet_readFromTextFile                        \
	tryFLSortedIO                                                   \
	tryFLESorted_readWrite						\
	tryConstantLabeledSquareDoubleFLENumMatrix                      \
	tryConstantLabeledSquareDoubleFLENumMatrix_renumLabels          \
	tryConstantLabeledSquareDoubleFLENumMatrix_renumLabels_SerialNumbersForStrings_accessCounting \
	tryConstantLabeledSquareDoubleFLENumMatrix_labeledIdentityDoubleMatrix


all : $(TARGETS)

clean :
	rm -f $(TARGETS) 2>/dev/null; true

list :
	@echo $(TARGETS) | sed 's/\s/\n/g'



# ***************** Production Rules *************************
$(tryFLArray_TARGETS) : %: %.cc $(SRCS) $(HDRS)	\
		$(FLArrayHH)
	$(CPP) $(CPP_FLAGS) -o $@ $@.cc $(SRCS)	$(CPP_LIBS) -I$(CBRC_CPP_HOME)

$(tryFLEArray_TARGETS) : %: %.cc $(SRCS) $(HDRS)	\
		$(FLEArrayHH)
	$(CPP) $(CPP_FLAGS) -o $@ $@.cc $(SRCS)	$(CPP_LIBS) -I$(CBRC_CPP_HOME)


tryFLSorted : $(SRCS) $(HDRS) tryFLSorted.cc
	$(CPP) $(CPP_FLAGS) -o $@ $@.cc -I$(CBRC_CPP_HOME)

tryFLESorted : $(SRCS) $(HDRS) $(FLESortedHH) tryFLESorted.cc
	$(CPP) $(CPP_FLAGS) -o $@ $@.cc -I$(CBRC_CPP_HOME)


tryFLSorted2TypePairSet : $(SRCS) $(HDRS) tryFLSorted2TypePairSet.cc $(CBRC_CPP_HOME)/utils/FLArray/FLSorted2TypePairSet.hh
	$(CPP) $(CPP_FLAGS) -o $@ $@.cc $(SRCS) $(CPP_LIBS) -I$(CBRC_CPP_HOME)


tryFLSorted2TypePairSet_readFromTextFile : $(SRCS) $(HEADERS) tryFLSorted2TypePairSet_readFromTextFile.cc $(CBRC_CPP_HOME)/utils/FLArray/FLSorted2TypePairSet.hh
	$(CPP) $(CPP_FLAGS) -o $@ $@.cc $(SRCS) $(CPP_LIBS) -I$(CBRC_CPP_HOME)

tryFLSortedIO : $(SRCS) $(HDRS)       \
		$(FLSortedFunctions)  \
		tryFLSortedIO.cc
	$(CPP) $(CPP_FLAGS) -o $@ tryFLSortedIO.cc $(SRCS) \
		$(FLSortedFunctionsCC)                     \
		$(CPP_LIBS) -I$(CBRC_CPP_HOME)

tryConstantLabeledSquareDoubleFLENumMatrix : $(SRCS) $(HDRS) \
	$(ConstantLabeledSquareDoubleFLENumMatrix)           \
	tryConstantLabeledSquareDoubleFLENumMatrix.cc
#
	$(CPP) $(CPP_FLAGS) \
		$(ConstantLabeledSquareDoubleFLENumMatrixCC) \
		-o $@ $@.cc $(SRCS) $(CPP_LIBS) -I$(CBRC_CPP_HOME)


tryConstantLabeledSquareDoubleFLENumMatrix_renumLabels : $(SRCS) $(HDRS) \
		$(ConstantLabeledSquareDoubleFLENumMatrix)           \
		tryConstantLabeledSquareDoubleFLENumMatrix_renumLabels.cc
	$(CPP) $(CPP_FLAGS) \
		$(ConstantLabeledSquareDoubleFLENumMatrixCC) \
		-o $@ $@.cc $(SRCS) $(CPP_LIBS) -I$(CBRC_CPP_HOME)

tryConstantLabeledSquareDoubleFLENumMatrix_renumLabels_SerialNumbersForStrings_accessCounting : $(SRCS) $(HDRS) \
		$(ConstantLabeledSquareDoubleFLENumMatrix)           \
		tryConstantLabeledSquareDoubleFLENumMatrix_renumLabels_SerialNumbersForStrings_accessCounting.cc
	$(CPP) $(CPP_FLAGS) \
		$(ConstantLabeledSquareDoubleFLENumMatrixCC) \
		-o $@ $@.cc $(SRCS) $(CPP_LIBS) -I$(CBRC_CPP_HOME)


tryConstantLabeledSquareDoubleFLENumMatrix_labeledIdentityDoubleMatrix : $(SRCS) $(HDRS) 	\
		$(ConstantLabeledSquareDoubleFLENumMatrix)           				\
		tryConstantLabeledSquareDoubleFLENumMatrix_labeledIdentityDoubleMatrix.cc
	$(CPP) $(CPP_FLAGS) 					\
		$(ConstantLabeledSquareDoubleFLENumMatrixCC) 	\
		-o $@ $@.cc $(SRCS) $(CPP_LIBS) -I$(CBRC_CPP_HOME)

tryFLArrayFunctions_randomElement : $(SRCS) $(HDRS) 	\
		$(FLArrayFunctionsHH) 			\
		tryFLArrayFunctions_randomElement.cc
	$(CPP) $(CPP_FLAGS) -o $@ $@.cc $(SRCS) $(CPP_LIBS) -I$(CBRC_CPP_HOME)


tryFLESorted_readWrite : $(SRCS) $(HDRS) $(FLESortedHH)	\
		tryFLESorted_readWrite.cc
	$(CPP) $(CPP_FLAGS)		\
	-o $@ $@.cc $(SRCS) $(CPP_LIBS) -I$(CBRC_CPP_HOME)


#xxx : $(SRCS) $(HDRS) xxx.cc
#	$(CPP) $(CPP_FLAGS) -o $@ $@.cc $(SRCS) $(CPP_LIBS) -I$(CBRC_CPP_HOME)

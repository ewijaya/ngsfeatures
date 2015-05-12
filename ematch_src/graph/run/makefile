#  Author: Paul B. Horton
#  Organization: Computational Biology Research Center, AIST, Japan
#  Copyright (C) 2008, Paul B. Horton. All rights reserved.
#  Creation Date: 2008.2.24
#  Last Modification: $Date: 2009/04/26 06:43:35 $
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

ConnectedComponentOnlineComputerDIR = $(CBRC_CPP_HOME)/utils/graph
ConnectedComponentOnlineComputerHH  = $(ConnectedComponentOnlineComputerDIR)/ConnectedComponentOnlineComputer.hh
ConnectedComponentOnlineComputerCC  = $(ConnectedComponentOnlineComputerDIR)/ConnectedComponentOnlineComputer.cc
ConnectedComponentOnlineComputer    = $(ConnectedComponentOnlineComputerHH) $(ConnectedComponentOnlineComputerCC)

OneEdgePerLineFormatDIR = $(CBRC_CPP_HOME)/utils/graph
OneEdgePerLineFormatHH  = $(OneEdgePerLineFormatDIR)/OneEdgePerLineFormat.hh
OneEdgePerLineFormatCC  = $(OneEdgePerLineFormatDIR)/OneEdgePerLineFormat.cc
OneEdgePerLineFormat    = $(OneEdgePerLineFormatHH) $(OneEdgePerLineFormatCC)

NeighborListGraphDIR = $(CBRC_CPP_HOME)/utils/graph
NeighborListGraphHH  = $(NeighborListGraphDIR)/NeighborListGraph.hh
NeighborListGraphCC  = $(NeighborListGraphDIR)/NeighborListGraph.cc
NeighborListGraph    = $(NeighborListGraphHH) $(NeighborListGraphCC)

BitArrayDIR = $(CBRC_CPP_HOME)/utils/BitMatrix
BitArrayHH  = $(BitArrayDIR)/BitArray.hh
BitArrayCC  = $(BitArrayDIR)/BitArray.cc
BitArray    = $(BitArrayHH) $(BitArrayCC)



# Files most targets depend on
HDRS = $(ArgvParserHH) $(perlishHH)

SRCS = $(perlishCC)


# ***************** Target List *************************
TARGETS =	listConnectedComponents					\
		printRandomRepresentativeFromEachConnectedComponent	\
		runAllShortestEdgeLengthPaths


all : $(TARGETS)

clean :
	rm -f $(TARGETS) 2>/dev/null; true

list :
	@echo $(TARGETS) | sed 's/\s/\n/g'


# ***************** Production Rules *************************
listConnectedComponents : $(SRCS) $(HDRS)  \
		listConnectedComponents.cc \
		$(OneEdgePerLineFormat)    \
		$(ConnectedComponentOnlineComputer)
	$(CPP) $(CPP_FLAGS) -o $@ $@.cc       \
		$(OneEdgePerLineFormatCC)     \
		$(ConnectedComponentOnlineComputerCC) \
		$(SRCS) $(CPP_LIBS) -I$(CBRC_CPP_HOME)


printRandomRepresentativeFromEachConnectedComponent : $(SRCS) $(HDRS)	\
		printRandomRepresentativeFromEachConnectedComponent.cc	\
		$(OneEdgePerLineFormat)    				\
		$(ConnectedComponentOnlineComputer)
	$(CPP) $(CPP_FLAGS) -o $@ $@.cc       \
		$(OneEdgePerLineFormatCC)     \
		$(ConnectedComponentOnlineComputerCC) \
		$(SRCS) $(CPP_LIBS) -I$(CBRC_CPP_HOME)



runAllShortestEdgeLengthPaths : $(SRCS) $(HDRS)  	\
		$(BitArray)				\
		$(ConnectedComponentOnlineComputer)	\
		$(NeighborListGraph)			\
		$(OneEdgePerLineFormat)			\
		runAllShortestEdgeLengthPaths.cc
	$(CPP) $(CPP_FLAGS) -o $@ $@.cc       		\
		$(BitArrayCC)				\
		$(ConnectedComponentOnlineComputerCC) 	\
		$(NeighborListGraphCC)			\
		$(OneEdgePerLineFormatCC)     		\
		$(SRCS) $(CPP_LIBS) -I$(CBRC_CPP_HOME)


# production rule template
#xxx : $(SRCS) $(HDRS) xxx.cc
#	$(CPP) $(CPP_FLAGS) -o $@ $@.cc \
#	$(SRCS) $(CPP_LIBS) -I$(CBRC_CPP_HOME)


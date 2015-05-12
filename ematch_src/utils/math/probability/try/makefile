#  Author: Paul B. Horton
#  Organization: Computational Biology Research Center, AIST, Japan
#  Copyright (C) 2006, Paul B. Horton. All rights reserved.
#  Creation Date: 2006.9.22
#  Last Modification: $Date: 2006/09/22 08:14:20 $
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

# ***************** Dependencies *************************
ArgvParserDIR = $(CBRC_CPP_HOME)/utils/argvParsing/
ArgvParserHH = $(ArgvParserDIR)/ArgvParser.hh
ArgvParserCC = $(ArgvParserDIR)/ArgvParser.cc

perlishDIR = $(CBRC_CPP_HOME)/utils/perlish/
perlishHH = $(perlishDIR)/perlish.hh
perlishCC = $(perlishDIR)/perlish.cc

FLEArrayDIR = $(CBRC_CPP_HOME)/utils/FLArray
FLEArrayHH = $(FLEArrayDIR)/FLEArray.hh

DiscreteCumulativeProbabilityDistributionDIR = ..
DiscreteCumulativeProbabilityDistributionHH = $(DiscreteCumulativeProbabilityDistributionDIR)/DiscreteCumulativeProbabilityDistribution.hh
DiscreteCumulativeProbabilityDistributionCC = $(DiscreteCumulativeProbabilityDistributionDIR)/DiscreteCumulativeProbabilityDistribution.cc
DiscreteCumulativeProbabilityDistribution = $(DiscreteCumulativeProbabilityDistributionHH) $(DiscreteCumulativeProbabilityDistributionCC)

HDRS = $(ArgvParserHH) $(FLEArrayHH)

SRCS = $(ArgvParserCC)



# ***************** Production Rules *************************
tryDiscreteCumulativeProbabilityDistribution : $(SRCS) $(HDRS) \
	$(DiscreteCumulativeProbabilityDistribution) \
	tryDiscreteCumulativeProbabilityDistribution.cc
#
	$(CPP) $(CPP_FLAGS) -o $@ \
	$(DiscreteCumulativeProbabilityDistributionCC) \
	$@.cc $(SRCS) -I$(CBRC_CPP_HOME)

#xxx : $(SRCS) $(HDRS) xxx.cc
#	$(CPP) $(CPP_FLAGS) -o $@ $@.cc $(SRCS) -I$(CBRC_CPP_HOME)

#  Author: Paul B. Horton
#  Organization: Computational Biology Research Center, Japan
#  Creation Date: : 2005.12.17
#  Last Modification: $Date: 2007/10/17 21:48:14 $
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

GDB_DEBUG = # set this variable to include code which is convenient for use with gdb.

CPP_OPTIMIZE = -O3
CPP_DEBUG = -g
OPTIMIZE = $(CPP_OPTIMIZE) $(CPP_DEBUG) -DCBRC_OPTIMIZE=$(CBRC_OPTIMIZE)
WARN = -Wall
CPP = g++

ArgvParserDIR = $(CBRC_CPP_HOME)/utils/argvParsing
ArgvParserHH  = $(ArgvParserDIR)/ArgvParser.hh



# ***************** Dependencies *************************
TextWindowHH = $(CBRC_CPP_HOME)/utils/textWindow/TextWindow.hh

HDRS = $(ArgvParserHH) $(CBRC_CPP_HOME)/utils/FLArray/FLArray.hh
SRCS = 

# ***************** Production Rules *************************
tryTextWindow : $(HDRS) $(TextWindowHH) tryTextWindow.cc
	$(CPP) $(CPP_DEBUG) $(WARN) $(OPTIMIZE) -o $@ $@.cc -I$(CBRC_CPP_HOME)

#  Author: Paul B. Horton
#
#  Organization: Computational Biology Research Center, Japan
#  
#  Creation Date: 2005.8.30
#
#  Last Modification: $Date: 2009/04/20 21:38:45 $
#
#  Copyright: All rights reserved
#  
.SUFFIXES: # suppress build in suffixes

# set environment variable $CBRC_CPP_HOME to the top of the source tree.
SRCS = $(CBRC_CPP_HOME)/utils/perlish/perlish.cc \
	../ConstantLabeledSquareDoubleFLENumMatrix.cc

HDRS = $(CBRC_CPP_HOME)/utils/perlish/perlish.hh \
	../ConstantLabeledSquareDoubleFLENumMatrix.hh

CPP_OPTIMIZE = -O3
CPP_DEBUG = -g
CPP_WARN = -Wall
CPP_FLAGS = $(CPP_WARN) $(CPP_DEBUG) $(CPP_OPTIMIZE) -DCBRC_OPTIMIZE=$(CBRC_OPTIMIZE)
CPP_LIBS = -lboost_regex
CPP = g++


isConstantLabeledSquareDoubleFLENumMatrixSymmetric : $(SRCS) $(HDRS)	\
		isConstantLabeledSquareDoubleFLENumMatrixSymmetric.cc
	$(CPP) $(CPP_FLAGS) -o $@ $@.cc	\
		 $(SRCS) $(CPP_LIBS) -I$(CBRC_CPP_HOME)


SRCS = $(CBRC_CPP_HOME)/utils/perlish/perlish.cc

HEADERS = $(CBRC_CPP_HOME)/utils/FLArray/FLArray.hh $(CBRC_CPP_HOME)/utils/perlish/perlish.hh $(CBRC_CPP_HOME)/utils/serialNumbers/SerialNumbersForStrings.hh

CPP_OPT = -O3
DEBUG = -g
WARN = -Wall


testFLSorted2TypePairSet : $(SRCS) $(HEADERS) testFLSorted2TypePairSet.cc $(CBRC_CPP_HOME)/utils/FLArray/FLSorted2TypePairSet.hh
	g++ $(WARN) $(DEBUG) $(CPP_OPT) -o $@ testFLSorted2TypePairSet.cc $(SRCS) -I$(CBRC_CPP_HOME)

#xxx : $(SRCS) $(HEADERS) xxx.cc
#	g++ $(WARN) $(DEBUG) $(CPP_OPT) -o $@ xxx.cc $(SRCS) -I../../..


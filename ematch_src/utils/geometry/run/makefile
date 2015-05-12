#CBRC_CPP_HOME = /home/paulh/cbrcRepos/C++
ArgvParserDIR = $(CBRC_CPP_HOME)/utils/argvParsing/
ArgvParserHH = $(ArgvParserDIR)/ArgvParser.hh
ArgvParserCC = $(ArgvParserDIR)/ArgvParser.cc

perlishDIR = $(CBRC_CPP_HOME)/utils/perlish/
perlishHH = $(perlishDIR)/perlish.hh
perlishCC = $(perlishDIR)/perlish.cc

SRCS = $(ArgvParserCC) $(perlishCC)
HDRS = $(ArgvParserHH) $(perlishHH)

CPP = g++
CPP_OPT = -O3
CPP_DEBUG = -g
CPP_WARN = -Wall
CPP_FLAGS = $(CPP_WARN) $(CPP_DEBUG) $(CPP_OPTIMIZE)


smoothMinSampleSize : ../smoothing.cc ../smoothing.hh smoothMinSampleSize.cc $(SRCS) $(HDRS)
	$(CPP) $(CPP_FLAGS) -o $@ $@.cc ../smoothing.cc -I$(CBRC_CPP_HOME) $(SRCS)

smoothMonotonically : ../smoothing.cc ../smoothing.hh smoothMonotonically.cc $(SRC) $(HDRS)
	$(CPP) $(CPP_FLAGS) -o $@ $@.cc ../smoothing.cc -I$(CBRC_CPP_HOME) $(SRCS)

#xxx : $(SRCS) $(HEADERS) xxx.cc
#	$(CPP) $(CPP_FLAGS) -o $@ $@.cc $(SRCS) -I$(CBRC_CPP_HOME)

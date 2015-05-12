#CBRC_CPP_HOME = /home/paulh/cbrcRepos/C++
SRCS = $(CBRC_CPP_HOME)/

HEADERS = $(CBRC_CPP_HOME)/

CPP = g++
CPP_OPTIMIZE = -O3
CPP_DEBUG = -g
CPP_WARN = -Wall
CPP_FLAGS = $(CPP_WARN) $(CPP_DEBUG) $(CPP_OPTIMIZE) -DCBRC_OPTIMIZE=$(CBRC_OPTIMIZE)

tryMonotonicSmoothing : ../smoothing.cc ../smoothing.hh tryMonotonicSmoothing.cc
	$(CPP) $(CPP_FLAGS) -o $@ $@.cc ../smoothing.cc -I$(CBRC_CPP_HOME)

#xxx : $(SRCS) $(HEADERS) xxx.cc
#	$(CPP) $(WARN) $(CPP_DEBUG) $(CPP_OPT) -o $@ $@.cc $(SRCS) -I$(CBRC_CPP_HOME)

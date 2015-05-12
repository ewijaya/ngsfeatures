#CBRC_CPP_HOME = /home/paulh/cbrcRepos/C++
SRCS = $(CBRC_CPP_HOME)/utils/perlish/perlish.cc
SRC=

HEADERS = $(CBRC_CPP_HOME)/utils/ChainHashtable/ChainHashtable.hh

CPP = g++
CPP_OPTIMIZE = -O3
CPP_DEBUG = -g
CPP_WARN = -Wall
CPP_LIBS = -lboost_regex
# export CPP_ARCHITECTURE_FLAG="-DCBRC_ARCHITECTURE_SOLARIS" for solaris compile.
CPP_ARCHITECTURE_FLAG= 
CPP_FLAGS = $(CPP_WARN) $(CPP_DEBUG) $(CPP_OPTIMIZE) $(CPP_ARCHITECTURE_FLAG)

# ***************** Target List *************************
TARGETS = 	testStringKeyHashtableSpeed	\
		testMapSpeed			\
		testString2KeyHashtable

all : $(TARGETS)

clean :
	rm -f $(TARGETS) 2>/dev/null; true

list :
	echo $(TARGETS) | sed 's/\s/\n/g'



# ***************** Production Rules *************************                  
testStringKeyHashtableSpeed : $(SRCS) $(CBRC_CPP_HOME)/utils/ChainHashtable/StringKeyHashtable.hh testStringKeyHashtableSpeed.cc
	$(CPP) $(CPP_FLAGS) -o $@ $@.cc $(SRCS) $(CPP_LIBS) -I$(CBRC_CPP_HOME)

testMapSpeed : $(SRCS) testMapSpeed.cc
	$(CPP) $(CPP_FLAGS) -o $@ $@.cc	$(SRCS) $(CPP_LIBS) -I$(CBRC_CPP_HOME)

testString2KeyHashtable : $(SRCS) $(CBRC_CPP_HOME)/utils/ChainHashtable/String2KeyHashtable.hh testString2KeyHashtable.cc
	$(CPP) $(CPP_FLAGS) -o $@ $@.cc $(SRCS) $(CPP_LIBS) -I$(CBRC_CPP_HOME)

#xxx : $(SRCS) $(HEADERS) xxx.cc
#	$(CPP) $(CPP_FLAGS) -o $@ $@.cc	\
#		$(SRCS) $(CPP_LIBS) -I$(CBRC_CPP_HOME)

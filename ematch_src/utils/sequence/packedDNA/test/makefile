SRCS = ../../ResidueIndexMap/ResidueIndexMap.cc

HEADERS = ../DNAArrayBit8.hh ../../ResidueIndexMap/ResidueIndexMap.hh

OPT = -O3
DEBUG = -g
WARN = -Wall

testDNAArrayBit8 : $(SRCS) $(HEADERS) testDNAArrayBit8.cc ../DNAArrayBit8.hh
	g++ $(WARN) $(DEBUG) $(OPT) -o $@ testDNAArrayBit8.cc $(SRCS) -I../../../..

testDNAArrayBit16 : $(SRCS) $(HEADERS) testDNAArrayBit16.cc ../DNAArrayBit16.hh
	g++ $(WARN) $(DEBUG) $(OPT) -o $@ testDNAArrayBit16.cc $(SRCS) -I../../../..

testDNAArrayBit16Hamming : $(SRCS) $(HEADERS) testDNAArrayBit16Hamming.cc ../DNAArrayBit16Hamming.hh ../DNAArrayBit16.hh
	g++ $(WARN) $(DEBUG) $(OPT) -o $@ testDNAArrayBit16Hamming.cc $(SRCS) -I../../../..

#xxx : $(SRCS) $(HEADERS) xxx.cc
#	g++ $(WARN) $(DEBUG) $(OPT) -o $@ xxx.cc $(SRCS) -I../..

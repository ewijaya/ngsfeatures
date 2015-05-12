#!/bin/bash
g++ -Wall FindNeighboursWithQualJuxt.cc -o FindNeighboursWithQualJuxt  
g++ -Wall -g -O3 -DCBRC_OPTIMIZE=2 -combine -o runRecountExpectationMatchingTagCorrector \
	./RecountComputerForGraphOnDisk.cc ./RecountExpectationMatchingTagCorrector.cc ./RecountNeighborList.cc ./RecountNeighborProbGraphOnDisk.cc ./RecountTagCounts.cc ./TagSet.cc ./utils/perlish/perlish.cc ./utils/sequence/ResidueIndexMap/ResidueIndexMap.cc ./utils/sequence/packedDNA/sigma4bitPackingUtils.cc runRecountExpectationMatchingTagCorrector.cc	\
	-lboost_regex -I.
g++ -Wall -g -O3 -DCBRC_OPTIMIZE=2 -combine -o writeRecountNeighborProbGraph \
	./RecountNeighborList.cc ./RecountNeighborProbGraphWriter.cc ./TagSet.cc ./utils/perlish/perlish.cc ./utils/sequence/ResidueIndexMap/ResidueIndexMap.cc ./utils/sequence/packedDNA/sigma4bitPackingUtils.cc writeRecountNeighborProbGraph.cc	\
	-lboost_regex -I.

g++ -Wall -g -O3 -DCBRC_DEBUG -DGDB_DEBUG -DCBRC_OPTIMIZE=2 -combine -o dumpRecountNeighborProbGraphOnDisk_ConnectedComponentSize \
./graph/ConnectedComponentOnlineComputer.cc ./RecountNeighborList.cc \
./RecountNeighborProbGraphOnDisk.cc ./TagSet.cc \
./utils/argvParsing/ArgvParser.cc \
./utils/perlish/perlish.cc \
./utils/sequence/ResidueIndexMap/ResidueIndexMap.cc \
./utils/sequence/packedDNA/sigma4bitPackingUtils.cc \
dumpRecountNeighborProbGraphOnDisk_ConnectedComponentSize.cc    \
    -lboost_regex -I.

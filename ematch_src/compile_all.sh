#!/bin/bash
# Optimized build flags for maximum performance
OPTFLAGS="-Wall -O3 -march=native -mtune=native -flto -ffast-math -funroll-loops -finline-functions -std=c++20"

g++ $OPTFLAGS FindNeighboursWithQualJuxt.cc -o FindNeighboursWithQualJuxt

g++ $OPTFLAGS -DCBRC_OPTIMIZE=2 -o runRecountExpectationMatchingTagCorrector \
	./RecountComputerForGraphOnDisk.cc ./RecountExpectationMatchingTagCorrector.cc ./RecountNeighborList.cc ./RecountNeighborProbGraphOnDisk.cc ./RecountTagCounts.cc ./TagSet.cc ./utils/perlish/perlish.cc ./utils/sequence/ResidueIndexMap/ResidueIndexMap.cc ./utils/sequence/packedDNA/sigma4bitPackingUtils.cc runRecountExpectationMatchingTagCorrector.cc	\
	-lboost_regex -I.

g++ $OPTFLAGS -DCBRC_OPTIMIZE=2 -o writeRecountNeighborProbGraph \
	./RecountNeighborList.cc ./RecountNeighborProbGraphWriter.cc ./TagSet.cc ./utils/perlish/perlish.cc ./utils/sequence/ResidueIndexMap/ResidueIndexMap.cc ./utils/sequence/packedDNA/sigma4bitPackingUtils.cc writeRecountNeighborProbGraph.cc	\
	-lboost_regex -I.

g++ $OPTFLAGS -DCBRC_DEBUG -DGDB_DEBUG -DCBRC_OPTIMIZE=2 -o dumpRecountNeighborProbGraphOnDisk_ConnectedComponentSize \
./graph/ConnectedComponentOnlineComputer.cc ./RecountNeighborList.cc \
./RecountNeighborProbGraphOnDisk.cc ./TagSet.cc \
./utils/argvParsing/ArgvParser.cc \
./utils/perlish/perlish.cc \
./utils/sequence/ResidueIndexMap/ResidueIndexMap.cc \
./utils/sequence/packedDNA/sigma4bitPackingUtils.cc \
dumpRecountNeighborProbGraphOnDisk_ConnectedComponentSize.cc    \
    -lboost_regex -I.

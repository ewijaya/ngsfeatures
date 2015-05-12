#!/bin/bash

g++ -Wall -g -O3 -DCBRC_OPTIMIZE=2 -combine -o tryKnapsackEnumeratorMultiProbes ./KnapsackEnumerator.cc \
./KnapsackObjectVector.cc ./utils/argvParsing/ArgvParser.cc \
./utils/perlish/perlish.cc \
tryKnapsackEnumeratorMultiProbes.cc -lboost_regex -I . 








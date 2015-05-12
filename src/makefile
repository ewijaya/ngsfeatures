CXX = g++ -O2 -Wall 

all: GenerateProportion FindNeighboursWithQual \
	AverageTagsQuals_27 AverageTagsQuals_36 PickBaseQual \
    EstimateTrueCount_llratio EstimateTrueCount_EntropyFast \
    EstimateTrueCount_Capacity EstimateTrueCount

FindNeighboursWithQual: FindNeighboursWithQual.cc Utilities.cc
	$(CXX)   $^ -o $@

GenerateProportion: GenerateProportion.cc Utilities.cc
	$(CXX)   $^ -o $@

PickBaseQual: PickBaseQual.cc 
	$(CXX)   $^ -o $@

AverageTagsQuals_27: AverageTagsQuals_27.cc
	$(CXX)   $^ -o $@

AverageTagsQuals_36: AverageTagsQuals_36.cc
	$(CXX)   $^ -o $@

EstimateTrueCount: EstimateTrueCount.cc Utilities.cc
	$(CXX)   $^ -o $@

EstimateTrueCount_llratio: EstimateTrueCount_llratio.cc Utilities.cc
	$(CXX)   $^ -o $@

EstimateTrueCount_Capacity: EstimateTrueCount_Capacity.cc Utilities.cc
	$(CXX)   $^ -o $@

EstimateTrueCount_EntropyFast: EstimateTrueCount_EntropyFast.cc Utilities.cc
	$(CXX)   $^ -o $@


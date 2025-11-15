# Optimized compiler flags for maximum performance
CXX = g++ -O3 -march=native -mtune=native -Wall -flto -ffast-math -funroll-loops -finline-functions -std=c++20
LDFLAGS = -flto 

all: GenerateProportion FindNeighboursWithQual \
	AverageTagsQuals_27 AverageTagsQuals_36 PickBaseQual \
    EstimateTrueCount_llratio EstimateTrueCount_EntropyFast \
    EstimateTrueCount_Capacity EstimateTrueCount

FindNeighboursWithQual: FindNeighboursWithQual.cc Utilities.cc
	$(CXX) $^ -o $@ $(LDFLAGS)

GenerateProportion: GenerateProportion.cc Utilities.cc
	$(CXX) $^ -o $@ $(LDFLAGS)

PickBaseQual: PickBaseQual.cc 
	$(CXX) $^ -o $@ $(LDFLAGS)

AverageTagsQuals_27: AverageTagsQuals_27.cc
	$(CXX) $^ -o $@ $(LDFLAGS)

AverageTagsQuals_36: AverageTagsQuals_36.cc
	$(CXX) $^ -o $@ $(LDFLAGS)

EstimateTrueCount: EstimateTrueCount.cc Utilities.cc
	$(CXX) $^ -o $@ $(LDFLAGS)

EstimateTrueCount_llratio: EstimateTrueCount_llratio.cc Utilities.cc
	$(CXX) $^ -o $@ $(LDFLAGS)

EstimateTrueCount_Capacity: EstimateTrueCount_Capacity.cc Utilities.cc
	$(CXX) $^ -o $@ $(LDFLAGS)

EstimateTrueCount_EntropyFast: EstimateTrueCount_EntropyFast.cc Utilities.cc
	$(CXX) $^ -o $@ $(LDFLAGS)


# CMakeLists.txt for main NGSFeatures binaries

# ============================================================================
# Shared utilities library
# ============================================================================

add_library(ngsfeatures_utilities OBJECT
    Utilities.cc
)

target_include_directories(ngsfeatures_utilities PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# ============================================================================
# Main binaries
# ============================================================================

# FindNeighboursWithQual - Finds neighbors within Hamming distance
add_executable(FindNeighboursWithQual
    FindNeighboursWithQual.cc
    $<TARGET_OBJECTS:ngsfeatures_utilities>
)

# GenerateProportion - Calculates sequence proportions
add_executable(GenerateProportion
    GenerateProportion.cc
    $<TARGET_OBJECTS:ngsfeatures_utilities>
)

# PickBaseQual - Base quality extraction utility
add_executable(PickBaseQual
    PickBaseQual.cc
)

# AverageTagsQuals_27 - Quality score averaging for 27bp reads
add_executable(AverageTagsQuals_27
    AverageTagsQuals_27.cc
)

# AverageTagsQuals_36 - Quality score averaging for 36bp reads
add_executable(AverageTagsQuals_36
    AverageTagsQuals_36.cc
)

# EstimateTrueCount - Base EM algorithm
add_executable(EstimateTrueCount
    EstimateTrueCount.cc
    $<TARGET_OBJECTS:ngsfeatures_utilities>
)

# EstimateTrueCount_llratio - Log-likelihood ratio variant
add_executable(EstimateTrueCount_llratio
    EstimateTrueCount_llratio.cc
    $<TARGET_OBJECTS:ngsfeatures_utilities>
)

# EstimateTrueCount_Capacity - Capacity-constrained variant
add_executable(EstimateTrueCount_Capacity
    EstimateTrueCount_Capacity.cc
    $<TARGET_OBJECTS:ngsfeatures_utilities>
)

# EstimateTrueCount_EntropyFast - Entropy-based variant
add_executable(EstimateTrueCount_EntropyFast
    EstimateTrueCount_EntropyFast.cc
    $<TARGET_OBJECTS:ngsfeatures_utilities>
)

# ============================================================================
# Installation
# ============================================================================

install(TARGETS
    FindNeighboursWithQual
    GenerateProportion
    PickBaseQual
    AverageTagsQuals_27
    AverageTagsQuals_36
    EstimateTrueCount
    EstimateTrueCount_llratio
    EstimateTrueCount_Capacity
    EstimateTrueCount_EntropyFast
    DESTINATION bin
)

# Install Python scripts
install(PROGRAMS
    scc.py
    lr.py
    convert_first_col2atcg.py
    DESTINATION bin
)

# Install shell scripts
install(PROGRAMS
    summarize.sh
    DESTINATION bin
)

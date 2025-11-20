# CMakeLists.txt for Knapsack module

# ============================================================================
# Compiler definitions
# ============================================================================

add_compile_definitions(CBRC_OPTIMIZE=2)

# ============================================================================
# Utility libraries
# ============================================================================

# Knapsack utilities (reuse from ematch if available)
add_library(knapsack_utils OBJECT
    utils/argvParsing/ArgvParser.cc
    utils/perlish/perlish.cc
)

target_include_directories(knapsack_utils PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# ============================================================================
# Knapsack core library
# ============================================================================

add_library(knapsack_core OBJECT
    KnapsackEnumerator.cc
    KnapsackObjectVector.cc
)

target_include_directories(knapsack_core PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# ============================================================================
# Executables
# ============================================================================

# tryKnapsackEnumeratorMultiProbes - Main knapsack algorithm
add_executable(tryKnapsackEnumeratorMultiProbes
    tryKnapsackEnumeratorMultiProbes.cc
    $<TARGET_OBJECTS:knapsack_core>
    $<TARGET_OBJECTS:knapsack_utils>
)

target_include_directories(tryKnapsackEnumeratorMultiProbes PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(tryKnapsackEnumeratorMultiProbes PRIVATE
    Boost::regex
)

# ============================================================================
# Installation
# ============================================================================

install(TARGETS
    tryKnapsackEnumeratorMultiProbes
    DESTINATION bin
)

# CMakeLists.txt for Expectation-Matching module

# ============================================================================
# Compiler definitions
# ============================================================================

add_compile_definitions(CBRC_OPTIMIZE=2)

# ============================================================================
# Utility libraries
# ============================================================================

# Perlish utilities
add_library(perlish OBJECT
    utils/perlish/perlish.cc
)

target_include_directories(perlish PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Sequence utilities
add_library(sequence_utils OBJECT
    utils/sequence/ResidueIndexMap/ResidueIndexMap.cc
    utils/sequence/packedDNA/sigma4bitPackingUtils.cc
)

target_include_directories(sequence_utils PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# ArgvParser utilities
add_library(argv_parser OBJECT
    utils/argvParsing/ArgvParser.cc
)

target_include_directories(argv_parser PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Graph utilities
add_library(graph_utils OBJECT
    graph/ConnectedComponentOnlineComputer.cc
    graph/MatrixGraph.cc
    graph/OneEdgePerLineFormat.cc
    graph/NeighborListGraph.cc
)

target_include_directories(graph_utils PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# ============================================================================
# Core Expectation-Matching libraries
# ============================================================================

# Recount components
add_library(recount_core OBJECT
    RecountComputerForGraphOnDisk.cc
    RecountExpectationMatchingTagCorrector.cc
    RecountNeighborList.cc
    RecountNeighborProbGraphOnDisk.cc
    RecountTagCounts.cc
    TagSet.cc
)

target_include_directories(recount_core PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(recount_core PUBLIC
    Boost::regex
)

# Recount writer components
add_library(recount_writer OBJECT
    RecountNeighborList.cc
    RecountNeighborProbGraphWriter.cc
    TagSet.cc
)

target_include_directories(recount_writer PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(recount_writer PUBLIC
    Boost::regex
)

# ============================================================================
# Executables
# ============================================================================

# FindNeighboursWithQualJuxt
add_executable(FindNeighboursWithQualJuxt
    FindNeighboursWithQualJuxt.cc
)

# runRecountExpectationMatchingTagCorrector - Main EM algorithm
add_executable(runRecountExpectationMatchingTagCorrector
    runRecountExpectationMatchingTagCorrector.cc
    $<TARGET_OBJECTS:recount_core>
    $<TARGET_OBJECTS:perlish>
    $<TARGET_OBJECTS:sequence_utils>
)

target_include_directories(runRecountExpectationMatchingTagCorrector PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(runRecountExpectationMatchingTagCorrector PRIVATE
    Boost::regex
)

# writeRecountNeighborProbGraph
add_executable(writeRecountNeighborProbGraph
    writeRecountNeighborProbGraph.cc
    $<TARGET_OBJECTS:recount_writer>
    $<TARGET_OBJECTS:perlish>
    $<TARGET_OBJECTS:sequence_utils>
)

target_include_directories(writeRecountNeighborProbGraph PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(writeRecountNeighborProbGraph PRIVATE
    Boost::regex
)

# dumpRecountNeighborProbGraphOnDisk_ConnectedComponentSize
add_executable(dumpRecountNeighborProbGraphOnDisk_ConnectedComponentSize
    dumpRecountNeighborProbGraphOnDisk_ConnectedComponentSize.cc
    $<TARGET_OBJECTS:graph_utils>
    RecountNeighborList.cc
    RecountNeighborProbGraphOnDisk.cc
    TagSet.cc
    $<TARGET_OBJECTS:argv_parser>
    $<TARGET_OBJECTS:perlish>
    $<TARGET_OBJECTS:sequence_utils>
)

target_compile_definitions(dumpRecountNeighborProbGraphOnDisk_ConnectedComponentSize PRIVATE
    CBRC_DEBUG
    GDB_DEBUG
)

target_include_directories(dumpRecountNeighborProbGraphOnDisk_ConnectedComponentSize PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(dumpRecountNeighborProbGraphOnDisk_ConnectedComponentSize PRIVATE
    Boost::regex
)

# ============================================================================
# Python scripts
# ============================================================================

# Install Python helper scripts
install(PROGRAMS
    run_Expmatch.py
    GenerateTagListFileWithBaseCount.py
    DESTINATION bin
)

# ============================================================================
# Installation
# ============================================================================

install(TARGETS
    FindNeighboursWithQualJuxt
    runRecountExpectationMatchingTagCorrector
    writeRecountNeighborProbGraph
    dumpRecountNeighborProbGraphOnDisk_ConnectedComponentSize
    DESTINATION bin
)

# ============================================================================
# Tests (if BUILD_TESTS is ON)
# ============================================================================

if(BUILD_TESTS)
    # Graph tests
    if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/graph/test)
        add_subdirectory(graph/test EXCLUDE_FROM_ALL)
    endif()
endif()

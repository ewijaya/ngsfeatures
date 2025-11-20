# Next Session TODO List

**Last Updated:** 2025-11-20
**Current Status:** Phase 2 - Streams A & B Complete

## Completed Work

### ✅ Phase 1: Infrastructure (Complete)
- CMake build system modernization
- CI/CD with GitHub Actions
- Pre-commit hooks
- Docker containerization
- Code formatting standards

### ✅ Phase 2 - Stream A: Code Refactoring (Complete)
- Cleaned all 13 remaining .cc files in src/, ematch_src/, knapsack_src/
- Organized includes alphabetically
- Removed duplicates and unused headers
- 25 files by Paul Horton were already well-organized

### ✅ Phase 2 - Stream B: Performance Optimization (Complete)
- Investigated OpenMP parallelization for EM algorithm
- **Result:** No performance benefit - keep serial implementation
- See `documentations/PERFORMANCE_OPTIMIZATION_REPORT.md` for details

## Remaining Work

### 🔄 Phase 2 - Stream C: Testing Expansion

**Priority:** HIGH
**Estimated Effort:** 2-3 sessions
**Goal:** Achieve 70%+ code coverage for core algorithms

#### Tasks:

1. **Unit Tests for EM Algorithm** (`src/EstimateTrueCount*.cc`)
   - [ ] Test convergence behavior
   - [ ] Test with various dataset sizes (100, 1K, 10K sequences)
   - [ ] Test edge cases (empty input, single sequence, all identical)
   - [ ] Test numerical stability (very small/large values)
   - [ ] Verify log-likelihood computation accuracy
   - [ ] Test early convergence detection

2. **Unit Tests for Utilities** (`src/Utilities.cc`)
   - [ ] Test `GetBaseNameFromFilename()` with various paths
   - [ ] Test `GetPathNameFromFilename()` with various paths
   - [ ] Test edge cases (empty string, no extension, relative paths)
   - [ ] Test with special characters in filenames

3. **Unit Tests for Quality Score Handling**
   - [ ] Test quality score conversions (Phred scale)
   - [ ] Test average quality calculations
   - [ ] Test quality filtering thresholds
   - [ ] Files: `src/AverageTagsQuals_27.cc`, `src/AverageTagsQuals_36.cc`

4. **Unit Tests for Neighbor Finding**
   - [ ] Test neighbor identification algorithms
   - [ ] Test with quality scores
   - [ ] Test hamming distance calculations
   - [ ] Files: `src/FindNeighboursWithQual.cc`, `ematch_src/FindNeighboursWithQualJuxt.cc`

5. **Unit Tests for Sparse Matrix Operations**
   - [ ] Test sparse matrix-vector products
   - [ ] Test with various sparsity patterns
   - [ ] Test numerical accuracy
   - [ ] Functions in `src/EstimateTrueCount.cc`

6. **Integration Tests**
   - [ ] End-to-end pipeline test with small dataset
   - [ ] Test file I/O (.nb, .prop, .nbq files)
   - [ ] Test error handling for malformed input
   - [ ] Test output format validation

7. **Test Coverage Analysis**
   - [ ] Generate coverage report with lcov
   - [ ] Identify untested code paths
   - [ ] Add tests to reach 70% coverage target

**Files to Create:**
- `tests/unit/test_em_algorithm.cc`
- `tests/unit/test_quality_scores.cc`
- `tests/unit/test_neighbors.cc`
- `tests/unit/test_sparse_operations.cc`
- `tests/integration/test_pipeline.cc`

**Reference:**
- Existing test structure: `tests/unit/test_utilities.cc`
- Test documentation: `tests/README.md`

---

### 🔄 Phase 2 - Stream D: Documentation Enhancement

**Priority:** MEDIUM
**Estimated Effort:** 1-2 sessions
**Goal:** Comprehensive documentation for users and developers

#### Tasks:

1. **API Documentation**
   - [ ] Document all public functions in header files
   - [ ] Add Doxygen-style comments
   - [ ] Document function parameters, return values, exceptions
   - [ ] Add usage examples in comments
   - [ ] Files: `src/Utilities.hh`, and other .hh files

2. **Algorithm Documentation**
   - [ ] Document EM algorithm mathematical formulation
   - [ ] Explain convergence criteria
   - [ ] Document neighbor finding algorithm
   - [ ] Explain quality score handling
   - [ ] Add references to papers/sources

3. **User Guide Enhancements**
   - [ ] Add tutorial with step-by-step example
   - [ ] Document all command-line options
   - [ ] Add troubleshooting section
   - [ ] Document expected input/output formats
   - [ ] Add FAQ section

4. **Developer Documentation**
   - [ ] Architecture overview diagram
   - [ ] Code organization explanation
   - [ ] How to add new features
   - [ ] How to run benchmarks
   - [ ] How to profile code

5. **Example Data and Workflows**
   - [ ] Create additional example datasets
   - [ ] Document typical workflows
   - [ ] Add example scripts for common tasks
   - [ ] Create visualization examples (if applicable)

**Files to Create/Update:**
- `documentations/API_REFERENCE.md`
- `documentations/ALGORITHM_DETAILS.md`
- `documentations/USER_GUIDE.md`
- `documentations/DEVELOPER_GUIDE.md`
- `documentations/EXAMPLES.md`

---

### 🔄 Phase 2 - Stream E: Advanced Features

**Priority:** LOW (Optional)
**Estimated Effort:** 3-4 sessions
**Goal:** Add advanced capabilities and improvements

#### Tasks:

1. **Input/Output Enhancements**
   - [ ] Add support for compressed input files (.gz, .bz2)
   - [ ] Add support for BAM/SAM format (optional)
   - [ ] Add progress bar for long-running operations
   - [ ] Add verbose logging mode
   - [ ] Add JSON output format option

2. **Algorithm Improvements**
   - [ ] Implement adaptive convergence criteria
   - [ ] Add multiple convergence metrics
   - [ ] Implement batch processing for multiple samples
   - [ ] Add checkpointing for long runs
   - [ ] Add resume capability

3. **Usability Enhancements**
   - [ ] Add parameter validation with helpful error messages
   - [ ] Add dry-run mode to validate inputs
   - [ ] Add output summary statistics
   - [ ] Add configuration file support (YAML/JSON)

4. **Performance Features** (if needed in future)
   - [ ] Integrate with Eigen library for sparse operations
   - [ ] Add BLAS support for dense operations
   - [ ] Implement cache-friendly data structures
   - [ ] Profile-guided optimization (PGO)

5. **Quality of Life**
   - [ ] Add unit conversion utilities
   - [ ] Add data validation tools
   - [ ] Add format conversion utilities
   - [ ] Add sample data generator for testing

**Files to Create/Modify:**
- Various source files depending on features
- Update CMakeLists.txt for new dependencies
- Update documentation

---

## Recommended Next Steps

### Session Priority Order:

1. **Start with Stream C: Testing Expansion**
   - High impact on code quality
   - Catches bugs early
   - Provides confidence for future changes
   - Begin with: `tests/unit/test_em_algorithm.cc`

2. **Then Stream D: Documentation Enhancement**
   - Makes code maintainable
   - Helps onboard new contributors
   - Low risk, high value
   - Begin with: API documentation in headers

3. **Finally Stream E: Advanced Features (Optional)**
   - Only if time permits
   - Based on user feedback/needs
   - Begin with: Input/output enhancements

### Suggested Session Plan:

**Next Session (Session 1):**
- Set up test infrastructure (if needed)
- Write unit tests for EM algorithm (test_em_algorithm.cc)
- Write unit tests for Utilities (enhance test_utilities.cc)
- Target: 40% code coverage

**Session 2:**
- Write unit tests for quality score handling
- Write unit tests for neighbor finding
- Write basic integration test
- Target: 60% code coverage

**Session 3:**
- Complete remaining unit tests
- Add edge case tests
- Generate and review coverage report
- Target: 70%+ code coverage

**Session 4:**
- Add API documentation (Doxygen comments)
- Create algorithm documentation
- Start user guide enhancements

**Session 5+:**
- Complete documentation
- Optional: Advanced features based on needs

---

## Quick Start for Next Session

To continue where we left off:

```bash
# 1. Check current branch and status
git status
git log --oneline -5

# 2. If starting new work, create a new feature branch
git checkout master
git pull origin master
git checkout -b claude/testing-expansion-[SESSION_ID]

# 3. Build and verify tests work
cmake -B build -DBUILD_TESTS=ON
cmake --build build
ctest --test-dir build --output-on-failure

# 4. Create new test file
vim tests/unit/test_em_algorithm.cc

# 5. Follow test structure from tests/unit/test_utilities.cc
```

## Key Files Reference

**Test Files:**
- `tests/unit/test_utilities.cc` - Example test structure
- `tests/unit/CMakeLists.txt` - How to add new tests
- `tests/README.md` - Testing guide

**Main Algorithm Files:**
- `src/EstimateTrueCount.cc` - Main EM algorithm (537-599 lines)
- `src/Utilities.cc` - Utility functions
- `src/AverageTagsQuals_*.cc` - Quality score handling

**Build Files:**
- `CMakeLists.txt` - Root build configuration
- `tests/CMakeLists.txt` - Test configuration

**Documentation Files:**
- `documentations/` - All documentation
- `README.md` - Main project README

---

## Notes

- All Phase 2 Stream A and B work has been merged to master
- Performance optimization showed no benefits from parallelization
- Original serial implementation is optimal for typical workloads
- Focus on testing and documentation for maximum impact

**Questions or Issues?**
- Check `documentations/REFACTORING_SUMMARY.md` for history
- Check `documentations/PERFORMANCE_OPTIMIZATION_REPORT.md` for performance details
- Check `documentations/PHASE2_EXECUTION_PLAN.md` for overall plan

---

**Ready to start Stream C: Testing Expansion!** 🚀

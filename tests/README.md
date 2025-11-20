# NGSFeatures Test Suite

Comprehensive test suite using Google Test framework.

## Structure

```
tests/
├── CMakeLists.txt          # Main test configuration
├── unit/                   # Unit tests
│   ├── CMakeLists.txt
│   └── test_utilities.cc   # Utility function tests
└── integration/            # Integration tests (planned)
    └── ...
```

## Building and Running Tests

### Quick Start

```bash
# Configure with tests enabled (default)
cmake -B build -DBUILD_TESTS=ON

# Build all tests
cmake --build build

# Run all tests
ctest --test-dir build --output-on-failure

# Or use the custom target
cmake --build build --target run_all_tests
```

### Running Specific Tests

```bash
# Run a specific test executable
./build/tests/unit/test_utilities

# Run tests matching a pattern
ctest --test-dir build -R utilities

# Run with verbose output
ctest --test-dir build --verbose

# Run tests in parallel
ctest --test-dir build -j$(nproc)
```

### Test Coverage

```bash
# Build with coverage flags
cmake -B build -DCMAKE_BUILD_TYPE=Debug \
    -DCMAKE_CXX_FLAGS="--coverage -fprofile-arcs -ftest-coverage"

# Run tests
ctest --test-dir build

# Generate coverage report with lcov
lcov --capture --directory build --output-file coverage.info
lcov --remove coverage.info '/usr/*' '*/tests/*' --output-file coverage.info
genhtml coverage.info --output-directory coverage_html

# View coverage
xdg-open coverage_html/index.html
```

## Test Categories

### Unit Tests (`tests/unit/`)

Test individual functions and classes in isolation.

**Current tests:**
- `test_utilities.cc` - DNA conversion, vector ops, file path handling

**Planned tests:**
- `test_em_algorithm.cc` - EM algorithm correctness
- `test_neighbors.cc` - Neighbor finding algorithms
- `test_scc.cc` - SCC calculation
- `test_quality_scores.cc` - Quality score conversions

### Integration Tests (`tests/integration/`)

Test complete workflows and module interactions.

**Planned tests:**
- End-to-end pipeline tests
- Multi-file processing
- Output validation

## Writing Tests

### Basic Test Structure

```cpp
#include <gtest/gtest.h>

// Test fixture
class MyTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Setup code
    }

    void TearDown() override {
        // Cleanup code
    }
};

// Simple test
TEST_F(MyTest, TestName) {
    EXPECT_EQ(1 + 1, 2);
    ASSERT_TRUE(condition);
}

// Parameterized test
class ParamTest : public ::testing::TestWithParam<int> {};

TEST_P(ParamTest, TestName) {
    int value = GetParam();
    EXPECT_GT(value, 0);
}

INSTANTIATE_TEST_SUITE_P(Values, ParamTest,
    ::testing::Values(1, 2, 3, 4, 5));
```

### Assertions

**Non-fatal assertions (test continues):**
- `EXPECT_EQ(a, b)` - Equality
- `EXPECT_NE(a, b)` - Inequality
- `EXPECT_LT(a, b)` - Less than
- `EXPECT_LE(a, b)` - Less than or equal
- `EXPECT_GT(a, b)` - Greater than
- `EXPECT_GE(a, b)` - Greater than or equal
- `EXPECT_TRUE(condition)`
- `EXPECT_FALSE(condition)`
- `EXPECT_NEAR(a, b, epsilon)` - Floating point comparison

**Fatal assertions (test stops):**
- `ASSERT_*` - Same as `EXPECT_*` but stops on failure

## Test Organization Guidelines

1. **One test file per source file** - `src/Foo.cc` → `tests/unit/test_foo.cc`
2. **Use descriptive test names** - `TEST_F(DNATest, ConvertBaseToIntReturnsCorrectValue)`
3. **Test one thing per test** - Each test should verify a single behavior
4. **Use fixtures for setup** - Avoid code duplication in tests
5. **Test edge cases** - Empty inputs, null pointers, boundary values
6. **Test error handling** - Verify exceptions are thrown when expected

## Coverage Goals

- **Unit tests:** 70%+ code coverage for core algorithms
- **Integration tests:** All major workflows covered
- **Regression tests:** All fixed bugs have tests

## CI Integration

Tests run automatically on every push via GitHub Actions:
- All unit tests must pass
- Coverage reports uploaded to Codecov
- Performance regression tests (planned)

## Adding New Tests

1. Create test file in appropriate directory
2. Add executable to `CMakeLists.txt`:
   ```cmake
   add_executable(test_name test_name.cc)
   target_link_libraries(test_name PRIVATE GTest::gtest_main)
   gtest_discover_tests(test_name)
   ```
3. Write tests using Google Test
4. Run locally to verify
5. Submit PR with tests

## Resources

- [Google Test Primer](https://google.github.io/googletest/primer.html)
- [Google Test Documentation](https://google.github.io/googletest/)
- [CMake and Google Test](https://cmake.org/cmake/help/latest/module/GoogleTest.html)
- [Test Coverage with lcov](https://github.com/linux-test-project/lcov)

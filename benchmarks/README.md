# Performance Benchmarks

This directory contains performance benchmarks for NGSFeatures core algorithms.

## Building Benchmarks

```bash
# From project root
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --target benchmark_em
```

## Running Benchmarks

### Quick Run

```bash
# Run all benchmarks
cmake --build build --target run_benchmarks

# Or directly
./build/benchmarks/benchmark_em
```

### Profiling with perf

```bash
# Record performance data
cmake --build build --target profile_benchmarks

# View report
perf report

# Generate flamegraph (requires flamegraph.pl)
cmake --build build --target flamegraph
xdg-open build/benchmarks/flamegraph.svg
```

### Profiling with gprof

```bash
# Build with profiling enabled
cmake -B build -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS="-pg"
cmake --build build --target benchmark_em

# Run benchmark
./build/benchmarks/benchmark_em

# View profile
gprof ./build/benchmarks/benchmark_em gmon.out > analysis.txt
less analysis.txt
```

## Benchmark Types

### EM Algorithm Benchmark (`benchmark_em_algorithm.cc`)

Tests the performance of the Expectation-Maximization algorithm with:
- Small dataset: 100 sequences, 50 iterations
- Medium dataset: 1,000 sequences, 50 iterations
- Large dataset: 10,000 sequences, 50 iterations
- Very large dataset: 100,000 sequences, 50 iterations

**Metrics:**
- Total execution time (milliseconds)
- Throughput (iterations per second)
- Per-iteration time (milliseconds)

## Baseline Performance

Record baseline metrics here after running benchmarks:

### Current System
- CPU: _[to be filled]_
- RAM: _[to be filled]_
- Compiler: _[to be filled]_

### Baseline Results (unoptimized)
```
Small dataset:     [X] ms, [Y] iter/sec
Medium dataset:    [X] ms, [Y] iter/sec
Large dataset:     [X] ms, [Y] iter/sec
Very large dataset: [X] ms, [Y] iter/sec
```

### After OpenMP Optimization
```
[To be filled after OpenMP implementation]
```

### After SIMD Optimization
```
[To be filled after SIMD implementation]
```

### After PGO Optimization
```
[To be filled after PGO implementation]
```

## Adding New Benchmarks

1. Create `benchmark_<name>.cc` in this directory
2. Add executable to `CMakeLists.txt`:
   ```cmake
   add_executable(benchmark_<name> benchmark_<name>.cc)
   target_compile_options(benchmark_<name> PRIVATE -O3 -march=native)
   ```
3. Add to `run_benchmarks` target
4. Document expected metrics in this README

## Performance Optimization Checklist

- [ ] Baseline measurements recorded
- [ ] Profiling data collected (perf/gprof)
- [ ] Hotspots identified
- [ ] OpenMP parallelization implemented
- [ ] SIMD vectorization implemented
- [ ] Profile-Guided Optimization applied
- [ ] Memory optimizations applied
- [ ] Final measurements recorded
- [ ] Performance gains documented

## Resources

- [perf Examples](http://www.brendangregg.com/perf.html)
- [Flame Graphs](https://github.com/brendangregg/FlameGraph)
- [gprof Tutorial](https://sourceware.org/binutils/docs/gprof/)
- [OpenMP Guide](https://www.openmp.org/resources/tutorials-articles/)
- [Intel Intrinsics Guide](https://www.intel.com/content/www/us/en/docs/intrinsics-guide/)

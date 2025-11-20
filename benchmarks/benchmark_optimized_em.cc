// =====================================================================================
// Benchmark for Optimized EM Algorithm Performance
// Compares original vs OpenMP-optimized implementations
// Copyright 2025, NGSFeatures Project
// =====================================================================================

#include <algorithm>
#include <chrono>
#include <iostream>
#include <numeric>
#include <random>
#include <vector>

#include <cmath>

#ifdef _OPENMP
#include <omp.h>
#endif

// ============================================================================
// ORIGINAL IMPLEMENTATIONS (non-optimized)
// ============================================================================

double computeLogLik_original(const std::vector<double>& m, const std::vector<double>& p,
                              const double& lmbd) {
    double Result = 0.0;
    for (unsigned i = 0; i < p.size(); i++) {
        Result += m[i] * log(p[i] * lmbd);
    }
    return (-lmbd + Result);
}

std::vector<double> divideVecWithScalar_original(const std::vector<double>& Vec,
                                                 const double& val) {
    std::vector<double> Result;
    Result.reserve(Vec.size());
    const double inverse = 1.0 / val;
    for (unsigned i = 0; i < Vec.size(); i++) {
        Result.push_back(Vec[i] * inverse);
    }
    return Result;
}

std::vector<double> divideVecWithVecCorsp_original(const std::vector<double>& Vec1,
                                                   const std::vector<double>& Vec2) {
    std::vector<double> Result;
    Result.reserve(Vec1.size());
    for (unsigned i = 0; i < Vec1.size(); i++) {
        Result.push_back(Vec1[i] / Vec2[i]);
    }
    return Result;
}

std::vector<double> multiplyVecWithVecCorsp_original(const std::vector<double>& Vec1,
                                                     const std::vector<double>& Vec2) {
    std::vector<double> Result;
    Result.reserve(Vec1.size());
    for (unsigned i = 0; i < Vec1.size(); i++) {
        Result.push_back(Vec1[i] * Vec2[i]);
    }
    return Result;
}

std::vector<double> sparseM_vec_prod_original(const std::vector<double>& p,
                                              const std::vector<int>& rowId,
                                              const std::vector<int>& colId,
                                              const std::vector<double>& realVal) {
    std::vector<double> Result(p.size(), 0.0);
    for (unsigned i = 0; i < rowId.size(); i++) {
        Result[rowId[i]] += realVal[i] * p[colId[i]];
    }
    return Result;
}

// ============================================================================
// OPTIMIZED IMPLEMENTATIONS (with OpenMP)
// ============================================================================

double computeLogLik_optimized(const std::vector<double>& m, const std::vector<double>& p,
                               const double& lmbd) {
    double Result = 0.0;
#pragma omp parallel for reduction(+ : Result)
    for (unsigned i = 0; i < p.size(); i++) {
        Result += m[i] * log(p[i] * lmbd);
    }
    return (-lmbd + Result);
}

std::vector<double> divideVecWithScalar_optimized(const std::vector<double>& Vec,
                                                  const double& val) {
    std::vector<double> Result(Vec.size());
    const double inverse = 1.0 / val;
#pragma omp parallel for
    for (unsigned i = 0; i < Vec.size(); i++) {
        Result[i] = Vec[i] * inverse;
    }
    return Result;
}

std::vector<double> divideVecWithVecCorsp_optimized(const std::vector<double>& Vec1,
                                                    const std::vector<double>& Vec2) {
    std::vector<double> Result(Vec1.size());
#pragma omp parallel for
    for (unsigned i = 0; i < Vec1.size(); i++) {
        Result[i] = Vec1[i] / Vec2[i];
    }
    return Result;
}

std::vector<double> multiplyVecWithVecCorsp_optimized(const std::vector<double>& Vec1,
                                                      const std::vector<double>& Vec2) {
    std::vector<double> Result(Vec1.size());
#pragma omp parallel for
    for (unsigned i = 0; i < Vec1.size(); i++) {
        Result[i] = Vec1[i] * Vec2[i];
    }
    return Result;
}

std::vector<double> sparseM_vec_prod_optimized(const std::vector<double>& p,
                                               const std::vector<int>& rowId,
                                               const std::vector<int>& colId,
                                               const std::vector<double>& realVal) {
    std::vector<double> Result(p.size(), 0.0);
#pragma omp parallel for
    for (unsigned i = 0; i < rowId.size(); i++) {
        double contribution = realVal[i] * p[colId[i]];
#pragma omp atomic
        Result[rowId[i]] += contribution;
    }
    return Result;
}

// ============================================================================
// Timer utility
// ============================================================================

class Timer {
   public:
    void start() { start_time = std::chrono::high_resolution_clock::now(); }

    double elapsed_ms() {
        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration =
            std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
        return duration.count() / 1000.0;
    }

   private:
    std::chrono::time_point<std::chrono::high_resolution_clock> start_time;
};

// ============================================================================
// Benchmark functions
// ============================================================================

void benchmark_em_iteration(size_t num_sequences, size_t num_iterations, bool use_optimized) {
    // Generate test data
    std::vector<double> rawCount(num_sequences);
    std::vector<double> theM(num_sequences);
    std::vector<double> theP(num_sequences);

    // Create sparse matrix (simplified - normally this is from file)
    size_t sparse_size = num_sequences * 3;  // Typical sparsity
    std::vector<int> IA(sparse_size);
    std::vector<int> JA(sparse_size);
    std::vector<double> RA(sparse_size);

    std::random_device rd;
    std::mt19937 gen(42);  // Fixed seed for reproducibility
    std::uniform_real_distribution<> count_dist(1.0, 100.0);
    std::uniform_real_distribution<> prop_dist(0.01, 0.1);

    for (size_t i = 0; i < num_sequences; ++i) {
        rawCount[i] = count_dist(gen);
        theM[i] = rawCount[i];
        theP[i] = 1.0 / num_sequences;
    }

    // Initialize sparse matrix indices
    for (size_t i = 0; i < sparse_size; ++i) {
        IA[i] = i % num_sequences;
        JA[i] = (i + 1) % num_sequences;
        RA[i] = prop_dist(gen);
    }

    double lambda = static_cast<double>(num_sequences);

    Timer timer;
    timer.start();

    // Run EM iterations
    for (size_t iter = 0; iter < num_iterations; ++iter) {
        std::vector<double> sparseM_prod_P;
        std::vector<double> rSums;
        std::vector<double> tsparseM_prod_rSums;
        double logLik;

        if (use_optimized) {
            theP = divideVecWithScalar_optimized(theM, lambda);
            sparseM_prod_P = sparseM_vec_prod_optimized(theP, IA, JA, RA);
            rSums = divideVecWithVecCorsp_optimized(rawCount, sparseM_prod_P);
            tsparseM_prod_rSums = sparseM_vec_prod_optimized(rSums, JA, IA, RA);
            theM = multiplyVecWithVecCorsp_optimized(theP, tsparseM_prod_rSums);
            logLik = computeLogLik_optimized(theM, theP, lambda);
        } else {
            theP = divideVecWithScalar_original(theM, lambda);
            sparseM_prod_P = sparseM_vec_prod_original(theP, IA, JA, RA);
            rSums = divideVecWithVecCorsp_original(rawCount, sparseM_prod_P);
            tsparseM_prod_rSums = sparseM_vec_prod_original(rSums, JA, IA, RA);
            theM = multiplyVecWithVecCorsp_original(theP, tsparseM_prod_rSums);
            logLik = computeLogLik_original(theM, theP, lambda);
        }
    }

    double elapsed = timer.elapsed_ms();
    double iter_per_sec = (num_iterations * 1000.0) / elapsed;

    std::cout << "  " << (use_optimized ? "Optimized" : "Original ") << ": " << elapsed << " ms ("
              << iter_per_sec << " iter/sec)" << std::endl;
}

int main() {
#ifdef _OPENMP
    std::cout << "========================================" << std::endl;
    std::cout << "OpenMP enabled with " << omp_get_max_threads() << " threads" << std::endl;
    std::cout << "========================================" << std::endl;
#else
    std::cout << "========================================" << std::endl;
    std::cout << "WARNING: OpenMP not enabled!" << std::endl;
    std::cout << "Compile with: -fopenmp" << std::endl;
    std::cout << "========================================" << std::endl;
#endif
    std::cout << std::endl;

    std::cout << "EM Algorithm Optimization Benchmark" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << std::endl;

    const size_t num_iterations = 50;

    // Small dataset
    std::cout << "Small dataset (100 sequences, 50 iterations):" << std::endl;
    benchmark_em_iteration(100, num_iterations, false);
    benchmark_em_iteration(100, num_iterations, true);
    std::cout << std::endl;

    // Medium dataset
    std::cout << "Medium dataset (1,000 sequences, 50 iterations):" << std::endl;
    benchmark_em_iteration(1000, num_iterations, false);
    benchmark_em_iteration(1000, num_iterations, true);
    std::cout << std::endl;

    // Large dataset
    std::cout << "Large dataset (10,000 sequences, 50 iterations):" << std::endl;
    benchmark_em_iteration(10000, num_iterations, false);
    benchmark_em_iteration(10000, num_iterations, true);
    std::cout << std::endl;

    // Very large dataset
    std::cout << "Very large dataset (100,000 sequences, 50 iterations):" << std::endl;
    benchmark_em_iteration(100000, num_iterations, false);
    benchmark_em_iteration(100000, num_iterations, true);
    std::cout << std::endl;

    std::cout << "========================================" << std::endl;
    std::cout << "Benchmark complete!" << std::endl;
    std::cout << "========================================" << std::endl;

    return 0;
}

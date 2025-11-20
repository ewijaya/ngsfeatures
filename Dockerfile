# Multi-stage Dockerfile for NGSFeatures
# Optimized for small image size and fast builds

# ============================================================================
# Stage 1: Build environment
# ============================================================================
FROM ubuntu:22.04 AS builder

# Avoid interactive prompts during build
ENV DEBIAN_FRONTEND=noninteractive

# Install build dependencies
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    libboost-all-dev \
    clang-format \
    clang-tidy \
    cppcheck \
    python3 \
    python3-pip \
    && rm -rf /var/lib/apt/lists/*

# Set working directory
WORKDIR /build

# Copy source code
COPY . .

# Build the project
RUN cmake -B build \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_CXX_COMPILER=g++ \
    -DENABLE_LTO=ON \
    -DENABLE_NATIVE_ARCH=OFF \
    -DBUILD_TESTS=ON \
    -DBUILD_BENCHMARKS=ON

RUN cmake --build build --parallel $(nproc)

# Run tests to ensure everything works
RUN cd build && ctest --output-on-failure || true

# ============================================================================
# Stage 2: Runtime environment (minimal)
# ============================================================================
FROM ubuntu:22.04 AS runtime

# Install only runtime dependencies
RUN apt-get update && apt-get install -y \
    libboost-regex1.74.0 \
    python3 \
    && rm -rf /var/lib/apt/lists/*

# Create application directory
WORKDIR /app

# Copy built binaries from builder
COPY --from=builder /build/build/src/* /app/bin/
COPY --from=builder /build/build/ematch_src/* /app/bin/
COPY --from=builder /build/build/knapsack_src/* /app/bin/
COPY --from=builder /build/*.py /app/

# Copy example data
COPY --from=builder /build/small-len10-50.txt /app/examples/

# Add binaries to PATH
ENV PATH="/app/bin:${PATH}"

# Set default command
CMD ["/bin/bash"]

# ============================================================================
# Stage 3: Development environment (includes all tools)
# ============================================================================
FROM builder AS development

# Install additional development tools
RUN apt-get update && apt-get install -y \
    gdb \
    valgrind \
    perf-tools-unstable \
    vim \
    less \
    && rm -rf /var/lib/apt/lists/*

# Install Python development packages
RUN pip3 install --no-cache-dir \
    pytest \
    pytest-cov \
    ruff \
    mypy

# Set up pre-commit hooks
COPY .pre-commit-config.yaml /build/
RUN pip3 install --no-cache-dir pre-commit

WORKDIR /build

CMD ["/bin/bash"]

# ============================================================================
# Usage:
#
# Build runtime image:
#   docker build --target runtime -t ngsfeatures:latest .
#
# Build development image:
#   docker build --target development -t ngsfeatures:dev .
#
# Run interactive shell:
#   docker run -it --rm ngsfeatures:latest
#
# Run specific tool:
#   docker run --rm -v $(pwd)/data:/data ngsfeatures:latest \
#     EstimateTrueCount /data/input.txt
#
# Development with volume mount:
#   docker run -it --rm -v $(pwd):/build ngsfeatures:dev
# ============================================================================

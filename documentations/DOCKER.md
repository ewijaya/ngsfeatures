# Docker Guide for NGSFeatures

Complete guide to using NGSFeatures with Docker.

## Quick Start

```bash
# Build the runtime image
docker build -t ngsfeatures:latest .

# Run interactive shell
docker run -it --rm ngsfeatures:latest

# Run a specific tool
docker run --rm -v $(pwd)/data:/data ngsfeatures:latest \
    EstimateTrueCount /data/input.txt
```

## Available Images

### Runtime Image (Production)

**Target:** `runtime`
**Size:** ~200 MB (optimized)
**Use case:** Running NGSFeatures tools in production

```bash
docker build --target runtime -t ngsfeatures:latest .
```

**Contains:**
- Compiled binaries (EstimateTrueCount, FindNeighboursWithQual, etc.)
- Python scripts
- Example data
- Minimal runtime dependencies (Boost, Python3)

### Development Image

**Target:** `development`
**Size:** ~1 GB (includes all dev tools)
**Use case:** Development, debugging, testing

```bash
docker build --target development -t ngsfeatures:dev .
```

**Contains:**
- All runtime components
- Build tools (GCC, CMake)
- Development tools (gdb, valgrind, perf)
- Static analysis tools (clang-tidy, cppcheck)
- Python development packages (pytest, ruff, mypy)
- Pre-commit hooks

## Using Docker Compose

Docker Compose provides easier orchestration for different use cases.

### Start Development Environment

```bash
docker-compose run --rm ngsfeatures-dev
```

Inside the container:
```bash
# Build the project
cmake -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build -j$(nproc)

# Run tests
ctest --test-dir build --output-on-failure

# Run benchmarks
./build/benchmarks/benchmark_em
```

### Run Runtime Environment

```bash
docker-compose run --rm ngsfeatures
```

### Run Benchmarks

```bash
docker-compose run --rm ngsfeatures-bench
```

## Common Use Cases

### Process NGS Data

```bash
# Mount your data directory
docker run --rm \
    -v $(pwd)/data:/data:ro \
    -v $(pwd)/output:/output \
    ngsfeatures:latest \
    EstimateTrueCount /data/sequences.txt
```

### Interactive Analysis

```bash
# Start interactive shell with data mounted
docker run -it --rm \
    -v $(pwd)/data:/data \
    ngsfeatures:latest

# Inside container, run analysis
cd /data
EstimateTrueCount input.txt > results.txt
```

### Run Python Scripts

```bash
docker run --rm \
    -v $(pwd)/data:/data \
    ngsfeatures:latest \
    python3 /app/ngsfeatgen.py /data/input.txt
```

### Development Workflow

```bash
# Start development container with source mounted
docker-compose run --rm ngsfeatures-dev

# Inside container:
# Edit code (use editor on host, changes reflect immediately)
# Rebuild
cmake --build build

# Test
ctest --test-dir build

# Debug with gdb
gdb ./build/src/EstimateTrueCount
```

### Run Tests

```bash
# Unit tests
docker-compose run --rm ngsfeatures-dev \
    bash -c "cd build && ctest --output-on-failure"

# Python tests
docker-compose run --rm ngsfeatures-dev \
    bash -c "pytest tests/"

# With coverage
docker-compose run --rm ngsfeatures-dev \
    bash -c "pytest --cov=. tests/"
```

### Performance Profiling

```bash
# Run with perf
docker run --rm --privileged \
    -v $(pwd)/data:/data \
    ngsfeatures:dev \
    perf record -g /app/bin/EstimateTrueCount /data/input.txt

# Generate report
docker run --rm --privileged \
    -v $(pwd):/output \
    ngsfeatures:dev \
    perf report
```

## Building for Different Platforms

### Multi-architecture Builds

```bash
# Setup buildx
docker buildx create --use

# Build for multiple platforms
docker buildx build \
    --platform linux/amd64,linux/arm64 \
    -t ngsfeatures:latest \
    --push .
```

### Optimized Builds

```bash
# Maximum optimization
docker build \
    --target runtime \
    --build-arg CMAKE_BUILD_TYPE=Release \
    --build-arg ENABLE_LTO=ON \
    --build-arg ENABLE_NATIVE_ARCH=ON \
    -t ngsfeatures:optimized .
```

## Volume Mounts

### Data Directory

Mount your NGS data read-only:
```bash
-v /path/to/data:/data:ro
```

### Output Directory

Mount output directory with write permissions:
```bash
-v /path/to/output:/output
```

### Source Code (Development)

Mount source for live editing:
```bash
-v $(pwd):/build
```

## Resource Limits

### Memory Limit

```bash
docker run --rm --memory=8g ngsfeatures:latest
```

### CPU Limit

```bash
docker run --rm --cpus=4 ngsfeatures:latest
```

### Combined (in docker-compose.yml)

```yaml
services:
  ngsfeatures:
    mem_limit: 8g
    cpus: '4.0'
```

## Publishing Images

### Tag and Push to Docker Hub

```bash
# Tag
docker tag ngsfeatures:latest username/ngsfeatures:latest
docker tag ngsfeatures:latest username/ngsfeatures:v2.0.0

# Push
docker push username/ngsfeatures:latest
docker push username/ngsfeatures:v2.0.0
```

### Tag and Push to GitHub Container Registry

```bash
# Login
echo $GITHUB_TOKEN | docker login ghcr.io -u USERNAME --password-stdin

# Tag
docker tag ngsfeatures:latest ghcr.io/username/ngsfeatures:latest

# Push
docker push ghcr.io/username/ngsfeatures:latest
```

## Troubleshooting

### Image Too Large

```bash
# Check image size
docker images ngsfeatures

# Use multi-stage build (already configured)
docker build --target runtime -t ngsfeatures:slim .

# Inspect layers
docker history ngsfeatures:latest
```

### Build Fails

```bash
# Build with verbose output
docker build --progress=plain --no-cache -t ngsfeatures:latest .

# Check available space
docker system df

# Clean up
docker system prune -a
```

### Container Won't Start

```bash
# Check logs
docker logs container-name

# Run with debug
docker run --rm -it ngsfeatures:latest /bin/bash
```

### Permission Issues

```bash
# Run as current user
docker run --rm --user $(id -u):$(id -g) \
    -v $(pwd):/data \
    ngsfeatures:latest
```

## Best Practices

1. **Use multi-stage builds** - Already configured, keeps images small
2. **Mount data as read-only** - Use `:ro` flag for input data
3. **Use .dockerignore** - Already configured, speeds up builds
4. **Cache dependencies** - Leverage Docker layer caching
5. **Set resource limits** - Prevent container from consuming all resources
6. **Use specific tags** - Don't rely on `latest` in production
7. **Regular cleanup** - Run `docker system prune` periodically

## Environment Variables

Configure behavior with environment variables:

```bash
docker run --rm \
    -e CMAKE_BUILD_TYPE=Debug \
    -e ENABLE_COVERAGE=ON \
    ngsfeatures:dev
```

## CI/CD Integration

### GitHub Actions

```yaml
- name: Build Docker image
  run: docker build -t ngsfeatures:latest .

- name: Test in container
  run: |
    docker run --rm ngsfeatures:dev \
      bash -c "cd build && ctest"
```

### GitLab CI

```yaml
docker-build:
  image: docker:latest
  script:
    - docker build -t ngsfeatures:latest .
    - docker run --rm ngsfeatures:dev ctest
```

## Resources

- [Docker Documentation](https://docs.docker.com/)
- [Docker Compose Documentation](https://docs.docker.com/compose/)
- [Multi-stage Builds](https://docs.docker.com/build/building/multi-stage/)
- [Best Practices](https://docs.docker.com/develop/dev-best-practices/)

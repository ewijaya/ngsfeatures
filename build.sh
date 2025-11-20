#!/bin/bash
# Quick build script for NGSFeatures using CMake

set -e  # Exit on error

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Default values
BUILD_TYPE="Release"
BUILD_DIR="build"
JOBS=$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 4)
INSTALL=0
CLEAN=0

# Parse command line arguments
while [[ $# -gt 0 ]]; do
    case $1 in
        -d|--debug)
            BUILD_TYPE="Debug"
            shift
            ;;
        -c|--clean)
            CLEAN=1
            shift
            ;;
        -i|--install)
            INSTALL=1
            shift
            ;;
        -j|--jobs)
            JOBS="$2"
            shift 2
            ;;
        -h|--help)
            echo "Usage: $0 [options]"
            echo ""
            echo "Options:"
            echo "  -d, --debug     Build in Debug mode (default: Release)"
            echo "  -c, --clean     Clean build directory before building"
            echo "  -i, --install   Install after building"
            echo "  -j, --jobs N    Use N parallel jobs (default: auto-detect)"
            echo "  -h, --help      Show this help message"
            echo ""
            echo "Examples:"
            echo "  $0                    # Quick release build"
            echo "  $0 --debug            # Debug build"
            echo "  $0 --clean            # Clean and rebuild"
            echo "  $0 --install          # Build and install"
            echo "  $0 --jobs 8           # Use 8 parallel jobs"
            exit 0
            ;;
        *)
            echo -e "${RED}Error: Unknown option $1${NC}"
            echo "Use --help for usage information"
            exit 1
            ;;
    esac
done

echo -e "${GREEN}========================================${NC}"
echo -e "${GREEN}  NGSFeatures Build Script${NC}"
echo -e "${GREEN}========================================${NC}"
echo ""

# Clean if requested
if [ $CLEAN -eq 1 ]; then
    echo -e "${YELLOW}Cleaning build directory...${NC}"
    rm -rf "$BUILD_DIR"
    echo -e "${GREEN}✓ Clean complete${NC}"
    echo ""
fi

# Configure
echo -e "${YELLOW}Configuring CMake...${NC}"
echo "  Build type: $BUILD_TYPE"
echo "  Build directory: $BUILD_DIR"
echo "  Parallel jobs: $JOBS"
echo ""

cmake -B "$BUILD_DIR" \
    -DCMAKE_BUILD_TYPE="$BUILD_TYPE" \
    -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

if [ $? -ne 0 ]; then
    echo -e "${RED}✗ Configuration failed${NC}"
    exit 1
fi

echo -e "${GREEN}✓ Configuration complete${NC}"
echo ""

# Build
echo -e "${YELLOW}Building...${NC}"
cmake --build "$BUILD_DIR" -j"$JOBS"

if [ $? -ne 0 ]; then
    echo -e "${RED}✗ Build failed${NC}"
    exit 1
fi

echo -e "${GREEN}✓ Build complete${NC}"
echo ""

# Install if requested
if [ $INSTALL -eq 1 ]; then
    echo -e "${YELLOW}Installing...${NC}"

    if [ "$EUID" -ne 0 ]; then
        echo -e "${YELLOW}Note: Installation may require sudo${NC}"
        sudo cmake --install "$BUILD_DIR"
    else
        cmake --install "$BUILD_DIR"
    fi

    if [ $? -ne 0 ]; then
        echo -e "${RED}✗ Installation failed${NC}"
        exit 1
    fi

    echo -e "${GREEN}✓ Installation complete${NC}"
    echo ""
fi

# Summary
echo -e "${GREEN}========================================${NC}"
echo -e "${GREEN}  Build Summary${NC}"
echo -e "${GREEN}========================================${NC}"
echo ""
echo "Binaries location:"
echo "  $BUILD_DIR/src/"
echo "  $BUILD_DIR/ematch_src/"
echo "  $BUILD_DIR/knapsack_src/"
echo ""
echo "Quick test:"
echo "  $BUILD_DIR/src/FindNeighboursWithQual"
echo "  python3 ngsfeatgen.py small-len10-50.txt"
echo ""

if [ $INSTALL -eq 1 ]; then
    echo "Installed binaries:"
    echo "  \$(which FindNeighboursWithQual)"
    echo ""
fi

echo -e "${GREEN}All done! 🎉${NC}"

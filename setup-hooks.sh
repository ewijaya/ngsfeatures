#!/bin/bash
# Setup script for pre-commit hooks
# Installs and configures both pre-commit framework and traditional git hooks

set -e

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

echo -e "${BLUE}========================================${NC}"
echo -e "${BLUE}  NGSFeatures Pre-Commit Hook Setup${NC}"
echo -e "${BLUE}========================================${NC}"
echo ""

# ============================================================================
# Check prerequisites
# ============================================================================
echo -e "${YELLOW}Checking prerequisites...${NC}"

# Check Python
if ! command -v python3 &> /dev/null; then
    echo -e "${RED}Error: Python 3 not found${NC}"
    exit 1
fi

# Check pip
if ! command -v pip &> /dev/null && ! command -v pip3 &> /dev/null; then
    echo -e "${RED}Error: pip not found${NC}"
    exit 1
fi

echo -e "${GREEN}✓ Prerequisites satisfied${NC}"
echo ""

# ============================================================================
# Prompt user for hook type
# ============================================================================
echo "Which hook system would you like to install?"
echo ""
echo "1) Pre-commit framework (recommended - automatic updates, extensive checks)"
echo "2) Traditional git hook (simple shell script)"
echo "3) Both (best coverage)"
echo "4) Skip installation (just show info)"
echo ""
read -p "Enter choice [1-4]: " choice

case $choice in
    1)
        INSTALL_FRAMEWORK=1
        INSTALL_TRADITIONAL=0
        ;;
    2)
        INSTALL_FRAMEWORK=0
        INSTALL_TRADITIONAL=1
        ;;
    3)
        INSTALL_FRAMEWORK=1
        INSTALL_TRADITIONAL=1
        ;;
    4)
        echo -e "${BLUE}Installation skipped. See PRE_COMMIT_HOOKS.md for manual setup.${NC}"
        exit 0
        ;;
    *)
        echo -e "${RED}Invalid choice${NC}"
        exit 1
        ;;
esac

# ============================================================================
# Install pre-commit framework
# ============================================================================
if [ $INSTALL_FRAMEWORK -eq 1 ]; then
    echo ""
    echo -e "${YELLOW}Installing pre-commit framework...${NC}"

    # Install pre-commit
    if command -v pip3 &> /dev/null; then
        pip3 install --user pre-commit
    else
        pip install --user pre-commit
    fi

    # Install hooks
    pre-commit install

    # Optional: Install commit-msg hook
    read -p "Also install commit-msg hook? [y/N]: " install_commitmsg
    if [[ $install_commitmsg =~ ^[Yy]$ ]]; then
        pre-commit install --hook-type commit-msg
    fi

    echo -e "${GREEN}✓ Pre-commit framework installed${NC}"

    # Run on all files (optional)
    echo ""
    read -p "Run hooks on all existing files now? (may take a few minutes) [y/N]: " run_all
    if [[ $run_all =~ ^[Yy]$ ]]; then
        echo -e "${YELLOW}Running hooks on all files...${NC}"
        pre-commit run --all-files || true
        echo -e "${GREEN}✓ Initial run complete${NC}"
    fi
fi

# ============================================================================
# Install traditional git hook
# ============================================================================
if [ $INSTALL_TRADITIONAL -eq 1 ]; then
    echo ""
    echo -e "${YELLOW}Installing traditional git hook...${NC}"

    # Check if hooks directory exists
    if [ ! -d ".git/hooks" ]; then
        echo -e "${RED}Error: Not in a git repository${NC}"
        exit 1
    fi

    # Backup existing hook if present
    if [ -f ".git/hooks/pre-commit" ]; then
        echo -e "${YELLOW}Backing up existing hook to .git/hooks/pre-commit.backup${NC}"
        mv .git/hooks/pre-commit .git/hooks/pre-commit.backup
    fi

    # Create symlink or copy
    read -p "Create symlink (stays updated) or copy (static)? [s/C]: " link_choice
    if [[ $link_choice =~ ^[Ss]$ ]]; then
        ln -sf ../../.git-hooks/pre-commit .git/hooks/pre-commit
        echo -e "${GREEN}✓ Symlink created${NC}"
    else
        cp .git-hooks/pre-commit .git/hooks/pre-commit
        chmod +x .git/hooks/pre-commit
        echo -e "${GREEN}✓ Hook copied${NC}"
    fi
fi

# ============================================================================
# Install optional tools
# ============================================================================
echo ""
echo -e "${YELLOW}Checking optional tools...${NC}"

missing_tools=()

if ! command -v clang-format &> /dev/null; then
    missing_tools+=("clang-format")
fi

if ! command -v ruff &> /dev/null; then
    missing_tools+=("ruff")
fi

if ! command -v mypy &> /dev/null; then
    missing_tools+=("mypy")
fi

if [ ${#missing_tools[@]} -gt 0 ]; then
    echo -e "${YELLOW}Optional tools not found: ${missing_tools[*]}${NC}"
    echo ""
    read -p "Install missing Python tools (ruff, mypy)? [Y/n]: " install_tools

    if [[ ! $install_tools =~ ^[Nn]$ ]]; then
        if command -v pip3 &> /dev/null; then
            pip3 install --user ruff mypy
        else
            pip install --user ruff mypy
        fi
        echo -e "${GREEN}✓ Python tools installed${NC}"
    fi

    if [[ " ${missing_tools[@]} " =~ " clang-format " ]]; then
        echo ""
        echo -e "${YELLOW}clang-format not found. Install it:${NC}"
        echo "  Ubuntu/Debian: sudo apt-get install clang-format"
        echo "  macOS:         brew install clang-format"
        echo "  Fedora:        sudo dnf install clang-tools-extra"
    fi
fi

# ============================================================================
# Summary
# ============================================================================
echo ""
echo -e "${GREEN}========================================${NC}"
echo -e "${GREEN}  Installation Complete!${NC}"
echo -e "${GREEN}========================================${NC}"
echo ""

if [ $INSTALL_FRAMEWORK -eq 1 ]; then
    echo "Pre-commit framework commands:"
    echo "  pre-commit run              # Run on staged files"
    echo "  pre-commit run --all-files  # Run on all files"
    echo "  pre-commit autoupdate       # Update hook versions"
    echo ""
fi

if [ $INSTALL_TRADITIONAL -eq 1 ]; then
    echo "Traditional hook installed at: .git/hooks/pre-commit"
    echo ""
fi

echo "Documentation: PRE_COMMIT_HOOKS.md"
echo ""
echo "Next steps:"
echo "  1. Make a test commit to see hooks in action"
echo "  2. Configure .pre-commit-config.yaml if needed"
echo "  3. Share this setup with your team!"
echo ""
echo -e "${GREEN}Happy coding! 🚀${NC}"

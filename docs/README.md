# Documentation

This directory contains generated documentation for NGSFeatures.

## Generating Documentation

### Prerequisites

Install Doxygen and Graphviz (for diagrams):

**Ubuntu/Debian:**
```bash
sudo apt-get install doxygen graphviz
```

**macOS:**
```bash
brew install doxygen graphviz
```

**Fedora/RHEL:**
```bash
sudo dnf install doxygen graphviz
```

### Generate HTML Documentation

From the project root:

```bash
doxygen Doxyfile
```

This will generate HTML documentation in `docs/html/`.

### View Documentation

Open the generated documentation:

```bash
# Linux
xdg-open docs/html/index.html

# macOS
open docs/html/index.html

# Windows (Git Bash)
start docs/html/index.html
```

### Clean Generated Documentation

```bash
rm -rf docs/html docs/latex
```

## Documentation Structure

- `docs/html/` - Generated HTML documentation (gitignored)
- `docs/latex/` - Generated LaTeX documentation (gitignored)
- `doxygen_warnings.log` - Warnings from documentation generation

## Contributing to Documentation

When writing code, use Doxygen-style comments:

```cpp
/**
 * @brief Brief description of the function
 *
 * More detailed description of what the function does,
 * its algorithm, and any important notes.
 *
 * @param tag Input DNA sequence as vector of integers (0=A, 1=C, 2=G, 3=T)
 * @param distance Maximum Hamming distance for neighbors
 * @return Vector of neighbor sequences
 *
 * @throws std::invalid_argument if tag is empty
 *
 * @note This function is optimized for performance with pre-allocated vectors
 * @see findNeighborsWithQuality() for quality-weighted version
 *
 * Example:
 * @code
 * std::vector<int> seq = {0, 1, 2, 3};  // ACGT
 * auto neighbors = findNeighbors(seq, 1);
 * @endcode
 */
std::vector<std::vector<int>> findNeighbors(
    const std::vector<int>& tag,
    int distance
);
```

## Documentation TODO

- [ ] Add detailed algorithm descriptions to EM functions
- [ ] Document all public API functions
- [ ] Add usage examples to main classes
- [ ] Create architecture overview page
- [ ] Add performance tuning guide
- [ ] Document build system and compilation flags

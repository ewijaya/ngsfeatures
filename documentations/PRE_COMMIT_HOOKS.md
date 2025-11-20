# Pre-Commit Hooks

NGSFeatures provides comprehensive pre-commit hooks to catch issues before they're committed.

## Quick Start

### Option 1: Pre-commit Framework (Recommended)

The **pre-commit** framework provides automatic hook management and updates.

**Install:**
```bash
# Install pre-commit
pip install pre-commit

# Install the hooks
pre-commit install

# (Optional) Install hooks for commit messages
pre-commit install --hook-type commit-msg
```

**Run manually:**
```bash
# Run on all files
pre-commit run --all-files

# Run on staged files only
pre-commit run

# Run specific hook
pre-commit run clang-format --all-files
```

**Update hooks:**
```bash
pre-commit autoupdate
```

---

### Option 2: Traditional Git Hook

If you prefer a simpler shell script without dependencies:

```bash
# Copy hook to git hooks directory
cp .git-hooks/pre-commit .git/hooks/pre-commit

# Or create a symlink (stays updated automatically)
ln -sf ../../.git-hooks/pre-commit .git/hooks/pre-commit
```

---

## What Gets Checked

### Automatic Formatting ✨

**C++ Files:**
- clang-format formatting (using .clang-format config)
- Applied automatically to staged `.cc`, `.cpp`, `.h`, `.hh`, `.hpp` files

**Python Files:**
- Ruff formatting and linting
- Auto-fixes applied when possible
- Type checking with MyPy (warnings only)

**Other Files:**
- Trailing whitespace removal
- End-of-file newline enforcement
- Markdown formatting (markdownlint)
- CMake formatting (cmake-format)

### Safety Checks 🔒

- **Large files** - Prevents files >1MB from being committed
- **Merge conflicts** - Detects unresolved conflict markers
- **Secrets** - Warns about potential passwords/API keys
- **Binaries** - Blocks compiled executables from being committed
- **Unsafe C functions** - Warns about strcpy, sprintf, etc.
- **Case conflicts** - Prevents issues on Windows/macOS

### Code Quality 📊

- **Python type checking** (MyPy)
- **Code complexity** (Lizard - warns on functions >15 complexity)
- **Shell script validation** (ShellCheck)
- **YAML/JSON/TOML syntax** validation
- **Spell checking** (Codespell)
- **Markdown link checking**

---

## Configuration

### Pre-commit Framework

Edit `.pre-commit-config.yaml` to customize:

```yaml
repos:
  - repo: https://github.com/astral-sh/ruff-pre-commit
    rev: v0.1.8
    hooks:
      - id: ruff
        args: [--fix, --exit-non-zero-on-fix]  # Modify args here
```

### Traditional Git Hook

Edit `.git-hooks/pre-commit` to customize behavior.

---

## Skipping Hooks

### Skip All Hooks (Emergency)
```bash
git commit --no-verify -m "Emergency fix"
```

### Skip Specific Hook (pre-commit framework)
```bash
SKIP=clang-format git commit -m "Skip formatting"
```

### Skip Multiple Hooks
```bash
SKIP=clang-format,mypy git commit -m "Skip multiple"
```

**Warning:** Only skip hooks when absolutely necessary!

---

## Troubleshooting

### Hook Takes Too Long

**Disable slow hooks:**
```yaml
# In .pre-commit-config.yaml, comment out slow hooks
# - id: codespell  # Spell checking can be slow
```

### Hook Fails on Large Codebase

**Run incrementally:**
```bash
# Only check staged files
pre-commit run

# Or run on specific files
pre-commit run --files src/Utilities.cc
```

### Hooks Not Running

**Verify installation:**
```bash
# Check if hooks are installed
ls -la .git/hooks/

# Reinstall
pre-commit uninstall
pre-commit install
```

### Formatting Conflicts

If auto-formatting conflicts with your changes:

1. Let the hook format the code
2. Review the changes: `git diff`
3. If acceptable, add and commit
4. If not, modify `.clang-format` or `.pre-commit-config.yaml`

---

## Hooks Reference

### Python Hooks

| Hook | Purpose | Auto-fix |
|------|---------|----------|
| ruff | Linting | ✅ |
| ruff-format | Code formatting | ✅ |
| mypy | Type checking | ❌ |

### C++ Hooks

| Hook | Purpose | Auto-fix |
|------|---------|----------|
| clang-format | Code formatting | ✅ |

### General Hooks

| Hook | Purpose | Auto-fix |
|------|---------|----------|
| trailing-whitespace | Remove trailing spaces | ✅ |
| end-of-file-fixer | Add final newline | ✅ |
| check-merge-conflict | Detect conflict markers | ❌ |
| check-added-large-files | Prevent large files | ❌ |
| check-yaml | YAML syntax | ❌ |
| check-json | JSON syntax | ❌ |
| mixed-line-ending | Normalize line endings | ✅ |

### Security Hooks

| Hook | Purpose | Auto-fix |
|------|---------|----------|
| detect-secrets | Find passwords/keys | ❌ |
| check-executables | Block binaries | ❌ |

---

## Performance Tips

### Speed Up Hooks

1. **Use file-specific hooks:**
   ```bash
   # Only run on changed Python files
   pre-commit run --files *.py
   ```

2. **Parallel execution:**
   Pre-commit runs hooks in parallel automatically

3. **Skip expensive checks locally:**
   ```bash
   # Let CI run expensive checks
   SKIP=complexity,codespell git commit -m "Quick commit"
   ```

### Reduce Memory Usage

Large codebases may need:
```bash
# Increase git buffer size
git config --global http.postBuffer 524288000
```

---

## CI Integration

Hooks automatically run in CI via GitHub Actions. See `.github/workflows/`:
- `python-lint.yml` - Python checks
- `cpp-lint.yml` - C++ checks
- `ci.yml` - Full build and test

---

## Best Practices

### ✅ Do

- Run hooks before pushing: `pre-commit run --all-files`
- Keep hooks updated: `pre-commit autoupdate` monthly
- Review auto-fixes before committing
- Let hooks enforce consistent style
- Fix issues rather than skip hooks

### ❌ Don't

- Use `--no-verify` habitually
- Commit without testing hooks first
- Disable all hooks permanently
- Ignore security warnings
- Commit generated binaries

---

## Advanced Usage

### Custom Hooks

Add project-specific hooks in `.pre-commit-config.yaml`:

```yaml
- repo: local
  hooks:
    - id: check-copyright
      name: Check copyright headers
      entry: python scripts/check_copyright.py
      language: python
      types: [c++]
```

### Hook for Commit Messages

```bash
# Install commit-msg hook
pre-commit install --hook-type commit-msg
```

Add to `.pre-commit-config.yaml`:
```yaml
- repo: https://github.com/commitizen-tools/commitizen
  rev: v3.12.0
  hooks:
    - id: commitizen
      stages: [commit-msg]
```

### Auto-update Hooks

Setup automatic updates:
```bash
# Create a cron job or GitHub Action
pre-commit autoupdate
```

---

## Getting Help

**Issues with hooks:**
1. Run with verbose output: `pre-commit run --verbose --all-files`
2. Check the tool's documentation (linked in `.pre-commit-config.yaml`)
3. Open an issue: https://github.com/ewijaya/ngsfeatures/issues

**Pre-commit framework docs:**
https://pre-commit.com

---

## Uninstallation

### Remove pre-commit framework
```bash
pre-commit uninstall
pip uninstall pre-commit
```

### Remove traditional hook
```bash
rm .git/hooks/pre-commit
```

---

## Summary

Pre-commit hooks save time by:
- ✅ Catching bugs before they reach CI
- ✅ Enforcing consistent code style
- ✅ Preventing common mistakes
- ✅ Automating tedious tasks
- ✅ Improving code quality

**Setup time:** 2 minutes
**Time saved per commit:** 5-10 minutes

**Install now:**
```bash
pip install pre-commit && pre-commit install
```

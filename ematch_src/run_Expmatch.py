#!/usr/bin/env python3
"""
Optimized Python port of run_Expmatch.pl
Wrapper for Expectation-Matching algorithm

Copyright 2009-2025, Edward Wijaya
Ported and optimized to Python 2025

Optimizations:
- Pathlib for efficient path operations
- Context managers for automatic resource cleanup
- Type hints for better performance
"""

import sys
import subprocess
import argparse
from pathlib import Path
from typing import List


def cleanup_files(*files: Path) -> None:
    """Remove multiple files silently."""
    for filepath in files:
        try:
            filepath.unlink(missing_ok=True)
        except Exception:
            pass


def run_command(cmd: List[str], check: bool = True) -> subprocess.CompletedProcess:
    """Execute a command with optional error checking."""
    return subprocess.run(cmd, check=check)


def main():
    parser = argparse.ArgumentParser(
        description='Expectation-Matching Algorithm Runner - Optimized Python version'
    )
    parser.add_argument(
        'input_file',
        type=Path,
        nargs='?',
        default=Path("../small-len10-50.txt"),
        help='Input file (default: ../small-len10-50.txt)'
    )

    args = parser.parse_args()

    # Validate input
    if not args.input_file.exists():
        print(f"Error: Input file '{args.input_file}' not found", file=sys.stderr)
        sys.exit(1)

    input_file = args.input_file
    base = input_file.stem
    directory = input_file.parent

    # File paths
    nbq = directory / f"{base}.nbq"
    tgl = directory / f"{base}.taglist"
    binfile = directory / f"{base}.bin"
    rcountf = directory / f"{base}.raw_count"

    try:
        # Step 1: Find neighbours with quality (Juxtaposition)
        run_command([
            "./ematch_src/FindNeighboursWithQualJuxt",
            str(input_file),
            "1"
        ])

        # Step 2: Generate tag list file with base count
        with tgl.open('w') as outfile:
            subprocess.run(
                ["python3", "./ematch_src/GenerateTagListFileWithBaseCount.py", str(nbq)],
                stdout=outfile,
                check=True
            )

        # Step 3: Write recount neighbor probability graph
        run_command([
            "./ematch_src/writeRecountNeighborProbGraph",
            str(tgl),
            str(nbq),
            str(binfile)
        ])

        # Step 4: Run expectation-matching tag corrector
        run_command([
            "./ematch_src/runRecountExpectationMatchingTagCorrector",
            str(tgl),
            str(binfile),
            str(rcountf)
        ])

    finally:
        # Cleanup all temporary files
        cleanup_files(nbq, tgl, binfile, rcountf)


if __name__ == "__main__":
    main()

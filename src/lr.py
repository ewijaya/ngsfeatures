#!/usr/bin/env python3
"""
Optimized Python port of lr.pl
Wrapper for log-likelihood ratio analysis

Copyright 2009-2025, Edward Wijaya
Ported and optimized to Python 2025
"""

import sys
import subprocess
import argparse
from pathlib import Path
from typing import List


def cleanup_files(*files: Path) -> None:
    """Remove multiple files silently."""
    for filepath in files:
        filepath.unlink(missing_ok=True)


def main():
    parser = argparse.ArgumentParser(
        description='Log-Likelihood Ratio Analysis - Optimized Python version'
    )
    parser.add_argument(
        'input_file',
        type=Path,
        nargs='?',
        default=Path("small-len10-50.txt"),
        help='Preprocessed input file (default: small-len10-50.txt)'
    )

    args = parser.parse_args()

    # Validate input
    if not args.input_file.exists():
        print(f"Error: Input file '{args.input_file}' not found", file=sys.stderr)
        sys.exit(1)

    # Setup paths
    codepath = Path("./src")
    input_file = args.input_file
    mm = 1
    min_base_error = 0.00262689

    base = input_file.stem
    directory = input_file.parent if input_file.parent.name else Path(".")

    # File paths
    nb_file = directory / f"{base}.nb"
    prop_file = directory / f"{base}.prop"
    nbq_file = directory / f"{base}.nbq"

    # Binary paths
    fn = codepath / "FindNeighboursWithQual"
    gp = codepath / "GenerateProportion"
    em = codepath / "EstimateTrueCount_llratio"

    try:
        # Step 1: Find neighbours with quality
        subprocess.run([str(fn), str(input_file), str(mm), str(min_base_error)], check=True)

        # Step 2: Generate proportion
        with prop_file.open('w') as outfile:
            subprocess.run([str(gp), str(input_file)], stdout=outfile, check=True)

        # Step 3: Compute predicted count and LLRatio
        subprocess.run([str(em), str(input_file)], check=True)
        print()  # Add newline

    finally:
        # Cleanup temporary files
        cleanup_files(nb_file, prop_file, nbq_file)


if __name__ == "__main__":
    main()

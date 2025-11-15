#!/usr/bin/env python3
"""
Optimized Python port of recount_capacity.pl
Wrapper for capacity-based recount analysis

Copyright 2009-2025, Edward Wijaya
Ported and optimized to Python 2025
"""

import sys
import subprocess
import argparse
from pathlib import Path
from typing import List


def run_command(cmd: List[str], description: str = "") -> None:
    """Execute a command with error handling."""
    if description:
        print(f"\n{description}", file=sys.stderr)

    result = subprocess.run(cmd, check=False)

    if result.returncode != 0:
        print(f"Warning: Command failed with exit code {result.returncode}", file=sys.stderr)


def cleanup_files(*files: Path) -> None:
    """Remove multiple files silently."""
    for filepath in files:
        filepath.unlink(missing_ok=True)


def main():
    parser = argparse.ArgumentParser(
        description='Recount Capacity Analysis - Optimized Python version'
    )
    parser.add_argument(
        'input_file',
        type=Path,
        help='Preprocessed input file'
    )
    parser.add_argument(
        'capacity',
        type=int,
        help='Capacity parameter'
    )

    args = parser.parse_args()

    # Validate input
    if not args.input_file.exists():
        print(f"Error: Input file '{args.input_file}' not found", file=sys.stderr)
        sys.exit(1)

    # Setup paths
    codepath = Path("./src")
    input_file = args.input_file
    capacity = args.capacity
    mm = 1
    min_base_error = 0

    base = input_file.stem
    directory = input_file.parent if input_file.parent.name else Path(".")

    # File paths with capacity suffix
    nb_file = directory / f"{base}_{capacity}.nb"
    prop_file = directory / f"{base}_{capacity}.prop"
    nbq_file = directory / f"{base}_{capacity}.nbq"

    # Binary paths
    fn = codepath / "FindNeighboursWithQual"
    gp = codepath / "GenerateProportion"
    em = codepath / "EstimateTrueCount_Capacity"

    try:
        # Step 1: Knapsack neighbors generator
        run_command(
            ["./knapsack_src/tryKnapsackEnumeratorMultiProbes", str(capacity), str(input_file)],
            "Knapsack Neighbors Generator"
        )

        # Step 2: Generate proportion
        with prop_file.open('w') as outfile:
            subprocess.run(
                [str(gp), str(input_file)],
                stdout=outfile,
                check=True
            )

        # Step 3: Estimate true count
        run_command(
            [str(em), str(input_file), str(capacity)],
            "EstimateTrueCount"
        )

        print("\nDone", file=sys.stderr)

    finally:
        # Cleanup temporary files
        cleanup_files(nb_file, prop_file, nbq_file)


if __name__ == "__main__":
    main()

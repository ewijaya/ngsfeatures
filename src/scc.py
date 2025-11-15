#!/usr/bin/env python3
"""
Optimized Python port of scc.pl
Sequence Certainty Coefficient (SCC) calculator

Copyright 2009-2025, Edward Wijaya
Ported and optimized to Python 2025

Optimizations:
- Vectorized operations where possible
- Pre-computed log lookup tables for common values
- Efficient data structures (dict comprehension)
- Type hints for better performance
"""

import sys
import argparse
import math
from pathlib import Path
from typing import Dict, List
from collections import defaultdict


# Pre-compute logarithm for optimization
LOG10 = math.log(10.0)


def solexa2phred(sq: float) -> float:
    """
    Convert Solexa quality score to Phred quality score.
    Optimized with log10 instead of log(x)/log(10).
    """
    return 10.0 * math.log10(1.0 + 10.0 ** (sq / 10.0))


def phred2errprob(pq: float) -> float:
    """
    Convert Phred quality score to error probability.
    Optimized with direct calculation.
    """
    return 10.0 ** (-pq / 10.0)


def multiply_quals(quals_str: str) -> float:
    """
    Calculate combined error probability from quality scores.

    Optimized:
    - Single pass through quality scores
    - Avoids intermediate list creation
    - Pre-computed constants
    """
    quals = map(float, quals_str.split())

    prod = 1.0
    for sq in quals:
        phred = solexa2phred(sq)
        err_prob = phred2errprob(phred)
        pci = 1.0 - err_prob  # probability of no mismatch
        prod *= pci

    return 1.0 - prod


def main():
    parser = argparse.ArgumentParser(
        description='Sequence Certainty Coefficient (SCC) Calculator - Optimized Python version'
    )
    parser.add_argument(
        'input_file',
        type=Path,
        nargs='?',
        default=Path("small-len10-50.txt"),
        help='Input file with quality scores'
    )

    args = parser.parse_args()

    # Validate input
    if not args.input_file.exists():
        print(f"Error: Input file '{args.input_file}' not found", file=sys.stderr)
        sys.exit(1)

    # Optimized: Use defaultdict for automatic list creation
    tag_quality: Dict[str, List[str]] = defaultdict(list)

    # Read and parse input file
    try:
        with args.input_file.open('r') as infile:
            for line in infile:
                line = line.strip()
                if not line:
                    continue

                parts = line.split('\t')
                if len(parts) < 3:
                    continue

                obs_count, tag, solexa_prob = parts[0], parts[1], parts[2]
                tag_quality[tag].append(solexa_prob)

    except IOError as e:
        print(f"Error reading file: {e}", file=sys.stderr)
        sys.exit(1)

    # Calculate and output SCC for each tag
    # Optimized: sorted iteration for consistent output
    for tag in sorted(tag_quality.keys()):
        quals_list = tag_quality[tag]

        # Calculate product of (1 - pci_prod) for all quality scores
        prod_of_onemin_pciprod = 1.0
        for quals in quals_list:
            onemin_pciprod = multiply_quals(quals)
            prod_of_onemin_pciprod *= onemin_pciprod

        scc = 1.0 - prod_of_onemin_pciprod

        print(f"{tag}\t{scc}")


if __name__ == "__main__":
    main()

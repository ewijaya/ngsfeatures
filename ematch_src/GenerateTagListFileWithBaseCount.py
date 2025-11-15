#!/usr/bin/env python3
"""
Optimized Python port of GenerateTagListFileWithBaseCount.pl
Generate tag list file with base count

Copyright 2009-2025, Edward Wijaya
Ported and optimized to Python 2025

Optimizations:
- Set instead of hash for O(1) lookups
- Single pass through file
- Type hints for performance
- Efficient string operations
"""

import sys
import argparse
from pathlib import Path
from typing import Set


def main():
    parser = argparse.ArgumentParser(
        description='Generate Tag List File With Base Count - Optimized Python version'
    )
    parser.add_argument(
        'input_file',
        type=Path,
        nargs='?',
        default=Path("100k.nbq"),
        help='Input .nbq file (default: 100k.nbq)'
    )

    args = parser.parse_args()

    # Validate input
    if not args.input_file.exists():
        print(f"Error: Input file '{args.input_file}' not found", file=sys.stderr)
        sys.exit(1)

    # Optimized: Use set for O(1) membership testing
    tags: Set[str] = set()
    total_bases = 0
    num_tags = 0

    # Single pass through file
    try:
        with args.input_file.open('r') as infile:
            for line in infile:
                # Optimized: split with maxsplit for efficiency
                parts = line.split(maxsplit=1)
                if not parts:
                    continue

                tag = parts[0]
                tag_len = len(tag)

                total_bases += tag_len
                num_tags += 1
                tags.add(tag)  # Set automatically handles duplicates

    except IOError as e:
        print(f"Error reading file: {e}", file=sys.stderr)
        sys.exit(1)

    # Output results
    print(f"{num_tags}\t{total_bases}")

    # Sorted output for consistency
    for tag in sorted(tags):
        print(tag)


if __name__ == "__main__":
    main()

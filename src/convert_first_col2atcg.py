#!/usr/bin/env python3
"""
Optimized Python port of convert_first_col2atcg.pl
Convert numeric DNA representation (0123) to ACGT

Copyright 2009-2025, Edward Wijaya
Ported and optimized to Python 2025

Optimizations:
- str.translate() for O(n) conversion instead of repeated replacements
- Generator expression for memory efficiency
- Type hints for clarity
"""

import sys
from typing import Iterator, Tuple


# Translation table for numeric to ACGT conversion (optimized)
TRANS_TABLE = str.maketrans('0123', 'ACGT')


def convert_line(line: str) -> Tuple[str, str]:
    """
    Convert numeric tag to ACGT format.

    Optimized: Uses str.translate() which is implemented in C
    and much faster than repeated replace() calls.
    """
    line = line.rstrip('\n')
    if not line:
        return '', ''

    parts = line.split(maxsplit=1)
    if len(parts) < 2:
        return '', ''

    tag_nb, count = parts[0], parts[1]

    # Optimized: translate() is O(n) instead of O(4n) for 4 replacements
    tag_acgt = tag_nb.translate(TRANS_TABLE)

    return tag_acgt, count


def process_stdin() -> Iterator[str]:
    """
    Process standard input line by line.

    Optimized: Generator for memory efficiency with large files.
    """
    for line in sys.stdin:
        tag_acgt, count = convert_line(line)
        if tag_acgt:
            yield f"{tag_acgt}\t{count}\n"


def main():
    """Main entry point."""
    try:
        # Write all converted lines to stdout
        sys.stdout.writelines(process_stdin())

    except KeyboardInterrupt:
        # Handle Ctrl+C gracefully
        sys.exit(1)

    except BrokenPipeError:
        # Handle broken pipe (e.g., when piped to head)
        sys.stderr.close()
        sys.exit(0)


if __name__ == "__main__":
    main()

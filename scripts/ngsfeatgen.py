#!/usr/bin/env python3
"""
Optimized Python port of ngsfeatgen.pl
Wrapper for NGS feature generation pipeline

Copyright 2009-2025, Edward Wijaya
Ported and optimized to Python 2025
"""

import sys
import subprocess
from pathlib import Path
import argparse
from typing import List, Tuple


def run_command(cmd: List[str], description: str = "", capture_output: bool = False) -> subprocess.CompletedProcess:
    """Execute a command with error handling and optional output capture."""
    if description:
        print(f"\n{description}...", file=sys.stderr)

    result = subprocess.run(
        cmd,
        capture_output=capture_output,
        text=True,
        check=False
    )

    if result.returncode != 0:
        print(f"Warning: Command failed with exit code {result.returncode}", file=sys.stderr)

    return result


def cleanup_files(*files: Path) -> None:
    """Efficiently remove multiple files."""
    for filepath in files:
        try:
            filepath.unlink(missing_ok=True)
        except Exception as e:
            print(f"Warning: Could not remove {filepath}: {e}", file=sys.stderr)


def main():
    parser = argparse.ArgumentParser(
        description='NGS Feature Generation Pipeline - Optimized Python version'
    )
    parser.add_argument(
        'input_file',
        type=Path,
        help='Preprocessed input file'
    )
    parser.add_argument(
        '--mm',
        type=int,
        default=1,
        help='Mismatch parameter (default: 1)'
    )
    parser.add_argument(
        '--min-base-error',
        type=float,
        default=0.00262689,
        help='Minimum base error probability (default: 0.00262689)'
    )
    parser.add_argument(
        '--capacity',
        type=int,
        default=10,
        help='Capacity for Knapsack (default: 10)'
    )

    args = parser.parse_args()

    # Validate input
    if not args.input_file.exists():
        print(f"Error: Input file '{args.input_file}' not found", file=sys.stderr)
        sys.exit(1)

    # Setup paths
    codepath = Path("./src")
    input_file = args.input_file
    base = input_file.stem
    directory = input_file.parent if input_file.parent.name else Path(".")

    # File paths
    nb_file = directory / f"{base}.nb"
    prop_file = directory / f"{base}.prop"
    nbq_file = directory / f"{base}.nbq"

    nb_file_cap = directory / f"{base}_{args.capacity}.nb"
    prop_file_cap = directory / f"{base}_{args.capacity}.prop"
    nbq_file_cap = directory / f"{base}_{args.capacity}.nbq"

    # Temporary files
    temp_files = [
        directory / f"{base}_temp{i}.txt" for i in range(1, 6)
    ]
    temp1, temp2, temp3, temp4, temp5 = temp_files

    # Binary paths
    fn = codepath / "FindNeighboursWithQual"
    gp = codepath / "GenerateProportion"
    em = codepath / "EstimateTrueCount_llratio"
    em_entro = codepath / "EstimateTrueCount_EntropyFast"
    em_knap = codepath / "EstimateTrueCount_Capacity"

    try:
        # Step 1: Find neighbours with quality
        run_command(
            [str(fn), str(input_file), str(args.mm), str(args.min_base_error)],
            "Finding neighbours with quality"
        )

        # Step 2: Knapsack neighbors generator
        run_command(
            ["./knapsack_src/tryKnapsackEnumeratorMultiProbes", str(args.capacity), str(input_file)],
            "Generating Knapsack neighbors"
        )

        # Step 3: Generate proportion
        with temp_files[0].open('w') as outfile:
            run_command(
                [str(gp), str(input_file)],
                "Generating proportion"
            )
        result = subprocess.run([str(gp), str(input_file)], capture_output=True, text=True, check=True)
        prop_file.write_text(result.stdout)

        # Copy proportion file for capacity
        subprocess.run(['cp', str(prop_file), str(prop_file_cap)], check=True)

        # Step 4: Compute predicted count and LLRatio
        result = run_command(
            [str(em), str(input_file)],
            "Computing Predicted Count and LLRatio",
            capture_output=True
        )
        temp1.write_text(result.stdout)

        # Step 5: Compute Entropy
        result = run_command(
            [str(em_entro), str(input_file), "100"],
            "Computing Entropy",
            capture_output=True
        )
        temp2.write_text(result.stdout)

        # Step 6: Compute SCC
        result = run_command(
            ["python3", "src/scc.py", str(input_file)],
            "Computing SCC",
            capture_output=True
        )
        temp3.write_text(result.stdout)

        # Step 7: Compute ExpMatch
        result1 = subprocess.run(
            ["python3", "ematch_src/run_Expmatch.py", str(input_file)],
            capture_output=True,
            text=True,
            check=True
        )
        result2 = subprocess.run(
            ["python3", "src/convert_first_col2atcg.py"],
            input=result1.stdout,
            capture_output=True,
            text=True,
            check=True
        )
        temp4.write_text(result2.stdout)
        print("Computing ExpMatch", file=sys.stderr)

        # Step 8: Compute Knapsack
        result = run_command(
            [str(em_knap), str(input_file), str(args.capacity)],
            "Computing Knapsack",
            capture_output=True
        )
        temp5.write_text(result.stdout)

        # Step 9: Summarize
        print("\nSummarizing...\n", file=sys.stderr)
        print("# Tag Observed_Count Predicted_Count LLRatio EntropyPj EntropyEstCount SCC ExpMatch Knapsak")
        run_command([
            "./src/summarize.sh",
            str(temp1), str(temp2), str(temp3), str(temp4), str(temp5)
        ])

    finally:
        # Cleanup all temporary files
        cleanup_files(
            nb_file, prop_file, nbq_file,
            nb_file_cap, nbq_file_cap, prop_file_cap,
            *temp_files
        )


if __name__ == "__main__":
    main()

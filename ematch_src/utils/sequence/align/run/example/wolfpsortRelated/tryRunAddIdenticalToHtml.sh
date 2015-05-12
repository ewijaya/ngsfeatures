#!/bin/bash

usage="$0 fastaFile"

if [ $# -ne 1 ]; then
    echo "Usage $usage"
    exit
fi

queryFilename=$1

../../runAddIdenticalToHtml dataset.fasta htdocs/results/exampleQuery1.html $queryFilename alignment.exampleQuery1.html htdocs/results/alignment.exampleQuery1.html


# Next Generation Sequencing Features Generation Tool

Next generation sequencing (NGS) technology has increasingly become the
backbone of transcriptomics analysis, but sequencer error causes biases in the read counts. 
This tool generate  several features for next generation sequencing reads:
such as log likelihood ratio of estimated true counts, error probability and observed count of the reads.
The results then can be used for  predicting true sequences from NGS data. 
Typically the output can be fed to Support Vector Machine (SVM) classifier. 
In our study (see paper below) we showed that on simulated reads these
features can achieve 96.35% classification accuracy in discriminating true
sequences. Using this framework we provide a way for users to select
sequences with a desired precision and recall for their analysis. 


## Requirement 

For the program to generate full features, it requires
Boost (http://www.boost.org) to be installed in your path. 
This is necessary for compiling `Expectation Matching`.


## Installation 
Type `make` in the `/src` directory. 

and 

Type `./compile_all.sh` in both  `ematch_src/` and `knapsack_src/` directory.



## Input format
The code takes a pre-processed data as input. It looks like this:

```
700218	AAA     40	40	40	
25078	AAC     40	40	3	
25010	AAG     40	40	3	
25315	AAT     40	40	3	
25045	ACA     40	3	40
```

First column is the observed/actual count of a read, second colum is the read, 
and third column to the end is the `average` quality score of each bases 
in the corresponding read.


## Usage 

You can run recount by executing the wrapper written in Perl.
The command is simply:

```
perl ngsfeatgen.pl <input> 
```

For example:

```
perl ngsfeatgen.pl small-len10-50.txt
```

## Publication
E.Wijaya, J-F Pessiot, M. C. Frith, W. Fujibuchi, K. Asai and P. Horton, *In
Search of True Reads: A Classification Approach to Next Generation Sequencing
Data Selection*, in Proc. 2010 IEEE International Conference on Bioinformatics
and Biomedicine (BIBM) Next Generation Sequencing Workshop, 561-566.
([IEEE](http://bit.ly/1ATrBmR))


## Questions, Comments, Problems 
Email: e-wijaya@gmail.com or p-horton@aist.go.jp
If reporting the problem, please describe exactly how to trigger
the problem.

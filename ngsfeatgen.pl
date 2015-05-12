#!/usr/bin/perl -w
#===============================================================================
# Wrapper for the Binaries
#
#===============================================================================
use strict;
use Data::Dumper;
use Carp;
use File::Basename;
use Cwd;

my $codepath = "./src";

my $file      = $ARGV[0] || "small-len10-50.txt";
my $mm        =  1;

# This parameter only affect when mm = 2 
# This values means that RECOUNT will only consider those
# neighboring tags with error probability greater than 0.00262689
my $minBaseEr = 0.00262689; 


if (scalar @ARGV != 1) {
     die "perl recount.pl <preprocessed_input>\n";
}

my $base      = basename( $file, ".txt" );
my $dir       = dirname($file)."/";
my $nb_file   = $dir . $base . ".nb";
my $prop_file = $dir . $base . ".prop";
my $nbq_file  = $dir . $base . ".nbq";
my $capacity = 10; # For Knapsack

my $nb_file_cap   = $dir . $base . "_".$capacity. ".nb";
my $prop_file_cap = $dir . $base . "_" .$capacity. ".prop";
my $nbq_file_cap  = $dir . $base . "_".$capacity.".nbq";

my $cwd       = getcwd;
my $temp1 = $base."_temp1.txt";
my $temp2 = $base."_temp2.txt";
my $temp3 = $base."_temp3.txt";
my $temp4 = $base."_temp4.txt";
my $temp5 = $base."_temp5.txt";

#print STDERR "$dir $cwd\n";

my $fn    = $codepath ."/FindNeighboursWithQual";
my $gp    = $codepath ."/GenerateProportion";
my $em    = $codepath ."/EstimateTrueCount_llratio";
my $em_entro = $codepath. "/EstimateTrueCount_EntropyFast";
my $em_knap = $codepath. "/EstimateTrueCount_Capacity";


#print STDERR "FindNeighboursWithQual";
system("$fn $file $mm $minBaseEr");

#print STDERR "KnapsackNeighborsGenerator";
system("./knapsack_src/tryKnapsackEnumeratorMultiProbes $capacity $file");

#print STDERR "\nGenerateProportion";
system("$gp $file > $prop_file");
system("cp $prop_file $prop_file_cap");

print STDERR "\nComputing Predicted Count and LLRatio...\n";
system("$em $file > $temp1");


print STDERR "\nComputing Entropy...\n";
system("$em_entro $file 100 > $temp2");


print STDERR "\nComputing SCC...\n";
system("perl src/scc.pl $file > $temp3");
print "\n";

print STDERR "\nComputing ExpMatch ...\n";
system("perl ematch_src/run_Expmatch.pl $file | perl src/convert_first_col2atcg.pl > $temp4 ");

print STDERR "\nComputing Knapsack\n";
system("$em_knap $file $capacity > $temp5");

print STDERR "\nSummarizing ... \n\n";
print "\# Tag Observed_Count Predicted_Count LLRatio EntropyPj EntropyEstCount SCC ExpMatch Knapsak\n"; 
system("./src/summarize.sh $temp1 $temp2 $temp3 $temp4 $temp5");

unlink($nb_file);
unlink($prop_file);
unlink($nbq_file);
unlink($nb_file_cap);
unlink($nbq_file_cap);
unlink($prop_file_cap);
unlink($temp1);
unlink($temp2);
unlink($temp3);
unlink($temp4);
unlink($temp5);

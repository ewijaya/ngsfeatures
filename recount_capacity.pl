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

my $file      = $ARGV[0] || "test5tags.txt";
my $capacity        = $ARGV[1] || 30;
my $minBaseEr = 0;

# This parameter only affect when mm = 2 
# This values means that RECOUNT will only consider those
# neighboring tags with error probability greater than 0.00262689


if (scalar @ARGV != 2) {
     die "perl recount.pl <preprocessed_input> <capacity>\n";
}

my $base      = basename( $file, ".txt" );
my $dir       = dirname($file)."/";
my $nb_file   = $dir . $base . "_".$capacity. ".nb";
my $prop_file = $dir . $base . "_" .$capacity. ".prop";
my $nbq_file  = $dir . $base . "_".$capacity.".nbq";
my $cwd       = getcwd;
my $mm = 1;

my $fn    = $codepath ."/FindNeighboursWithQual";
my $gp    = $codepath ."/GenerateProportion";
my $em    = $codepath ."/EstimateTrueCount_Capacity";


#print STDERR "KnapsackNeighborsGenerator";
system("./knapsack_src/tryKnapsackEnumeratorMultiProbes $capacity $file");

#print STDERR "\nGenerateProportion";
system("$gp $file > $prop_file");

print STDERR "\nEstimateTrueCount\n";
system("$em $file $capacity");

print STDERR "\nDone\n";

unlink($nb_file);
unlink($prop_file);
unlink($nbq_file);

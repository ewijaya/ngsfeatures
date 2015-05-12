#!/usr/bin/perl -w
#===============================================================================
# Wrapper for the Binaries
# Running solely on Loglikelihood Ratio
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
my $cwd       = getcwd;

#print STDERR "$dir $cwd\n";

my $fn    = $codepath ."/FindNeighboursWithQual";
my $gp    = $codepath ."/GenerateProportion";
my $em    = $codepath ."/EstimateTrueCount_llratio";


#print STDERR "FindNeighboursWithQual";
system("$fn $file $mm $minBaseEr");

#print STDERR "\nGenerateProportion";
system("$gp $file > $prop_file");

#print STDERR "\nComputing Predicted Count and LLRatio...\n";
system("$em $file");
print "\n";

unlink($nb_file);
unlink($prop_file);
unlink($nbq_file);

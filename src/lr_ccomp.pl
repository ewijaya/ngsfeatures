#!/usr/bin/perl -w
#===============================================================================
#
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
my $ccomp_file  = $dir . $base . ".ccoc";
my $lr_temp     = $dir. $base.".temp";
my $cwd       = getcwd;



system("perl ematch_src/run_CreateCCOMP.pl $file > $ccomp_file");
system("perl ematch_src/create_avg_file_by_connected_component.pl $ccomp_file $file");

my @files = glob($base."*cc-*.txt");

foreach my $file (  @files ) {

    system("perl ./src/lr.pl $file >> $lr_temp");
    system("sort -k1 $lr_temp");
    unlink($file);

}               

unlink($ccomp_file);
unlink($lr_temp);

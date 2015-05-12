#!/usr/bin/perl -w
#===============================================================================
#
#
#===============================================================================
use strict;
use Data::Dumper;
use Carp;
use File::Basename;


my $input = $ARGV[0] || "../small-len10-50.txt";
my $base = basename($input,".txt");
my $dir       = dirname($input)."/";

my $nbq = $dir.$base.".nbq";
my $tgl = $dir.$base.".taglist";
my $binfile = $dir.$base.".bin";
my $rcountf = $dir.$base.".raw_count";


system("./ematch_src/FindNeighboursWithQualJuxt $input 1");
system("perl ./ematch_src/GenerateTagListFileWithBaseCount.pl $nbq > $tgl");
system("./ematch_src/writeRecountNeighborProbGraph $tgl $nbq $binfile");
system("./ematch_src/dumpRecountNeighborProbGraphOnDisk_ConnectedComponentSize $tgl $binfile");

unlink($nbq);
unlink($tgl);
unlink($binfile);
unlink($rcountf);

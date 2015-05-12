#!/usr/bin/perl -w
#===============================================================================
#  perl GenerateTagListFileWithBaseCount.pl 100k.nbq
#
#===============================================================================
use strict;
use Data::Dumper;
use Carp;

my $file = $ARGV[0] || "100k.nbq";


my %hash;
my $totbase =0;
my $noftag = 0;;


my	$INFILE_file_name = $file;		# input file name

open ( INFILE, '<', $INFILE_file_name )
	or croak "$0 : failed to open input file $INFILE_file_name : $!\n";



while ( <INFILE> ) {

    my $tag = (split(/\s+/,$_))[0];
    my $tlen = length($tag);
    $totbase += $tlen;
    $noftag++;
    $hash{$tag}=1;

}               


close ( INFILE );			# close input file




print "$noftag\t$totbase\n";

foreach my $tg ( sort keys %hash ) {
    print "$tg\n";

}


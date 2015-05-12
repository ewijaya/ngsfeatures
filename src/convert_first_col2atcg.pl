#!/usr/bin/perl -w
#===============================================================================
#
#
#===============================================================================
use strict;
use Data::Dumper;
use Carp;


while ( <> ) {
    chomp;
    my $line = $_;

    my ($tagnb,$count) = split(/\s+/,$line);
    $tagnb =~ tr/0123/ACGT/;
    print "$tagnb\t$count\n";

}  














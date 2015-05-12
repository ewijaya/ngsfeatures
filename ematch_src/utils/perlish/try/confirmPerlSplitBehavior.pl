#!/usr/bin/perl -w
#  Author: Paul B. Horton
#  Organization: Computational Biology Research Center, AIST, Japan
#  Copyright: Paul B. Horton 2007, All rights reserved.
#  Creation Date: 2007.4.7
#  Last Modification: $Date: 2007/05/12 07:04:26 $
#
#  Description: 
#
#  Purpose: (high level purpose or motivation)
#
use strict;

my $usage = "Usage: $0 pattern expression [limit]\n\nPipe string to spit from standard in\n";

die "$usage\n" if( grep {/-help/i} @ARGV );
die "$usage\n" if( grep {/-usage/i} @ARGV );

(@ARGV > 1) or die "Command line Error: not enough args\n\n$usage\n";
(@ARGV < 4) or die "Command line Error: too many args\n\n$usage\n";


my $pattern = shift  if( @ARGV );
my $expr    = shift  if( @ARGV );
my $limit   = shift  if( @ARGV );

my @field;
if( defined $limit ){
    @field = split( $pattern, $expr, $limit );
}
else{
    @field = split( $pattern, $expr );
}


for my $f (@field){
    print "<$f> ";
}
print "\n";


#!/usr/bin/perl -w
#===============================================================================
# Take connected component (.ccoc) file and full average quality with tag
# counts file then create multiple K files, where each file contain only 
# tags that belongs to the connected component of a tag under consideration.
# Let K be the size of the connected component we will have K files where each
# tag in that file 1 tag is clamped to zero
#===============================================================================
use strict;
use Data::Dumper;
use Carp;
use File::Basename;

my $ccomp_file    = $ARGV[0]; #|| "../test-data.ccoc";
my $full_avg_file = $ARGV[1]; #|| "../exampleData/test-data.txt";
my $base          = basename($full_avg_file,".txt");


#--------------------------------------------------
# Store all full_avg_file  
#-------------------------------------------------- 


my @all_full_avg;

my	$FULLAVGFILE_file_name = $full_avg_file;		# input file name

open ( FULLAVGFILE, '<', $FULLAVGFILE_file_name )
	or croak "$0 : failed to open input file $FULLAVGFILE_file_name : $!\n";



while ( <FULLAVGFILE> ) {
    chomp;
    push @all_full_avg, $_;


}               


close ( FULLAVGFILE );			# close input file



#--------------------------------------------------
#  Stores membership of all the connected components
#-------------------------------------------------- 
my %ccomp_with_tagids;

my	$INFILE_file_name = $ccomp_file;		# input file name

open ( INFILE, '<', $INFILE_file_name )
	or croak "$0 : failed to open input file $INFILE_file_name : $!\n";


while ( <INFILE> ) {
    next if (/^number/);
    my $line = $_;

    my @ccomps_ids = split(/\s+/,$line);


    my $tagid = -1;
    foreach my $ccomp_id ( @ccomps_ids  ) {
        $tagid++;
        push @{$ccomp_with_tagids{$ccomp_id}},$tagid;

    }               


}               

close ( INFILE );			# close input file



foreach my $ccomp ( sort keys %ccomp_with_tagids ) {

    my @tags_id_of_ccomp = @{ $ccomp_with_tagids{$ccomp} };
    my @members_of_ccomp = @all_full_avg[@tags_id_of_ccomp];


    my $nn = "./".$base."_cc-".$ccomp.".txt";
    #print "$nn\n";
    #print join(",",@tags_id_of_ccomp),"\n";;



     my	$OUTFILE_file_name = $nn;		# output file name

     open ( OUTFILE, '>', $OUTFILE_file_name )
         or croak "$0 : failed to open output file $OUTFILE_file_name : $!\n";

     print OUTFILE join("\n",@members_of_ccomp),"\n";

     close ( OUTFILE );			# close output file



}



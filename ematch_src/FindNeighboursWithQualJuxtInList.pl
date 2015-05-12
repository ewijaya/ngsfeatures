#!/usr/bin/perl -w
#===============================================================================
#
#
#===============================================================================
use strict;
use Data::Dumper;
use Carp;
use File::Basename;

my $file = $ARGV[0] || "test-data-small2.txt";

my $base = basename($file,".txt");
my $dirname = dirname($file);



my	$INFILE_file_name = $file;		# input file name

open ( INFILE, '<', $INFILE_file_name )
	or croak "$0 : failed to open input file $INFILE_file_name : $!\n";

my %taglist;

while ( <INFILE> ) {
    chomp;

    my @members = split(/\s+/,$_);
    my $raw_count = $members[0];
    my $tag       = $members[1];
    my @quals     = @members[2..$#members];

    $taglist{$tag} = $raw_count."/".join("/",@quals);


}               


close ( INFILE );			# close input file


my	$OUTFILE_RAWCOUNT_file_name = "./".$dirname."/".$base.".rawcountinlist";		# output file name

open ( OUTFILE_RAWCOUNT, '>', $OUTFILE_RAWCOUNT_file_name )
	or croak "$0 : failed to open output file $OUTFILE_RAWCOUNT_file_name : $!\n";


my	$OUTFILE_NBQ_file_name = "./".$dirname."/".$base.".nbqinlist";		# output file name

open ( OUTFILE_NBQ, '>', $OUTFILE_NBQ_file_name )
	or croak "$0 : failed to open output file $OUTFILE_NBQ_file_name : $!\n";



foreach my $tag ( sort keys %taglist  ) {
    my @vals = split("/",$taglist{$tag});
    my $rc = $vals[0];
    my @qls = @vals[1..$#vals];

    my @reads = split("",$tag);

    my @numreads = dna2num(@reads);
    print OUTFILE_NBQ join ("", @numreads), "\t";
    print OUTFILE_RAWCOUNT join ("", @numreads), "\t";
    print OUTFILE_RAWCOUNT  "$rc\n";


    # Generate neighbors
    my @tb = qw(C G T);

    foreach my $p ( 0 .. $#reads ) {

        foreach my $tbi ( 0 .. $#tb ) {

            my @nbreads = @reads;
            my $basetochange = $reads[$p];
            my $nbase = $tb[$tbi];

            if ( $basetochange eq $tb[$tbi] ) {
                $nbase = 'A';
            }

            $nbreads[$p] = $nbase;
            my @numnbreads = dna2num(@nbreads);
            
            my $dnanumstr = join("",@nbreads);

            my $nerr = Phred2ErrProb(Solexa2Phred($qls[$p]))/3;
            

            if ( $taglist{$dnanumstr} ) {
                print OUTFILE_NBQ join("", @numnbreads),"\t$nerr\t";

            }



        }

    }


print OUTFILE_NBQ "\n";

}               

close ( OUTFILE_RAWCOUNT );			# close output file
close ( OUTFILE_NBQ );			# close output file

sub dna2num {

    my @dnabases = @_;
    my %vals = ( "A" => 0, "C" => 1, "G" => 2, "T" => 3 );
    my @numdna = map { $vals{ $dnabases[$_] } } ( 0 .. $#dnabases );
    return @numdna;
}

sub num2dna {

    my @numbases = @_;
    my %vals = ( 0 => "A", 1 => "C", 2 => "G", 3 => "T" );
    my @dnabases = map { $vals{ $numbases[$_] } } ( 0 .. $#numbases );
    return @dnabases;
}

sub Solexa2Phred {

    my $sQ = shift;
    my $pQ = 10 * log( 1 + 10**( $sQ / 10 ) ) / log(10);
    return $pQ;
}

sub Phred2ErrProb {

    my $pQ   = shift;
    my $errp = 10**( -$pQ / 10 );

    return $errp;
}















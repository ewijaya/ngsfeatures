#!/usr/bin/perl -w
#===============================================================================
#
#
#===============================================================================
use strict;
use Data::Dumper;
use Carp;
use File::Basename;
my $initfile = $ARGV[0] || "small-len10-50.txt";

# Compute probability a being correct, individually

my %tag_quality;

my	$INFILEQ_file_name = $initfile;		# input file name

open ( INFILEQ, '<', $INFILEQ_file_name )
	or croak "$0 : failed to open input file $INFILEQ_file_name : $!\n";

while ( <INFILEQ> ) {
    chomp;
    my ($obscount, $tag,$solexaprob) = split(/\t/,$_);
    push @{$tag_quality{$tag}}, $solexaprob;
}               

close ( INFILEQ );			# close input file


my @sccs;

foreach my $tag ( sort keys %tag_quality  ) {

    my @quals = @{$tag_quality{$tag}};


    my $prod_of_onemin_pciprod = 1;
    foreach my $quals ( @quals ) {

        my $onemin_pciprod = multiply_quals($quals);
         $prod_of_onemin_pciprod *= $onemin_pciprod;

    }               

    my $scc = 1 - $prod_of_onemin_pciprod;

    print "$tag\t$scc\n";

}               



#--------------------------------------------------
# 
#-------------------------------------------------- 


sub multiply_quals {

	my	$qls	= shift;
    my @qls = split(/\s+/,$qls);

    my $prod = 1;
    foreach my $sp ( @qls ) {
        my $errprob = Phred2ErrProb(Solexa2Phred($sp));
        my $pci = 1-$errprob; # probability of no mismatch
        $prod *= $pci;

    }               
	
	return (1 - $prod);
}

sub Solexa2Phred {

    my      $sQ     = shift;
    my $pQ = 10 * log(1+ 10**($sQ/10))/log(10);
    return $pQ;
}



sub Phred2ErrProb {

    my      $pQ     = shift;
    my $errp = 10 ** (-$pQ/10);

    return $errp;
}


sub get_prediction_label {

	my	($presm,$tg,$ec)	= @_;

    my %presimhash = %{$presm};
    my $presence = $presimhash{$tg};
    
    my $label = 0;
	

    if ( $ec == 0 && !defined($presence) ) {
        $label = 1;
    }
    elsif ( $ec == 0 && defined($presence) ) {
         $label = 0;
     }
    elsif ( $ec > 0 && defined($presence) ) {
        $label = 1;
    }    

    #elsif ( $ec > 0 && !defined($presence) ) {
    #     $label = 0;
    #}
    
	return $label;
}





















/*  
 *  Author: Paul Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2009, Paul Horton, All rights reserved.
 *  Creation Date: 2009.5.6
 *  Last Modified: $Date: 2009/06/09 09:13:30 $
 *
 *  Description: Extension of FastaRecordReader which provides convenient methods
 *               for extracting sequences.
 *
 */
#ifndef FASTASEQSLURPER_HH_
#define FASTASEQSLURPER_HH_
#include <iostream>
#include "FastaRecordReader.hh"
#include "utils/sequence/LabeledAsciiSeq.hh"
#include "utils/sequence/LabeledSequence.hh"

namespace cbrc{

class FastaSeqSlurper : public FastaRecordReader{
public:
  /* ********** CONSTRUCTORS ********** */
  FastaSeqSlurper
  (
   /***/ std::istream&       defaultIstream  =  std::cin,
   const FastaInputOptions&  inputOptions    =  defaultFastaInputOptions
   ) :
    FastaRecordReader( defaultIstream, inputOptions )
  {}


  /* ********** METHODS ********** */
  std::vector<LabeledAsciiSeq> slurpSeqs(){
    return slurpSeqs( defaultIstream() );
  }

  std::vector<LabeledAsciiSeq> slurpSeqs( std::istream& iStream );

  
  void slurpLabeledAsciiSeqs( std::vector<LabeledAsciiSeq>& labSeqs,
			      std::istream& is ){
    while(  nextRecord( is )  )  labSeqs.push_back( getLabeledAsciiSeq() );
  }


  std::vector<LabeledSequence>
  slurpLabeledSequences(){
    return slurpLabeledSequences( defaultIstream() );
  }

  std::vector<LabeledSequence>
  slurpLabeledSequences( std::istream& is ){

    std::vector<LabeledSequence> labSeqs;

    while(  nextRecord( is )  )  labSeqs.push_back( getLabeledSequence() );

    return labSeqs;
  }


  std::vector<LabeledSequence>
  slurpLabeledSequences( std::istream& is,
			 const ResidueIndexMap& residueIndexMap ){

    std::vector<LabeledSequence> labSeqs;

    while(  nextRecord( is )  ){
      labSeqs.push_back( getLabeledSequence(residueIndexMap) );
    }

    return labSeqs;
  }


  /* ***** methods which push all remaining sequences in stream onto labSeqs ***** */
  void  slurpLabeledSequences( /***/ std::vector<LabeledSequence>& labSeqs ){
    slurpLabeledSequences( labSeqs, defaultIstream() );
  }

  void  slurpLabeledSequences( /***/ std::vector<LabeledSequence>& labSeqs,
			       const ResidueIndexMap&              residueIndexMap ){
    slurpLabeledSequences( labSeqs, defaultIstream(), residueIndexMap );
  }

  void slurpLabeledSequences( /***/ std::vector<LabeledSequence>& labSeqs,
			      /***/ std::istream& is ){
    while(  nextRecord( is )  )   labSeqs.push_back( getLabeledSequence() );
  }

  void slurpLabeledSequences( /***/ std::vector<LabeledSequence>& labSeqs,
			      /***/ std::istream&                 is,
			      const ResidueIndexMap&              residueIndexMap ){
    while(  nextRecord( is )  ){
      labSeqs.push_back( getLabeledSequence(residueIndexMap) );
    }
  }



  /* ****** Pseudo-Accessor To Current Record Data ***** */
  LabeledAsciiSeq getLabeledAsciiSeq() const{
    return LabeledAsciiSeq( curRecord().seq(), curRecord().labels() );
  }

  LabeledSequence getLabeledSequence() const{
    return LabeledSequence( curRecord().seq(), curRecord().labels() ); 
  }
    
  LabeledSequence getLabeledSequence( const ResidueIndexMap& residueIndexMap ) const{ 
    return LabeledSequence( curRecord().seq(), curRecord().labels(), residueIndexMap ); 
  }

private:
  // This extension of FastaRecordReader holds no object data
};

} // end namespace cbrc
#endif // FASTASEQSLURPER_HH_

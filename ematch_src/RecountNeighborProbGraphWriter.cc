/*
 *  Author: Paul Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2009, Paul Horton, All rights reserved.
 *  Creation Date: 2009.5.6
 *  Last Modified: $Date: 2009/09/24 01:44:06 $
 *  Description: See header file.
 */
#include <boost/lexical_cast.hpp>
#include "utils/perlish/perlish.hh"
#include "utils/stl/binaryIO.hh"
#include "utils/stl/stlUtils.hh"
#include "utils/gdb/gdbUtils.hh"
#include "recountTypes.hh"
#include "RecountNeighborProbGraphFormat.hh"
#include "RecountNeighborProbGraphWriter.hh"

namespace cbrc{

  size_t RecountNeighborProbGraphWriter::write(  std::ofstream& ofStream,
						 std::istream& tagNeighborlistIstream  ){

    
    // declare vectors to hold neighbor data
    std::vector<tagIdT>  neighborIds;
    std::vector<probT>   neighborProbs;
    RecountNeighborList  neighborList;

    size_t nodeCount  =  0; // number of neighborhood's written.

    // write signature
    binaryIO::writeContentsOnly( RecountNeighborProbGraphFormat::signature(), ofStream );

    // save ofstream pointer so we can go back and write it later
    const std::ofstream::pos_type posBeforeNodeCount = ofStream.tellp();

    // make room for writing nodeCount afterwards.
    ofStream.seekp( sizeof(nodeCount), std::ios_base::cur );


    for( ; ; ++nodeCount ){
    
      const std::string line = perlish::slurpLine( tagNeighborlistIstream );

      if( !line.size() )  break;  /** LOOP EXIT! **/


       GDB_ASSERTF(  !boost::regex_search( line, boost::regex("\t\t") ),
		     "Input error. Fields should be separted by exactly one tab character."\
		     " line:\n%s\n", line.c_str()  );


      const stringVecT  fields  =  perlish::split( "\t", line );


      GDB_ASSERTF(  fields.size() > 3,
		    "Expected at least 3 fields, but got %zu", fields.size()  );

      GDB_ASSERTF(  (fields.size() % 2),
		    "Expected an odd number of fields, but got %zu", fields.size()  );

      /* ***  Pop off tag sequence and count  *** */
      const std::string  tagSeq  =  fields[0];


      { // ***** Build vector of neighborIds and neighborProbs

	const neighborhoodSizeT  listBegIdx  =  1;   // neighbor list start index

	neighborIds  .resize ( 0 );
	neighborProbs.resize ( 0 );

	/* ***** Parse FIELDS into id and probability lists ***** */
	for(  size_t i = listBegIdx;  i < fields.size();
	      i++  /* also incremented in loop body */
	      ){

	  
	  const std::string  neighborSeq  =  fields[i];

	  GDB_ASSERTF(  ( neighborSeq != tagSeq ),
			"tagId:%s found in its own neighbor list", tagSeq.c_str()  );

	  const TagSet::unpackedSeqT  neighborUnpackedSeq
	    =   tagId_to_seq.toUnpackedSeq( fields[i] );
	  

	  // When tag present, this implementation does 2 binary searches.
	  // Could be optimized to 1.
	  if(  tagId_to_seq.has( neighborUnpackedSeq )  ){
	    const size_t neighborId  =  tagId_to_seq( neighborUnpackedSeq );
	    neighborIds  .push_back(  neighborId  );
	    neighborProbs.push_back(  boost::lexical_cast<probT> ( fields[++i] )  );
	  }
	  else{ // skip neighbor whose tag is not in the tag list
	    //	    std::cerr  <<  "skipping tag: "  <<  neighborSeq  <<  std::endl;
	    ++i;  // skip over unused probability field. 
	  }
	}
      } // END: Build vector of neighborIds and neighborProbs block.


      { // ***** Check validity of probs and add self to neighbor list

	probT sumOtherProbs = 0.0;
	BOOST_FOREACH(  const probT& prob,  neighborProbs  ){
	  GDB_ASSERTF(  ( (prob >= 0) && (prob < 0.5) ),
			"invalid probability value: %f", prob  );
	  sumOtherProbs += prob;
	}

	/*--------------------------------------------------
	* GDB_ASSERTF(  ( (sumOtherProbs >= 0) && (sumOtherProbs < 0.9) ),
	* 	      "When processing tag \"%s\", sumOtherProbs of non-self neighbors should be in [0,0.9] but was: %g",
	* 	      tagSeq.c_str(), sumOtherProbs  );
	*--------------------------------------------------*/

         if ( sumOtherProbs <= 1)  {
            // push self onto neighbor list
            neighborIds  .push_back(  tagId_to_seq.getSerialNumberOrDie( tagSeq )  );
            neighborProbs.push_back(  1.0 - sumOtherProbs  );
          }
      }
      

      /* ********** WRITE OUTPUT ********** */

      neighborList.set(  tagId_to_seq.getSerialNumberOrDie( tagSeq ),
			 neighborIds, neighborProbs  );


      neighborList.write( ofStream );
      

    } // end for slurpLine( tagNeighborlistIstream )

    // overwrite nodeCount at beginning of ofStream
    ofStream.seekp( posBeforeNodeCount, std::ios_base::beg );
    ofStream.write( (char*) &nodeCount, sizeof(nodeCount) );

    return nodeCount;
  }

} // end namespace cbrc


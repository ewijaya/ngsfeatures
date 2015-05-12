/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2008, Paul B. Horton, All rights reserved.
 *  Creation Date: 2008.11.3
 *  Last Modified: $Date: 2008/11/19 10:19:18 $
 *
 *  Description: Parser for reading SwissProt flat files
 *
 *  Purpose: Created for WoLF PSORT project.
 *
 */
#ifndef SPROTREADER_HH_
#define SPROTREADER_HH_
#include <iostream>

namespace cbrc{

class SprotReader{
public:
  /* ********** CONSTRUCTORS ********** */
  SprotReader(){
    clearRecord();
  }

  /* ********** READ RECORDS ********** */
  // returns false if no more records left in stream.
  bool readRecord( std::istream& is );

  /* ********** ACCESSORS ********** */
  const std::string& id() const        { return _id;        }
  const std::string& accession() const { return _accession; }
  const std::string& sequence()  const { return _sequence;  }
  const std::string& organismClassification() const{
    return _organismClassification;
  }

  const std::vector<std::string>& recordLines() const{
    return _recordLines;
  }

private:
  std::istream& instream() const{
    return *_istreamPtr;
  }

  const std::string& recordEndMarker() const{
    static const std::string _recordEndMarker( "//" );
    return _recordEndMarker;
  }

  bool curLineIsEndMarker() const{
    return(  curLinePrefix() == recordEndMarker() );
  }

  const std::string& curLine() const{
    return recordLines()[ recordLines().size()-1 ];
  }

  std::string curLinePrefix() const{
    return curLine().substr(0,2);
  }

  std::string curLineBody() const{
    return curLine().substr(6);
  }


  // read record line from instream() and push it onto end of recordLines
  void readLine();
  
  
  // clear data fields
  void clearRecord();

  /* ********** OBJECT DATA ********** */

  /* ***** Record Data ***** */
  std::string _id;
  std::string _accession;
  std::string _sequence;
  std::string _organismClassification;

  std::vector<std::string> _recordLines;

  // other data
  std::istream* _istreamPtr;

};


std::ostream& operator<<( std::ostream& os, const SprotReader& sprotReader );


} // end namespace cbrc
#endif // SPROTREADER_HH_

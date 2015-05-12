/*
 *  Author: Paul B. Horton
 *  
 *  Organization: Computational Biology Research Center, Japan
 *
 *  Copyright: All rights reserved
 *  
 *  Creation Date: 2004.5.18
 *
 *  Last Modified: $Date: 2009/08/25 21:12:17 $
 *  
 *  Purpose: Parse output of blast output format 8.
 *
 */

#include "utils/perlish/perlish.hh"
#include "utils/ChainHashtable/StringKeyHashtable.hh"

#ifndef _BLAST8RESULTREADER_HH_
#define _BLAST8RESULTREADER_HH_

namespace cbrc{

class Blast8Result{
public:
  Blast8Result( const std::vector<std::string> fields ) : fields(fields){}
  Blast8Result( const Blast8Result& br ){
    initFields( br.fields );
  }
  Blast8Result(){
    fields.clear();
  }
  void setFields( const std::vector<std::string>& f ){
    fields.clear();
    initFields( f );
  }
  // returns false if at end of stream.
  bool setFromStream( std::istream& is );
  std::string queryName() const{ return fields.at(0); }
  std::string hitName() const{ return fields.at(1); }
  size_t size() const{ return fields.size(); }
  double eValue() const{ return perlish::toDouble( fields.at(10) ); }
  bool operator<( const Blast8Result& br ) const{ return( eValue() < br.eValue() ); }
  static bool firstGreater( const Blast8Result& br1, const Blast8Result& br2 ){ return br1.eValue() > br2.eValue(); }
private:
  void initFields( const std::vector<std::string>& f ){
    fields.reserve( f.size() );
    std::copy( f.begin(), f.end(), std::back_inserter(fields) );
  }
  std::vector<std::string> fields;
};

inline std::ostream& operator<<( std::ostream& os, const Blast8Result& br ){
  os << br.queryName() << " " << br.hitName() << " " << br.eValue();
  return os;
}

class Blast8ResultReader{
public:
  Blast8ResultReader( std::istream& is, const bool ignoreSelfHit = true );
  size_t size() const{ return results.size(); }
  bool has( const std::string s ) const{ return queryLabels.has(s); }
  const std::vector<Blast8Result>& operator() ( const size_t i ) const { return results.at(i); }
  const std::vector<Blast8Result>& operator() ( const std::string id ) const {
    if( !queryLabels.has(id) ){
      std::cerr << __FILE__ << ":" << __LINE__ << " " << "no result for id: " << id << std::endl;
#ifdef GDB_DEBUG
      for(;;);
#endif // defined GDB_DEBUG
      exit( -1 );
    }
    return results.at( queryLabels(id) );
  }
  double maxEValue() const;
  void sortByEValue();
  bool sortedByEValue() const;
  void printNormalizedSimilarity( std::ostream& os ) const;
  void printNegLgEValues( std::ostream& os ) const;
  static bool isBlastCommentLine( const std::string& line );
  friend void testBlast8ResultReader();
  friend std::ostream& operator<<( std::ostream& os, const Blast8ResultReader& brr );
private:
  std::vector< std::vector<Blast8Result> > results;
  StringKeyHashtable<size_t> queryLabels;
};

std::ostream& operator<<( std::ostream& os, const Blast8ResultReader& brr );

}; // end namespace cbrc
#endif // defined _BLAST8RESULTREADER_HH_


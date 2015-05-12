/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2003,2008, Paul B. Horton, All rights reserved.
 *  Creation Date: 2003.5.1
 *  Last Modified: $Date: 2008/11/28 08:32:10 $
 *
 *  Description: Hashtable in which both key and value are std::string's
 *               Based on the ChainHashtable class implemented by Paul Horton in spring of 2003.
 *
 */

#ifndef _STRINGKEYHASHTABLE_HH_
#define _STRINGKEYHASHTABLE_HH_

#include <string>
#include <vector>
#include <iostream>
#include "utils/gdb/gdbUtils.hh"
#include "utils/ChainHashtable/bitLeftRotate.hh"
#include "nextPrime.hh"
#include "genericHashFunction.hh"

namespace cbrc{


// forward decls.
class StringHashtable;
class StringHashtableIterator;



/* ********** Helper Class to hold (key, value) pairs ********** */
class KeyValuePair{
public:
  /* ***** Constructors ***** */
  KeyValuePair( const std::string& key, const std::string& value )
  {
    _key = key;
    _value = value;
  }
  KeyValuePair( const KeyValuePair& snh )
    : _key( snh.key() ), _value( snh.value() )
  {}

  void operator= ( const std::string& val ){
    _value = val;
  }

  const std::string& operator()() const{  return _value;  }

  size_t hashValue() const{ return stringHashValue( _key ); }

  bool keyEquals( const KeyValuePair& snh ) const{
    return( key() == snh.key() );
  }

  const std::string& key()   const{  return _key;    }
  const std::string& value() const{  return _value;  }
  /***/ std::string& valueRef()   {  return _value;  }
  void setValue( const std::string& newValue ){  _value = newValue;  }
  friend class StringHashtable;
private:
  // object data
  //  const std::string _key;
  std::string _key;
  /***/ std::string _value;
}; // end class KeyValuePair.


// Private helper class.
class StringHashtableLink{
public:
  friend class StringHashtable;
  friend class KeyValuePair;
  friend class StringKeyHashIterator;
  StringHashtableLink( const StringHashtableLink& link )
    : _elem( link._elem ),
      _next( NULL )
  {}
  StringHashtableLink( const KeyValuePair& elem )
    : _elem(elem),
      _next( NULL )
  {}
  StringHashtableLink( const std::string& key, const std::string& value )
    : _elem( key, value ),
      _next( NULL )
  {}

  ~StringHashtableLink(){}  // no memory needs to be freed.

  StringHashtableLink* const next()   const       {  return _next;      }
  void setNext( StringHashtableLink* const& next ){ this->_next = next; }

  // private:  /* ACHTUNG temporarily commented out! */
  const std::string& key()   const {  return elem().key(); }
  const std::string& value() const {  return elem().value(); }
  
  /***/ std::string& valueRef() {  return _elem.valueRef();  }

  const KeyValuePair& elem()     const{  return  _elem;  }
  /***/ KeyValuePair* elemPtr()       {  return &_elem;  }  

  // object data
  KeyValuePair         _elem;
  StringHashtableLink* _next;
}; // end StringHashtableLink


// Auxillary class for StringHashtable
// This iterator has three state variables. All pointers.
// One is head. An iterator with a null head <==> an invalid iterator,
// This is used for the end() of hashtables and the begin() of empty hashtables.
class StringHashtableIterator{
public:
  StringHashtableIterator(){
    invalidate(); // default constructor initializes to hashtable.end()
  } 
  StringHashtableIterator( StringHashtableLink** head, StringHashtableLink** last ){
    this->head = head; this->last = last;
    if( head ) linkPtr = *head;
  }
  void invalidate(){ head = NULL; }
  bool valid() const{ return head; }

  static StringHashtableIterator makeInvalidIterator(){
    return StringHashtableIterator();
  }

  bool operator==( const StringHashtableIterator& ch ) const{
    if( head != ch.head ) return false;
    if( !valid() ) return true; // only head needs to be compared for invalid iterators.
    return( linkPtr == ch.linkPtr );
  }

  bool operator!=( const StringHashtableIterator& ch ) const{
    return( !(*this==ch) );
  }

  KeyValuePair* elemPtr() const{  return getLinkPtr()->elemPtr();  }
  KeyValuePair& operator*()  const{ return *(elemPtr()); }

  StringHashtableIterator& operator++(){

    if( !valid() ){
      return *this;  // ++ing an invalid iterator is a no-op, not an error.
    }

    if( linkPtr->next() ){  // walk down one step.
      linkPtr = linkPtr->next();
      return *this;
    }

    if( head >= last ){
      invalidate();
      return *this;
    }

    for( ++head; (head<last) && !*head; ++head );
    if( *head )  linkPtr = *head;
    else         invalidate();

    return *this;
  }

  // object data.
  StringHashtableLink* getLinkPtr() const{  return linkPtr;  }
  StringHashtableLink** head;
  StringHashtableLink* linkPtr;
  StringHashtableLink** last;
};


// Main class.
class StringHashtable{
public:
  /* ********** TYPEDEFS ********** */
  typedef  enum {binary, text}                 fileFormatT;


  /* ********** CONSTRUCTORS ********** */
StringHashtable( const size_t size = 11 )
  : _end()
  {
    tsize = nextPrime( size );
    t = new StringHashtableLink*[tsize];
    memset( t, 0, tsize*sizeof(StringHashtableLink*) );
    numElems = 0;
  }
  StringHashtable( const StringHashtable& ht ){
    copy( ht );
  }
  StringHashtable operator=( const StringHashtable& ht ){
    copy( ht );
    return *this;
  }

  void copy( const StringHashtable& ht ){
    RUNTIME_WARN( "calling copy constructor for StringKeyhashtable\n" );
    tsize = ht.tsize;
    t = new StringHashtableLink*[tsize];
    for( size_t i = 0; i < tsize; ++i ){
      if( !ht.t[i] ){
	t[i] = 0;
	continue;
      }

      t[i] = new StringHashtableLink( ht.t[i]->elem().key(),
				      ht.t[i]->elem().value() );

      StringHashtableLink* prevHT = ht.t[i];
      StringHashtableLink* prev = t[i];
      while( StringHashtableLink* curHT = prevHT->next() ){
	prev->_next = new StringHashtableLink( curHT->elem() );
	prev = prev->next();
	prevHT = curHT;
      }
    }
  }

  ~StringHashtable(){ clear(); delete t; }

  /* ***** Class Constants ***** */
  static const std::string& notFoundValue(){
    static const std::string _notFoundValue( "" );
    return _notFoundValue;
  }

  static const std::string& defaultInsertionValue(){
    static const std::string _defaultInsertionValue( "?" );
    return _defaultInsertionValue;
  }

  // might be better for this to be set adaptively, by probing
  // for available memory size.
  static size_t maxAllowedLinkListLength(){  return( 1796359771 );  }

  void clear(){
    for( size_t i = 0; i < tsize; ++i ){
      if( !t[i] ) continue;
      StringHashtableLink* prev = t[i];
      while( StringHashtableLink* next = prev->next() ){
	delete prev;
	prev = next;
      }
      t[i] = NULL;
      delete prev;
    }
    numElems = 0;
  }
  size_t size() const{ return numElems; }

  StringHashtableIterator begin() const{
    if( !size() ){
      return StringHashtableIterator::makeInvalidIterator();
    }
    StringHashtableLink** headPtr;
    for( headPtr = t; !*headPtr; ++headPtr ){
      GDB_ASSERTF(  static_cast<size_t>(headPtr - t)  <  tsize   );
    }
    return StringHashtableIterator( headPtr, t+tsize-1 );
  }

  const StringHashtableIterator& end() const{ return _end; }

  std::vector< std::pair<std::string, std::string> >
  contentsAsVector() const{ // returns contents in arbitray order.
    std::vector< std::pair<std::string, std::string> > retVal;
    retVal.reserve( size() );
    for( StringHashtableIterator it = begin(); it != end(); ++it ){
      retVal.push_back( std::make_pair( (*it).key(), (*it).value() ) );
    }
    return retVal;
  }

  bool has( const std::string& key ) const{
    const size_t index = stringHashValue( key ) % tsize;
    StringHashtableLink* linkPtr = t[index];
    if( !linkPtr ) return false;

    for( ; /* no initialization */
	 !(key == linkPtr->elem().key()) && linkPtr->next();
	 linkPtr = linkPtr->next() );

    return(  key == linkPtr->elem().key()  );
  }

  const std::string& operator() ( const std::string& key ) const{
    size_t index = stringHashValue( key ) % tsize;
    StringHashtableLink* linkPtr = t[index];

    if( !linkPtr )  return notFoundValue();

    for( ; /* no initialization */
	 !(key == linkPtr->elem().key()) && linkPtr->next();
	 linkPtr=linkPtr->next() );

    if(  key == linkPtr->elem().key()  ){
      return linkPtr->elem().value();
    } // else
    return notFoundValue();
  }

  std::string& operator[] ( const std::string& key ){
    return insertKeyIfNew_returnValue( key );
  }
  std::string& insertKeyIfNew_returnValue( const std::string& key,
					   const bool resizeAllowed = true ){
    const unsigned int StringHashtableMaxChainLen = 8;
    const size_t index = stringHashValue( key ) % tsize;
    unsigned int chainLen;
    StringHashtableLink* linkPtr = t[index];

    if( !linkPtr ){
      /** add to head of list at position index **/
      t[index] = new StringHashtableLink( key, defaultInsertionValue() );
      ++numElems;
      return( t[index]->valueRef() );
    }

    // head of list was not null, so walk down linked list
    for( chainLen = 2;
	 !(key==linkPtr->elem().key()) && linkPtr->next();
	 linkPtr=linkPtr->next(), chainLen++
	 );

    if( key == linkPtr->elem().key() ){
      return linkPtr->valueRef();  // EXIT. element already in table
    }
    // need to allocate new slot, but first check if we need to resize.
    if( /**/ resizeAllowed
	&& (tsize < maxAllowedLinkListLength())
	&& (chainLen > StringHashtableMaxChainLen) ){
      resizeLinkedList( tsize+1 );
      return insertKeyIfNew_returnValue( key, false );
    }
    ++numElems;
    linkPtr->setNext(  new StringHashtableLink( key, defaultInsertionValue() )  );
    return linkPtr->valueRef();
  } // end insertKeyIfNew_returnValue

  KeyValuePair* add( const KeyValuePair& elem, const bool resizeAllowed = true ){
    const unsigned int StringHashtableMaxChainLen = 8;
    const size_t index = elem.hashValue() % tsize;
    unsigned int chainLen;
    StringHashtableLink* linkPtr = t[index];

    if( !linkPtr ){
      /** add to head of list at position index. **/
      t[index] = new StringHashtableLink(elem);
      ++numElems;
      return NULL;  // elem was not found in the table so return null as per the API.
    }

    // head of list was not null so walk down\n";
    for( chainLen = 2;
	 !elem.keyEquals( linkPtr->elem() ) && linkPtr->next();
	 linkPtr=linkPtr->next(), chainLen++ );

    if( elem.keyEquals( linkPtr->elem()) ){
      // std::cout << "element already in table\n";
      return linkPtr->elemPtr();
    }
    // need to allocate new slot, but first check if we need to resize.
    if( /**/ resizeAllowed
	&&   (tsize < maxAllowedLinkListLength())
	&&   (chainLen > StringHashtableMaxChainLen) ){
      resizeLinkedList( tsize+1 );
      return add( elem, false );
    }
    ++numElems;
    linkPtr->setNext( new StringHashtableLink(elem) );
    return NULL;  // elem was not found in the table so return null as per the API.
  } // end method add( const KeyValuePair& elem, const bool... )

  // for debugging.
  void printTableState( std::ostream& os );
private:
  // used by resize so can assume oldElemPtr is not in new table.
  void addNewToTablePtr( StringHashtableLink* oldLinkPtr, 
			 StringHashtableLink** newT,
			 unsigned int newSize ){
    size_t index = oldLinkPtr->elem().hashValue() % newSize;
    StringHashtableLink* linkPtr = newT[index];
    oldLinkPtr->setNext( NULL ); // adding to tail of chain.
    if( !linkPtr ){
      newT[index] = oldLinkPtr;
      return;
    }
    for( /* noop */; linkPtr->next(); linkPtr=linkPtr->next() );
    linkPtr->setNext( oldLinkPtr );
  }

  void resizeLinkedList( const size_t& minNewSize );

  StringHashtableLink* lastInChain( StringHashtableLink* head ) const{
    if( !head ) return head;
    for( /* noop */; head->next(); head = head->next() );
    return head;
  }

  // object data
  StringHashtableLink** t; // hashtable
  size_t tsize; //table size
  const std::string _notFoundValue;
  unsigned int numElems;
  const StringHashtableIterator _end;
}; // end class StringHashtable.


/* ********** METHOD IMPLEMENTATIONS ********** */
inline void StringHashtable::resizeLinkedList( const size_t& minNewSize ){
    const size_t newSize = nextPrime( minNewSize );
    StringHashtableLink** newT = new StringHashtableLink*[newSize];
    memset( newT, 0, sizeof(StringHashtableLink*)*newSize );
    StringHashtableIterator curIt = begin();
    StringHashtableIterator nextIt = curIt;
    GDB_ASSERTF( curIt.valid() );
    GDB_ASSERTF( nextIt.valid() );
    for( ++nextIt;
	 curIt.valid();
	 curIt = nextIt, ++nextIt ){ // curIt state gets modified so can't ++curIt.
      addNewToTablePtr( curIt.getLinkPtr(), newT, newSize );
    }
    StringHashtableLink** temp = t;
    t = newT;
    tsize = newSize;
    delete temp;
}

inline void StringHashtable::printTableState( std::ostream& os ){
  for( size_t i = 0; i < tsize; ++i ){

    StringHashtableLink* linkPtr = t[i];

    if( !linkPtr ){
      os << " --> NULL\n";
      continue;
    }

    // else print each node in linked list.
    do{
      os << " --> " 
	 << " ( " << linkPtr->key() << ", " << linkPtr->value() << " ) ";
      linkPtr = linkPtr->next();
    } while( linkPtr );

    os << " --> NULL\n";
  }
}


/* ********** IMPLEMENTATION OF RELATED FUNCTIONS ********** */
inline std::ostream& operator<<( std::ostream& os, const StringHashtableIterator& skh ){
  std::cout << " head: " << skh.head << std::endl;
  std::cout << " tail: " << skh.last << std::endl;
  return os;
}


inline std::ostream& operator<<( std::ostream& os, const StringHashtable& skh ){
  for( StringHashtableIterator it = skh.begin(); it != skh.end(); ++it ){
    os << (*it).key();
    os << " " << (*it).value();
    os << std::endl;
  }
  return os;
}

  

}; // end namespace cbrc

#endif // defined _STRINGKEYHASHTABLE_HH_

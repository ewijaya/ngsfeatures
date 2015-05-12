/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2003,2008, Paul B. Horton, All rights reserved.
 *  Creation Date: 2003.5.1
 *  Last Modified: $Date: 2008/11/23 00:17:03 $
 *
 *  Description: Hashtable for hashes using a string for the key.
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
template<typename ValT>class StringKeyHashtable;
template<typename ValT>class StringKeyHashIterator;

template<typename ValT>
class StringKeyAndValue{
public:
  //  friend class StringKeyHashtableLink;
  StringKeyAndValue( const std::string& key, const ValT value ){ keyVar = key; valueVar = value; }
  StringKeyAndValue( const StringKeyAndValue<ValT>& snh ){ keyVar = snh.keyVar; valueVar = snh.valueVar; }
  void operator= ( const ValT& val ){ valueVar = val; }
  operator ValT&(){ return valueVar; }
  size_t hashVal() const{ return stringHashValue( keyVar ); }
  bool keyEquals( const StringKeyAndValue<ValT>& snh ) const{ return keyVar == snh.keyVar; }
  std::string key() const{ return keyVar; }
  ValT value() const{ return valueVar; }
  void setValue( const ValT& newVal ){ valueVar = newVal; }
  friend class StringKeyHashtable<ValT>;
  friend void testStringKeyAndValue();
private:
  std::string keyVar;
  ValT valueVar;
};


// Private helper class.
template<typename ValT>
class StringKeyHashtableLink{
public:
  friend class StringKeyHashtable<ValT>;
  friend class StringKeyAndValue<ValT>;
  friend class StringKeyHashIterator<ValT>;
  StringKeyHashtableLink<ValT>( const StringKeyHashtableLink<ValT>& link ){
    elemPtr = new StringKeyAndValue<ValT>( *(link.getElemPtr()) );
    next = NULL;
  }
  StringKeyHashtableLink<ValT>( const StringKeyAndValue<ValT>& elem ){
    elemPtr = new StringKeyAndValue<ValT>(elem);
    next = NULL;
  }
  StringKeyHashtableLink<ValT>( const std::string s, const ValT& val ){
    elemPtr = new StringKeyAndValue<ValT>( s, val );
    next = NULL;
  }
  StringKeyHashtableLink<ValT>* getNext() const{ return next; }
  ~StringKeyHashtableLink(){ delete elemPtr; }
  void setNext( StringKeyHashtableLink<ValT>* next ){ this->next = next; }
  bool valid(){
    static int dummy;
    if( !elemPtr ) return false;
    dummy = elemPtr->hashVal();
    if( next ){
      if( next->elemPtr ) ++dummy;
      if( next->next ) ++dummy;
    }
    return true;
  }
private:
  StringKeyAndValue<ValT>* getElemPtr() const{ return elemPtr; }
  StringKeyAndValue<ValT>* elemPtr;
  StringKeyHashtableLink<ValT>* next;
};


// Auxillary class for StringKeyHashtable
// This iterator has three state variables. All pointers.
// One is head. An iterator with a null head <==> an invalid iterator,
// This is used for the end() of hashtables and the begin() of empty hashtables.
template<typename ValT>
class StringKeyHashIterator{
public:
  friend class StringKeyHashtable<ValT>;
  StringKeyHashIterator<ValT>(){ invalidate(); } // default constructor initializes to hashtable.end()
  StringKeyHashIterator<ValT>( StringKeyHashtableLink<ValT>** head, StringKeyHashtableLink<ValT>** last ){
    this->head = head; this->last = last;
    if( head ) linkPtr = *head;
  }
  void invalidate(){ head = NULL; }
  bool valid() const{ return head; }
  static StringKeyHashIterator<ValT> makeInvalidIterator(){
    return StringKeyHashIterator<ValT>();
  }
  bool operator==( const StringKeyHashIterator<ValT>& ch ) const{
    if( head != ch.head ) return false;
    if( !valid() ) return true; // only head needs to be compared for invalid iterators.
    return( linkPtr == ch.linkPtr );
  }
  bool operator!=( const StringKeyHashIterator<ValT>& ch ) const{ return( !(*this==ch) ); }
  StringKeyAndValue<ValT> * getElemPtr() const{ return getLinkPtr()->getElemPtr(); }
  StringKeyAndValue<ValT>& operator*() const{ return *(getElemPtr()); }
  StringKeyHashIterator<ValT>& operator++(){

    if( !valid() ){
      return *this;  // ++ing an invalid iterator is a no-op not an error.
    }

    if( linkPtr->getNext() ){  // walk down one step.
      linkPtr = linkPtr->getNext();
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
  //  friend std::ostream& operator<<<ValT>( std::ostream& os, const StringKeyHashIterator<ValT>& skh );
private:
  StringKeyHashtableLink<ValT>* getLinkPtr() const{ return linkPtr; }
  StringKeyHashtableLink<ValT>** head;
  StringKeyHashtableLink<ValT>* linkPtr;
  StringKeyHashtableLink<ValT>** last;
};


// Old saint Nick himself
template<typename ValT>
class StringKeyHashtable{
public:
  /* ********** CONSTRUCTORS ********** */
  StringKeyHashtable<ValT>( const ValT defaultValue = 1,
			    const size_t size = 11,
			    const size_t maxTSize = 1796359771 )
  : defaultValue(defaultValue),
    maxTSize(maxTSize),
    endVar()
  {
    tsize = nextPrime( size );
    t = new StringKeyHashtableLink<ValT>*[tsize];
    memset( t, 0, tsize*sizeof(StringKeyHashtableLink<ValT>*) );
    numElems = 0;
  }
  StringKeyHashtable<ValT>( const StringKeyHashtable<ValT>& ht ){
    copy( ht );
  }
  StringKeyHashtable<ValT> operator=( const StringKeyHashtable<ValT>& ht ){
    copy( ht );
    return *this;
  }
  void copy( const StringKeyHashtable<ValT>& ht ){ // Achtung! Deep copy not tested!
    RUNTIME_WARN( "calling copy constructor for StringKeyhashtable\n" );
    tsize = ht.tsize;
    t = new StringKeyHashtableLink<ValT>*[tsize];
    for( size_t i = 0; i < tsize; ++i ){
      if( !ht.t[i] ){
	t[i] = 0;
	continue;
      }
      t[i] = new StringKeyHashtableLink<ValT>( *(ht.t[i]) );
      StringKeyHashtableLink<ValT>* prevHT = ht.t[i];
      StringKeyHashtableLink<ValT>* prev = t[i];
      while( StringKeyHashtableLink<ValT>* curHT = prevHT->getNext() ){
	prev->next = new StringKeyHashtableLink<ValT>( *curHT );
	prev = prev->getNext();
	prevHT = curHT;
      }
    }
  }
  ~StringKeyHashtable(){ clear(); delete t; }
  void setNotFoundValue( const ValT val ){ notFoundValue = val; }
  void clear(){
    for( size_t i = 0; i < tsize; ++i ){
      if( !t[i] ) continue;
      StringKeyHashtableLink<ValT>* prev = t[i];
      while( StringKeyHashtableLink<ValT>* next = prev->getNext() ){
	delete prev;
	prev = next;
      }
      t[i] = NULL;
      delete prev;
    }
    numElems = 0;
  }
  size_t size() const{ return numElems; }
  StringKeyHashIterator<ValT> begin() const{
    if( !size() )  return StringKeyHashIterator<ValT>::makeInvalidIterator();
    StringKeyHashtableLink<ValT>** headPtr;
    for( headPtr = t; !*headPtr; ++headPtr ){
      GDB_ASSERTF(  static_cast<size_t>(headPtr - t)  <  tsize   );
    }
    return StringKeyHashIterator<ValT>( headPtr, t+tsize-1 );
  }
  const StringKeyHashIterator<ValT>& end() const{ return endVar; }
  std::vector< std::pair<std::string, ValT> > contentsAsVector() const{ // returns contents in arbitray order.
    std::vector< std::pair<std::string, ValT> > retVal;
    retVal.reserve( size() );
    for( StringKeyHashIterator<ValT> it = begin(); it != end(); ++it ){
      retVal.push_back( std::make_pair( (*it).key(), (*it).value() ) );
    }
    return retVal;
  }
  bool has( const std::string s ) const{
    size_t index = stringHashValue( s ) % tsize;
    StringKeyHashtableLink<ValT>* linkPtr = t[index];
    if( !linkPtr ) return false;
    for( ; !(s == linkPtr->getElemPtr()->key()) && linkPtr->getNext(); linkPtr=linkPtr->getNext() );
    if( s == linkPtr->getElemPtr()->key() )  return true;
    return false;
  }
  ValT operator() ( const std::string s ) const{
    size_t index = stringHashValue( s ) % tsize;
    StringKeyHashtableLink<ValT>* linkPtr = t[index];
    if( !linkPtr ) return notFoundValue;
    for( ; !(s == linkPtr->getElemPtr()->key()) && linkPtr->getNext(); linkPtr=linkPtr->getNext() );
    if( s == linkPtr->getElemPtr()->key() )  return linkPtr->getElemPtr()->value();
    return notFoundValue;
  }
  ValT& operator[] ( const std::string s ){
    size_t index = stringHashValue( s ) % tsize;
    //    std::cout << "s: " << s << " index: " << index << std::endl;
    StringKeyHashtableLink<ValT>* linkPtr = t[index];
    if( !linkPtr ){ return add2( s ); }
    for( ; !(s == linkPtr->getElemPtr()->key()) && linkPtr->getNext(); linkPtr=linkPtr->getNext() );
    if( s == linkPtr->getElemPtr()->key() )  return linkPtr->getElemPtr()->valueVar;
    return add2( s );
  }
  StringKeyAndValue<ValT>* find( const StringKeyAndValue<ValT> & elem ){ // adds elem if necessary.
    size_t index = elem.hashVal() % tsize;
    StringKeyHashtableLink<ValT>* linkPtr = t[index];
    if( !linkPtr ){
      t[index] = new StringKeyHashtableLink<ValT>(elem);
      return (t[index]->getElemPtr());
    }
    for( ; !(elem.keyEquals(*linkPtr->getElemPtr())) && linkPtr->getNext(); linkPtr=linkPtr->getNext() );
    if( elem.keyEquals(*linkPtr->getElemPtr()) )  return linkPtr->getElemPtr();
    return NULL;
  }
  ValT& add2( const std::string& s, bool resizeAllowed = true ){
    GDB_ASSERTF(  tableStateConsistent( tsize, t )  );
    const unsigned int StringKeyHashtableMaxChainLen = 8;
    size_t index = stringHashValue( s ) % tsize;
    unsigned int chainLen;
    StringKeyHashtableLink<ValT>* linkPtr = t[index];
    if( !linkPtr ){
      // std::cout << "##add to head of list at position " << index << std::endl;
      t[index] = new StringKeyHashtableLink<ValT>( s, defaultValue );
      ++numElems;
      GDB_ASSERTF(  tableStateConsistent( tsize, t )  );
      return *(t[index]->getElemPtr());
    }
    // std::cout << "##head of list was not null so walk down\n";
    for( chainLen = 2; !(s==linkPtr->elemPtr->key()) && linkPtr->getNext(); linkPtr=linkPtr->getNext(), 
	   chainLen++ );
    if( s == linkPtr->elemPtr->key() ){
      // std::cout << "element already in table\n";
      return *(linkPtr->getElemPtr());
    }
    // need to allocate new slot, but first check if we need to resize.
    if( resizeAllowed && (tsize < maxTSize) && (chainLen > StringKeyHashtableMaxChainLen) ){
      GDB_ASSERTF(  tableStateConsistent( tsize, t )  );
      resize( tsize+1 );
      GDB_ASSERTF(  tableStateConsistent( tsize, t )  );
      return add2( s, false );
    }
    ++numElems;
    linkPtr->setNext(  new StringKeyHashtableLink<ValT>( s, defaultValue )  );
    GDB_ASSERTF(  linkPtr->valid()  );
    GDB_ASSERTF(  tableStateConsistent( tsize, t )  );
    return *(linkPtr->next->getElemPtr());
  }
  StringKeyAndValue<ValT> * add( const  StringKeyAndValue<ValT> & elem, bool resizeAllowed = true ){
    GDB_ASSERTF(  tableStateConsistent( tsize, t )  );
    const unsigned int StringKeyHashtableMaxChainLen = 8;
    size_t index = elem.hashVal() % tsize;
    unsigned int chainLen;
    StringKeyHashtableLink<ValT>* linkPtr = t[index];
    if( !linkPtr ){
      // std::cout << "##add to head of list at position " << index << std::endl;
      t[index] = new StringKeyHashtableLink<ValT>(elem);
      ++numElems;
      GDB_ASSERTF(  tableStateConsistent( tsize, t )  );
      return NULL;  // elem was not found in the table so return null as per the API.
    }
    // std::cout << "##head of list was not null so walk down\n";
    for( chainLen = 2; !elem.keyEquals(*(linkPtr->elemPtr)) && linkPtr->getNext(); linkPtr=linkPtr->getNext(), 
	   chainLen++ );
    if( elem.keyEquals(*linkPtr->elemPtr) ){
      // std::cout << "element already in table\n";
      return linkPtr->getElemPtr();
    }
    // need to allocate new slot, but first check if we need to resize.
    if( resizeAllowed && (tsize < maxTSize) && (chainLen > StringKeyHashtableMaxChainLen) ){
      GDB_ASSERTF(  tableStateConsistent( tsize, t )  );
      resize( tsize+1 );
      GDB_ASSERTF(  tableStateConsistent( tsize, t )  );
      return add( elem, false );
    }
    ++numElems;
    linkPtr->setNext( new StringKeyHashtableLink<ValT>(elem) );
    GDB_ASSERTF(  linkPtr->valid()  );
    GDB_ASSERTF(  tableStateConsistent( tsize, t )  );
    return NULL;  // elem was not found in the table so return null as per the API.
  }
private:
  bool tableStateConsistent( size_t tableSize, StringKeyHashtableLink<ValT>** tablePtr ){
    if( tablePtr == t ){
      if( tableSize != tsize ){
	std::cout << "tableSize: " << tableSize << " not equal to tsize: " << tsize << "\n";
	return false;
      }
    }
    for( size_t i = 0; i < tsize; i++ ){
      if( !t[i] ) continue; // NULL's okay
      StringKeyHashtableLink<ValT>* linkPtr = t[i];
      if( !linkPtr->valid() ){
	return false;
      }
      for( ; linkPtr->getNext(); linkPtr = linkPtr->getNext() ){
	if( !linkPtr->valid() ){
	  return false;
	}
      }
    }
    return true;
  }
  // used by resize so can assume oldElemPtr is not in new table.
  void addNewToTablePtr( StringKeyHashtableLink<ValT>* oldLinkPtr, 
			 StringKeyHashtableLink<ValT>** newT, unsigned int newSize ){
    size_t index = oldLinkPtr->elemPtr->hashVal() % newSize;
    StringKeyHashtableLink<ValT>* linkPtr = newT[index];
    oldLinkPtr->setNext( NULL ); // adding to tail of chain.
    if( !linkPtr ){
      newT[index] = oldLinkPtr;
      GDB_ASSERTF(  tableStateConsistent( newSize, newT )  );
      return;
    }
    for( ;linkPtr->getNext(); linkPtr=linkPtr->getNext() );
    linkPtr->setNext( oldLinkPtr );
  }
  void resize( size_t size ){
    size_t newSize = nextPrime( size );
    StringKeyHashtableLink<ValT>** newT = new StringKeyHashtableLink<ValT>*[newSize];
    memset( newT, 0, sizeof(StringKeyHashtableLink<ValT>*)*newSize );
    StringKeyHashIterator<ValT> curIt = begin();
    StringKeyHashIterator<ValT> nextIt = curIt;
    GDB_ASSERTF( curIt.valid() );
    GDB_ASSERTF( nextIt.valid() ); // should be alright if table size > 2.
    for( ++nextIt; curIt.valid(); curIt = nextIt, ++nextIt ){ // curIt state gets modified so can't ++curIt.
      addNewToTablePtr( curIt.getLinkPtr(), newT, newSize );
      GDB_ASSERTF(  tableStateConsistent( tsize, t )  );
      GDB_ASSERTF(  tableStateConsistent( newSize, newT )  );
    }
    StringKeyHashtableLink<ValT>** temp = t;
    t = newT;
    tsize = newSize;
    delete temp;
  }
  StringKeyHashtableLink<ValT>* lastInChain( StringKeyHashtableLink<ValT>* head ) const{
    if( !head ) return head;
    for( ;head->getNext(); head = head->getNext() );
    return head;
  }
  StringKeyHashtableLink<ValT>** t; // hashtable
  ValT defaultValue;
  size_t maxTSize;
  size_t tsize; //table size
  unsigned int numElems;
  ValT notFoundValue;
  const StringKeyHashIterator<ValT> endVar;
};



template <typename ValT>
std::ostream& operator<<( std::ostream& os, const StringKeyHashIterator<ValT>& skh ){
  std::cout << " head: " << skh.head << std::endl;
  std::cout << " tail: " << skh.last << std::endl;
  return os;
}

template <typename ValT>
std::ostream& operator<<( std::ostream& os, const StringKeyHashtable<ValT>& skh ){
  for( StringKeyHashIterator<ValT> it = skh.begin(); it != skh.end(); ++it ){
    os << (*it).key();
    os << " " << (*it).value();
    os << std::endl;
  }
  return os;
}

  

}; // end namespace cbrc

#endif // defined _STRINGKEYHASHTABLE_HH_

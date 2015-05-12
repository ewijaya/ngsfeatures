/*
 *  Author: Paul B. Horton
 *  
 *  Organization: Computational Biology Research Center, Japan
 *
 *  Copyright: All rights reserved
 *  
 *  Creation Date: 2003.5.1
 *  
 *  Last Modified: 2003.7.16
 *  
 *  Description: Hashtable for hashes using a pair of strings for the key. Based on the ChainHashtable class
 *  implemented by Paul Horton in spring of 2003.
 *
 */

#ifndef _STRING2KEYHASHTABLE_HH_
#define _STRING2KEYHASHTABLE_HH_

#include <string>
#include <vector>
#include <iostream>
#include "utils/ChainHashtable/bitLeftRotate.hh"

namespace cbrc{

// if num >= max, output max.
// else output the first number from the prime list not smaller than num.
static int nextPrime( size_t num ){ 
  const size_t max =  1796359771;
  const size_t somePrimes[] = { 11, 19, 37, 73, 109, 163, 251, 367, 557, 823, 1237, 1861, 2777, 4177, 6247, 
				9371, 14057, 21089, 31627, 47431, 71143, 106721, 160073, 240101, 360163, 
				540217, 810343, 1215497, 1823231, 2734867, 4102283, 6153409, 9230113, 13845163,
				20767751, 31151623, 46727449, 70091159, 105136753, 157705109, 236557751,
				354836507, 532254761, 798382121, 1197573203, max };
  if( num >= max ) return max;
  size_t i;
  for( i = 0; somePrimes[i] < num; ++i );
  return somePrimes[i];
}

// designed so that permuting bytes gives different hash values.
static size_t bytesHashVal( const size_t numBytes, size_t* a){
    size_t numWords = numBytes/ sizeof(size_t);
    size_t numExtraBytes = numBytes - sizeof(size_t)*numWords;
    size_t key = 0;
    size_t* pastLastWord = a+numWords;
    for( ; a < pastLastWord; ++a ){
      key ^= *a;
      BIT_ROTATE_LEFT( key );
    }
    unsigned char* b = (unsigned char*) pastLastWord+1;
    switch( numExtraBytes ){ // purposely using fallthrough...
    case 3: key ^= ( *b++ << 16 );
    case 2: key ^= ( *b++ << 8 );
    case 1: key ^= *b;
    default: return key;
    }
}

// designed so that hash( s0, s1 ) != hash( s1, s0 ).
static size_t string2HashVal( const std::string& s0, const std::string& s1 ){ 
  size_t hash2 = bytesHashVal( s1.size(), (size_t*) s1.c_str() );
  BIT_ROTATE_LEFT( hash2 );
  return(  bytesHashVal( s0.size(), (size_t*) s0.c_str() ) ^ hash2 );
}
                                

// forward decls.
template<typename ValT> class String2KeyHashtable;
template<typename ValT> class String2KeyHashIterator;
template<typename ValT> class String2KeyHashtableLink;

template<typename ValT>
class String2KeyAndValue{
public:
  String2KeyAndValue( const std::string& key0, const std::string& key1, const ValT value ){ 
    keyVar0 = key0; keyVar1 = key1; valueVar = value; }
  String2KeyAndValue( const String2KeyAndValue<ValT>& snh ){ keyVar0 = snh.keyVar0; keyVar1 = snh.keyVar1;
  valueVar = snh.valueVar; }
  void operator= ( const ValT& val ){ valueVar = val; }
  operator ValT&(){ return valueVar; }
  size_t hashVal() const{ return ( string2HashVal(keyVar0, keyVar1) ); }
  bool keysEqual( const String2KeyAndValue<ValT>& snh ) const{ return( (keyVar0 == snh.keyVar0) 
								    && (keyVar1 == snh.keyVar1) ); }
  bool keysEqual( const std::string& s0, const std::string& s1 ) const{ 
    return( (keyVar0 == s0) && (keyVar1 == s1) );
  }
  std::string key0() const{ return keyVar0; }
  std::string key1() const{ return keyVar1; }
  ValT value() const{ return valueVar; }
  void setValue( const ValT& newVal ){ valueVar = newVal; }
  friend class String2KeyHashtable<ValT>;
  friend class String2KeyHashtableLink<ValT>;
private:
  std::string keyVar0;
  std::string keyVar1;
  ValT valueVar;
};


// Private helper class.
template<typename ValT>
class String2KeyHashtableLink{
public:
  friend class String2KeyHashtable<ValT>;
  friend class String2KeyAndValue<ValT>;
  friend class String2KeyHashIterator<ValT>;
  String2KeyHashtableLink<ValT>( const String2KeyHashtableLink<ValT>& link ){
    elemPtr = new String2KeyAndValue<ValT>( *(link.getElemPtr()) );
    next = NULL;
  }
  String2KeyHashtableLink<ValT>( const String2KeyAndValue<ValT>& elem ){
    elemPtr = new String2KeyAndValue<ValT>(elem);
    next = NULL;
  }
  String2KeyHashtableLink<ValT>( const std::string s0, const std::string s1, const ValT& val ){
    elemPtr = new String2KeyAndValue<ValT>( s0, s1, val );
    next = NULL;
  }
  String2KeyHashtableLink<ValT>* getNext() const{ return next; }
  ~String2KeyHashtableLink(){ delete elemPtr; }
  void setNext( String2KeyHashtableLink<ValT>* next ){ this->next = next; }
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
  String2KeyAndValue<ValT>* getElemPtr() const{ return elemPtr; }
  String2KeyAndValue<ValT>* elemPtr;
  String2KeyHashtableLink<ValT>* next;
};


// Santa's iterator elf.
template<typename ValT>
class String2KeyHashIterator{
public:
  friend class String2KeyHashtable<ValT>;
  String2KeyHashIterator<ValT>(){ head = NULL; linkPtr = NULL; } // default constructor initializes to hashtable.end()
  String2KeyHashIterator<ValT>( String2KeyHashtableLink<ValT>** head, String2KeyHashtableLink<ValT>** last ){
    this->head = head; this->last = last;
    if( head ) linkPtr = *head;
  }
  bool valid(){ return head; }
  bool operator==( const String2KeyHashIterator<ValT>& ch ) const{
    if( head != ch.head ) return false;
    return( linkPtr == ch.linkPtr );
  }
  bool operator!=( const String2KeyHashIterator<ValT>& ch ) const{ return( !(*this==ch) ); }
  String2KeyAndValue<ValT> * getElemPtr() const{ return getLinkPtr()->getElemPtr(); }
  String2KeyAndValue<ValT>& operator*() const{ return *(getElemPtr()); }

  String2KeyHashIterator<ValT>& operator++(){
    if( !valid() ) return *this;  // ++ing an invalid iterator is a no-op not an error.
    if( linkPtr->getNext() ){  // walk down one step.
      linkPtr = linkPtr->getNext();
      return (*this);
    }
    if( head >= last ){
      head = NULL; linkPtr = NULL;
      return *this;
    }
    for( ++head; (head<last) && !*head; ++head );
    if( *head )  linkPtr = *head;
    else{         head = NULL; linkPtr = NULL; }

    return *this;
  }

  //friend std::ostream& operator<<<ValT>( std::ostream& os, const String2KeyHashtable<ValT>& skh );
private:
  String2KeyHashtableLink<ValT>* getLinkPtr() const{ return linkPtr; }
  String2KeyHashtableLink<ValT>** head;
  String2KeyHashtableLink<ValT>* linkPtr;
  String2KeyHashtableLink<ValT>** last;
};


// Old saint Nick himself
template<typename ValT>
class String2KeyHashtable{
public:
  String2KeyHashtable<ValT>( size_t size = 11, size_t maxTSize = 1796359771 ) : maxTSize(maxTSize){
    tsize = nextPrime( size );
    t = new String2KeyHashtableLink<ValT>*[tsize];
    memset( t, 0, tsize*sizeof(String2KeyHashtableLink<ValT>*) );
    numElems = 0;
    //    notFoundValue = 0; could add this to a specialization for numerical value types.
    defaultValue = 1;
  }
  String2KeyHashtable<ValT>( const String2KeyHashtable<ValT>& ht ){
    copy( ht );
  }
  String2KeyHashtable<ValT> operator=( const String2KeyHashtable<ValT>& ht ){
    copy( ht );
    return *this;
  }
  void copy( const String2KeyHashtable<ValT>& ht ){ // Achtung! Deep copy not tested!
#ifdef CBRC_DEBUG
    std::cout << __FILE__ << ":" << __LINE__ << " calling copy constructor for String2Keyhashtable<" << ValT << ">\n";
#endif // defined CBRC_DEBUG
    tsize = ht.tsize;
    t = new String2KeyHashtableLink<ValT>*[tsize];
    for( size_t i = 0; i < tsize; ++i ){
      if( !ht.t[i] ){
	t[i] = 0;
	continue;
      }
      t[i] = new String2KeyHashtableLink<ValT>( *(ht.t[i]) );
      String2KeyHashtableLink<ValT>* prevHT = ht.t[i];
      String2KeyHashtableLink<ValT>* prev = t[i];
      while( String2KeyHashtableLink<ValT>* curHT = prevHT->getNext() ){
	prev->next = new String2KeyHashtableLink<ValT>( *curHT );
	prev = prev->getNext();
	prevHT = curHT;
      }
    }
  }
  ~String2KeyHashtable(){ clear(); delete t; }
  void setNotFoundValue( ValT val ){ notFoundValue = val; }
  void clear(){
    for( size_t i = 0; i < tsize; ++i ){
      if( !t[i] ) continue;
      String2KeyHashtableLink<ValT>* prev = t[i];
      while( String2KeyHashtableLink<ValT>* next = prev->getNext() ){
	delete prev;
	prev = next;
      }
      t[i] = NULL;
      delete prev;
    }
    numElems = 0;
  }
  size_t size() const{ return numElems; }
  String2KeyHashIterator<ValT> begin() const{
    if( size() < 1 )  return String2KeyHashIterator<ValT>( NULL, NULL );
    String2KeyHashtableLink<ValT>** headPtr;
    for( headPtr = t; !*headPtr; ++headPtr );
    return String2KeyHashIterator<ValT>( headPtr, t+tsize-1 );
  }
  String2KeyHashIterator<ValT> end() const{ return endVar; }
  std::vector< std::pair<std::string, ValT> > contentsAsVector() const{ // returns contents in arbitray order.
    std::vector< std::pair<std::string, ValT> > retVal;
    retVal.reserve( size() );
    for( String2KeyHashIterator<ValT> it = begin(); it != end(); ++it ){
      retVal.push_back( std::make_pair( (*it).key(), (*it).value() ) );
    }
    return retVal;
  }
  bool has( const std::string s0, const std::string s1 ) const{
    size_t index = string2HashVal( s0, s1 ) % tsize;
    String2KeyHashtableLink<ValT>* linkPtr = t[index];
    if( !linkPtr ) return false;
    for( ; !(linkPtr->getElemPtr()->keysEqual( s0, s1)) && linkPtr->getNext(); linkPtr=linkPtr->getNext() );
    if( linkPtr->getElemPtr()->keysEqual( s0, s1 ) )  return true;
    return false;
  }
  ValT operator() ( const std::string s0, const std::string s1 ) const{
    size_t index = string2HashVal( s0, s1 ) % tsize;
    String2KeyHashtableLink<ValT>* linkPtr = t[index];
    if( !linkPtr ) return notFoundValue;
    for( ; !(linkPtr->getElemPtr()->keysEqual( s0, s1 )) && linkPtr->getNext(); linkPtr=linkPtr->getNext() );
    if( linkPtr->getElemPtr()->keysEqual( s0, s1 ) )  return linkPtr->getElemPtr()->value();
    return notFoundValue;
  }
  ValT& ref( const std::string s0, const std::string s1 ){
    size_t index = string2HashVal( s0, s1 ) % tsize;
    String2KeyHashtableLink<ValT>* linkPtr = t[index];
    if( !linkPtr ){ return add2( s0, s1 ); }
    for( ; /* noop */
	 !(linkPtr->getElemPtr()->keysEqual( s0, s1)) && linkPtr->getNext();
	 linkPtr=linkPtr->getNext() );
    if( linkPtr->getElemPtr()->keysEqual( s0, s1) )  return linkPtr->getElemPtr()->valueVar;
    return add2( s0, s1 );
  }
  String2KeyAndValue<ValT>* find( const  String2KeyAndValue<ValT> & elem ){ // adds elem if necessary.
    size_t index = elem.hashVal() % tsize;
    String2KeyHashtableLink<ValT>* linkPtr = t[index];
    if( !linkPtr ){
      t[index] = new String2KeyHashtableLink<ValT>(elem);
      return (t[index]->getElemPtr());
    }
    for( ; !(elem.keyEquals(*linkPtr->getElemPtr())) && linkPtr->getNext(); linkPtr=linkPtr->getNext() );
    if( elem.keyEquals(*linkPtr->getElemPtr()) )  return linkPtr->getElemPtr();
    return NULL;
  }
  ValT& add2( const std::string& s0, const std::string& s1, bool resizeAllowed = true ){
#ifdef CBRC_DEBUG
    assert( tableStateConsistent( tsize, t ) );
#endif // debined CBRC_DEBUG    
    const unsigned int String2KeyHashtableMaxChainLen = 8;
    size_t index = string2HashVal( s0, s1 ) % tsize;
    unsigned int chainLen;
    String2KeyHashtableLink<ValT>* linkPtr = t[index];
    if( !linkPtr ){
      // std::cout << "##add to head of list at position " << index << std::endl;
      t[index] = new String2KeyHashtableLink<ValT>( s0, s1, defaultValue );
      ++numElems;
#ifdef CBRC_DEBUG
      assert( tableStateConsistent( tsize, t ) );
#endif // debined CBRC_DEBUG    
      return *(t[index]->getElemPtr());
    }
    // std::cout << "##head of list was not null so walk down\n";
    for( chainLen = 2; !(linkPtr->elemPtr->keysEqual(s0, s1)) && linkPtr->getNext(); linkPtr=linkPtr->getNext(), 
	   chainLen++ );
    if( linkPtr->elemPtr->keysEqual( s0, s1 ) ){
      // std::cout << "element already in table\n";
      return *(linkPtr->getElemPtr());
    }
    // need to allocate new slot, but first check if we need to resize.
    if( resizeAllowed && (tsize < maxTSize) && (chainLen > String2KeyHashtableMaxChainLen) ){
#ifdef CBRC_DEBUG
      assert( tableStateConsistent( tsize, t ) );
#endif // defined CBRC_DEBUG
      resize( tsize+1 );
#ifdef CBRC_DEBUG
      assert( tableStateConsistent( tsize, t ) );
#endif // defined CBRC_DEBUG
      return add2( s0, s1, false );
    }
    ++numElems;
    linkPtr->setNext(  new String2KeyHashtableLink<ValT>( s0, s1, defaultValue )  );
#ifdef CBRC_DEBUG
    assert( linkPtr->valid() );
    assert( tableStateConsistent( tsize, t ) );
#endif // defined CBRC_DEBUG
    return *(linkPtr->next->getElemPtr());
  }
  String2KeyAndValue<ValT> * add( const  String2KeyAndValue<ValT> & elem, bool resizeAllowed = true ){
    //assert( tableStateConsistent( tsize, t ) );
    const unsigned int String2KeyHashtableMaxChainLen = 10;
    size_t index = elem.hashVal() % tsize;
    unsigned int chainLen;
    String2KeyHashtableLink<ValT>* linkPtr = t[index];
    if( !linkPtr ){
      // std::cout << "##add to head of list at position " << index << std::endl;
      t[index] = new String2KeyHashtableLink<ValT>(elem);
      ++numElems;
#ifdef CBRC_DEBUG
      assert( tableStateConsistent( tsize, t ) );
#endif // defined CBRC_DEBUG
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
    if( resizeAllowed && (tsize < maxTSize) && (chainLen > String2KeyHashtableMaxChainLen) ){
#ifdef CBRC_DEBUG
      assert( tableStateConsistent( tsize, t ) );
#endif // defined CBRC_DEBUG
      resize( tsize+1 );
#ifdef CBRC_DEBUG
      assert( tableStateConsistent( tsize, t ) );
#endif // defined CBRC_DEBUG
      return add( elem, false );
    }
    ++numElems;
    linkPtr->setNext( new String2KeyHashtableLink<ValT>(elem) );
#ifdef CBRC_DEBUG
    assert( linkPtr->valid() );
    assert( tableStateConsistent( tsize, t ) );
#endif // defined CBRC_DEBUG
    return NULL;  // elem was not found in the table so return null as per the API.
  }
private:
  bool tableStateConsistent( size_t tableSize, String2KeyHashtableLink<ValT>** tablePtr ){
    if( tablePtr == t ){
      if( tableSize != tsize ){
	std::cout << "tableSize: " << tableSize << " not equal to tsize: " << tsize << "\n";
	return false;
      }
    }
    for( size_t i = 0; i < tsize; i++ ){
      if( !t[i] ) continue; // NULL's okay
      String2KeyHashtableLink<ValT>* linkPtr = t[i];
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
  void addNewToTablePtr( String2KeyHashtableLink<ValT>* oldLinkPtr, 
			 String2KeyHashtableLink<ValT>** newT, unsigned int newSize ){
    size_t index = oldLinkPtr->elemPtr->hashVal() % newSize;
    String2KeyHashtableLink<ValT>* linkPtr = newT[index];
    oldLinkPtr->setNext( NULL ); // adding to tail of chain.
    if( !linkPtr ){
      newT[index] = oldLinkPtr;
#ifdef CBRC_DEBUG
      assert( tableStateConsistent( newSize, newT ) );
#endif // defined CBRC_DEBUG
      return;
    }
    for( ;linkPtr->getNext(); linkPtr=linkPtr->getNext() );
    linkPtr->setNext( oldLinkPtr );
  }
  void resize( size_t size ){
    size_t newSize = nextPrime( size );
    // std::cout << "resizing, old_size: " << size << " new size: " << newSize << std::endl;
    String2KeyHashtableLink<ValT>** newT = new String2KeyHashtableLink<ValT>*[newSize];
    memset( newT, 0, sizeof(String2KeyHashtableLink<ValT>*)*newSize );
    String2KeyHashIterator<ValT> curIt = begin();
    String2KeyHashIterator<ValT> nextIt = curIt; 
    //assert( curIt.valid() ); //assert( nextIt.valid() ); // should be alright if table size > 2.
    for( ++nextIt; curIt.valid(); curIt = nextIt, ++nextIt ){ // curIt state gets modified so can't ++curIt.
      addNewToTablePtr( curIt.getLinkPtr(), newT, newSize );
#ifdef CBRC_DEBUG
      assert( tableStateConsistent( tsize, t ) );
      assert( tableStateConsistent( newSize, newT ) );
#endif // defined CBRC_DEBUG
    }
    String2KeyHashtableLink<ValT>** temp = t;
    t = newT;
    tsize = newSize;
    delete temp;
  }
  String2KeyHashtableLink<ValT>* lastInChain( String2KeyHashtableLink<ValT>* head ) const{
    if( !head ) return head;
    for( ;head->getNext(); head = head->getNext() );
    return head;
  }
  String2KeyHashtableLink<ValT>** t; // hashtable
  size_t maxTSize;
  size_t tsize; //table size
  unsigned int numElems;
  ValT notFoundValue;
  ValT defaultValue;
  String2KeyHashIterator<ValT> endVar;
};

template <typename ValT>
std::ostream& operator<<( std::ostream& os, const String2KeyHashtable<ValT>& skh ){
  for( String2KeyHashIterator<ValT> it = skh.begin(); it != skh.end(); ++it ){
    os << (*it).key();
    os << " " << (*it).value();
    os << std::endl;
  }
  return os;
}

  

}; // end namespace cbrc

#endif // defined _2STRINGKEYHASHTABLE_HH_

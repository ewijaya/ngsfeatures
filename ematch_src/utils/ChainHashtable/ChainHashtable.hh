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
 *  Purpose: Provide a general purpose hashtable which is easier to debug than sgi's hash_map. I do not claim
 *  that this code has any special merits other than that at this point. This code may not appear that easy to
 *  read, but try reading code from the STL...
 *
 *  O'Reilly's Practical C++ Programming claims the way to make templates portable is to inline everything,
 *  and having everything in one file does have its advantages, thus this mammoth header file. Reading the skeleton
 *  API's in this section should make the rest easier to grok.
 *  
 *  ChainHashtable is a template class constructed with type hashableType. hashableType must supply a copy
 *  constructor, destructor, and two additional special methods: size_t hashVal() const; 
 *                                                               bool keyEquals( hashableType& ht ) const;
 *  
 *  Although hashVal() must be provided by the class of type hashableType, this file provides a utility function
 *  genericHashVal which often makes this method trivial to provide:
 *
 *  size_t ChainHashtableUtils::genericHashVal( size_t numBytes, unsigned int* a);
 *   
 *  returns a general hash value for any contiguous data of length numBytes, starting at the place where
 *  a is pointing to. In general of course a cast will need to be used to make the pointer of type unsigned int*
 *  Note that the numBytes does not have to be a multiple of sizeof( unsigned int ).
 *
 *  See the implementation below for details on the hash function.
 *
 *
 *  A full STL type iterator is not provided. Instead the following bare bones ChainHashIterator is provided:
 *
 *    // skeleton of ChainHashIterator
 *    template<typename hashableType>
 *    class ChainHashIterator{
 *    public:
 *      hashableType* getElemPtr() const;
 *      bool operator==( ChainHashIterator& ch ) const;
 *      bool operator!=( ChainHashIterator& ch ) const;
 *      ChainHashIterator& ChainHashIterator::operator++();
 *      bool valid(); 
 *    }
 *
 *  I think this iterator should be reasonably robust to concurrent additions to the table, but this is untested.
 *  The iterator is definitely not robust to having the table resized.
 *
 *  The iterator is used in the following way:
 *  
 *   ChainHashtable<FreqMatrixChainHashable> table;
 *   for( ChainHashIterator<FreqMatrixChainHashable> it = table.begin(); it.valid(); ++it ){
 *       os << *(it.getElemPtr());
 *   }      
 *
 *  Here is a skeleton of the public methods ChainHashtable provides at this time:
 *
 *    template<typename hashableType>
 *    // construct with size (actual size will be a prime number >= size), see code for default values.
 *    ChainHashtable<hashableType>( size_t size = default, maxTSize = default );
 *    
 *    // if element with key equal to elem is in the table return a pointer to it, otherwise return null.
 *    // This function is currently untested.
 *    hashableType* ChainHashtable::find( const hashableType& elem )
 *    
 *    // if elem is not found in table add it and return null.
 *    // else return a pointer to the element in the table which has the same key as elem (as per elem.keyEqual())
 *    // called with resizeAllowed set to true the table may get resized to accomodate the added element.
 *    // the condition for resizing is determined by "const unsigned int ChainHashtableMaxChainLen" in this method.
 *    hashableType* add( const hashableType& elem, bool resizeAllowed = true );
 *
 *    // Clear the hash table of all elements, the hashableType destructor gets called for each cleared element.
 *    void ChainHashtable::clear()
 *    
 *     // self explanatory methods:
 *    size_t size() const{ return numElems; }
 *    ChainHashIterator<hashableType> begin() const;  
 *
 *    // for debugging.
 *    bool tableStateConsistent( size_t tableSize, ChainHashtableLink<hashableType>** tablePtr )
 *  *************************************************************************
 *
 *  Note: At this point this class has been constructed for a very particular purpose and thus has a very
 *  limited set of methods. If this turns out to truly be a resuable class, methods such as delete will added.
 */

#ifndef _CHAINHASHTABLE_HH_
#define _CHAINHASHTABLE_HH_

#include <string>
#include <iostream>
#include "utils/ChainHashtable/bitLeftRotate.hh"

namespace cbrc{

// if num >= max, output max.
// else output the first number from the prime list not smaller than num.
static int nextPrime( size_t num ){ 

  // NOTE: I should add some bigger primes for 64 bit architectures.
  const size_t max =  1796359771;
  const size_t
    somePrimes[] = {         11,        19,        37,        73,       109,
                            163,       251,       367,       557,       823,
		           1237,      1861,      2777,      4177,      6247, 
                           9371,     14057,     21089,     31627,     47431,
                          71143,    106721,    160073,    240101,    360163, 
	                 540217,    810343,   1215497,   1823231,   2734867,
                        4102283,   6153409,   9230113,  13845163,  20767751,
                       31151623,  46727449,  70091159, 105136753, 157705109,
                      236557751, 354836507, 532254761, 798382121,1197573203, max };
  if( num >= max ) return max;
  size_t i;
  for( i = 0; somePrimes[i] < num; ++i );
  return somePrimes[i];
}

// forward decls.
template<typename hashableType>class ChainHashtable;
template<typename hashableType>class ChainHashIterator;


// Private helper class.
template<typename hashableType>
class ChainHashtableLink{
public:
  friend class ChainHashIterator<hashableType>;  
  friend class ChainHashtable<hashableType>;
  //private:
  ChainHashtableLink<hashableType>( const hashableType& elem ){
    elemPtr = new hashableType(elem);
    next = NULL;
  }
  ChainHashtableLink<hashableType>* getNext() const{ return next; }
  ~ChainHashtableLink(){ delete elemPtr; }
  void setNext( ChainHashtableLink<hashableType>* next ){ this->next = next; }
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
  hashableType* getElemPtr(){ return elemPtr; }
  hashableType* elemPtr;
  ChainHashtableLink<hashableType>* next;
};


// Santa's iterator elf.
template<typename hashableType>
class ChainHashIterator{
public:
  friend class ChainHashtable<hashableType>;
  ChainHashIterator<hashableType>( ChainHashtableLink<hashableType>** head, 
				   ChainHashtableLink<hashableType>** last ){
    this->head = head; this->last = last;
    if( head ) linkPtr = *head;
  }
  bool valid(){ return head; }
  hashableType* getElemPtr() const{ return getLinkPtr()->getElemPtr(); }
  bool operator==( ChainHashIterator& ch ) const{
    if( head != ch.head ) return false;
    return( linkPtr == ch.linkPtr );
  }
  bool operator!=( ChainHashIterator& ch ) const{ return( !(*this==ch) ); }
  ChainHashIterator<hashableType>& operator++(){
    if( !valid() ) return *this;  // ++ing an invalid iterator is a no-op not an error.
    if( linkPtr->getNext() ){  // walk down one step.
      linkPtr = linkPtr->getNext();
      return (*this);
    }
    if( head >= last ){
      head = NULL;
      return *this;
    }
    for( ++head; (head<last) && !*head; ++head );
    if( *head )  linkPtr = *head;
    else         head = NULL;

    return *this;
  }
private:
  ChainHashtableLink<hashableType>* getLinkPtr() const{ return linkPtr; }
  ChainHashtableLink<hashableType>** head;
  ChainHashtableLink<hashableType>* linkPtr;
  ChainHashtableLink<hashableType>** last;
};


// Old saint Nick himself
template<typename hashableType>
class ChainHashtable{
public:
  ChainHashtable<hashableType>( size_t size = 11, size_t maxTSize = 1796359771 ) : maxTSize(maxTSize){
    //  ChainHashtable<hashableType>( size_t size = 163, size_t maxTSize = 1796359771 ) : maxTSize(maxTSize){
    tsize = nextPrime( size );
    t = new ChainHashtableLink<hashableType>*[tsize];
    memset( t, 0, tsize*sizeof(ChainHashtableLink<hashableType>*) );
    numElems = 0;
  }
  ~ChainHashtable(){ clear(); delete t; }

  void clear(){
    for( size_t i = 0; i < tsize; ++i ){
      if( !t[i] ) continue;
      ChainHashtableLink<hashableType>* prev = t[i];
      while( ChainHashtableLink<hashableType>* next = prev->getNext() ){
	delete prev;
	prev = next;
      }
      t[i] = NULL;
      delete prev;
    }
    numElems = 0;
  }
  size_t size() const{ return numElems; }
  ChainHashIterator<hashableType> begin() const{
    if( size() < 1 )  return ChainHashIterator<hashableType>( NULL, NULL );
    ChainHashtableLink<hashableType>** headPtr;
    for( headPtr = t; !*headPtr; ++headPtr );
    return ChainHashIterator<hashableType>( headPtr, t+tsize-1 );
  }
  hashableType* find( const hashableType& elem ){ // adds elem if necessary.
    size_t index = elem.hashVal() % tsize;
    ChainHashtableLink<hashableType>* linkPtr = t[index];
    if( !linkPtr ){
      t[index] = new ChainHashtableLink<hashableType>(elem);
      return (t[index]->getElemPtr());
    }
    for( ; !(elem.keyEquals(*linkPtr->getElemPtr())) && linkPtr->getNext(); linkPtr=linkPtr->getNext() );
    if( elem.keyEquals(*linkPtr->getElemPtr()) )  return linkPtr->getElemPtr();
    return NULL;
  }
  hashableType* add( const hashableType& elem, bool resizeAllowed = true ){
    //assert( tableStateConsistent( tsize, t ) );
    const unsigned int ChainHashtableMaxChainLen = 10;
    size_t index = elem.hashVal() % tsize;
    unsigned int chainLen;
    ChainHashtableLink<hashableType>* linkPtr = t[index];
    if( !linkPtr ){
      // std::cout << "##add to head of list at position " << index << std::endl;
      t[index] = new ChainHashtableLink<hashableType>(elem);
      ++numElems;
      //assert( tableStateConsistent( tsize, t ) );
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
    if( resizeAllowed && (tsize < maxTSize) && (chainLen > ChainHashtableMaxChainLen) ){
      //assert( tableStateConsistent( tsize, t ) );
      resize( tsize+1 );
      //assert( tableStateConsistent( tsize, t ) );
      return add( elem, false );
    }
    ++numElems;
    linkPtr->setNext( new ChainHashtableLink<hashableType>(elem) );
    //assert( linkPtr->valid() );
    //assert( tableStateConsistent( tsize, t ) );
    return NULL;  // elem was not found in the table so return null as per the API.
  }
private:
  bool tableStateConsistent( size_t tableSize, ChainHashtableLink<hashableType>** tablePtr ){
    if( tablePtr == t ){
      if( tableSize != tsize ){
	std::cout << "tableSize: " << tableSize << " not equal to tsize: " << tsize << "\n";
	return false;
      }
    }
    for( size_t i = 0; i < tsize; i++ ){
      if( !t[i] ) continue; // NULL's okay
      ChainHashtableLink<hashableType>* linkPtr = t[i];
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
  // used be resize so can assume oldElemPtr is not in new table.
  void addNewToTablePtr( ChainHashtableLink<hashableType>* oldLinkPtr, 
			 ChainHashtableLink<hashableType>** newT, unsigned int newSize ){
    size_t index = oldLinkPtr->elemPtr->hashVal() % newSize;
    ChainHashtableLink<hashableType>* linkPtr = newT[index];
    oldLinkPtr->setNext( NULL ); // adding to tail of chain.
    if( !linkPtr ){
      newT[index] = oldLinkPtr;
      //assert( tableStateConsistent( newSize, newT ) );
      return;
    }
    for( ;linkPtr->getNext(); linkPtr=linkPtr->getNext() );
    linkPtr->setNext( oldLinkPtr );
  }
  void resize( size_t size ){
    size_t newSize = nextPrime( size );
    // std::cout << "resizing, old_size: " << size << " new size: " << newSize << std::endl;
    ChainHashtableLink<hashableType>** newT = new ChainHashtableLink<hashableType>*[newSize];
    memset( newT, 0, sizeof(ChainHashtableLink<hashableType>*)*newSize );
    ChainHashIterator<hashableType> curIt = begin();
    ChainHashIterator<hashableType> nextIt = curIt; 
    //assert( curIt.valid() ); //assert( nextIt.valid() ); // should be alright if table size > 2.
    for( ++nextIt; curIt.valid(); curIt = nextIt, ++nextIt ){ // curIt state gets modified so can't ++curIt.
      addNewToTablePtr( curIt.getLinkPtr(), newT, newSize );
      //assert( tableStateConsistent( tsize, t ) );
      //assert( tableStateConsistent( newSize, newT ) );
    }
    ChainHashtableLink<hashableType>** temp = t;
    t = newT;
    tsize = newSize;
    delete temp;
  }
  ChainHashtableLink<hashableType>* lastInChain( ChainHashtableLink<hashableType>* head ) const{
    if( !head ) return head;
    for( ;head->getNext(); head = head->getNext() );
    return head;
  }
  ChainHashtableLink<hashableType>** t; // hashtable
  const size_t maxTSize;
  size_t tsize; //table size
  unsigned int numElems;
};

namespace ChainHashtableUtils{
  // utility which can make hashVal() trivial to write in many cases.
  // It uses exclusive or and bit rotation to produce a hash function which should be robust to
  // most cases of transposing elements or bytes (using only exclusive or is weak against transpositions)
  inline size_t genericHashVal( const size_t& numBytes, const size_t* a){
    size_t numWords = numBytes/4;
    size_t numExtraBytes = numBytes - 4*numWords;
    size_t key = 0;
    const size_t* pastLastWord = a+numWords;
    for( ; a < pastLastWord; ++a ){
      key ^= *a;
      BIT_ROTATE_LEFT( key );
    }
    const size_t b = * (size_t*) pastLastWord;
#ifdef __x86_64__
  static const size_t mask[] = {
    0x0000000000000000,
    0x00000000000000FF,
    0x000000000000FFFF,
    0x0000000000FFFFFF,
    0x00000000FFFFFFFF,
    0x000000FFFFFFFFFF,
    0x0000FFFFFFFFFFFF,
    0x00FFFFFFFFFFFFFF,
  };
  return(  key ^ (b & mask[numExtraBytes])  );
#elif __sparc__  // big endian
  if( numExtraBytes == 1 )  return(  key ^ (b & 0xFF000000)  );
  if( numExtraBytes == 2 )  return(  key ^ (b & 0xFFFF0000)  );
  /*  numExtraBytes == 3 */ return(  key ^ (b & 0xFFFFFF00)  );
#elif __i386__
   if( numExtraBytes == 1 )  return(  key ^ (b & 0x000000FF)  );
   if( numExtraBytes == 2 )  return(  key ^ (b & 0x0000FFFF)  );
   /*  numExtraBytes == 3 */ return(  key ^ (b & 0x00FFFFFF)  );
#else
#error unknown architecture. only __x86_64__, __i386__ and __sparc__ currrently supported
#endif // big or little endian
  }
}

}; // end namespace cbrc

#endif // defined _CHAINHASHTABLE_HH_

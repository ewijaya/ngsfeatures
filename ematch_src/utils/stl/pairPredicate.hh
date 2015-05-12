/*
 *  Author: "Paul B. Horton"
 *  
 *  Organization: Computational Biology Research Center, Japan
 *
 *  Copyright: All rights reserved
 *  
 *  Creation Date: 2004.12.1
 *
 *  Last Modified: 2004.12.1
 *  
 *  Description: some predicates for std::pair.
 *
 */
#include <functional>
#include <cmath>
#include <utility>

namespace cbrc{
  namespace pairPredicate{

    template<typename T0, typename T1>
    bool secondLess( const std::pair<T0, T1>& p0, const std::pair<T0, T1>& p1 ){
      return( p0.second < p1.second );
    }

    template<typename T0, typename T1>
    bool secondGreater( const std::pair<T0, T1>& p0, const std::pair<T0, T1>& p1 ){
      return( p0.second > p1.second );
    }

    template<typename T0, typename T1>
    bool secondAbsGreater( const std::pair<T0, T1>& p0, const std::pair<T0, T1>& p1 ){
      return( fabs(p0.second) > fabs(p1.second) );
    }

    template<typename T0, typename T1>
    class FirstEqualUni{
    public:
      FirstEqualUni( T0 val ) : val(val){}
      bool operator()( const std::pair<T0, T1> p ) const{ return p.first == val; }
    private: 
      T0 val;
    };

    template<typename T0, typename T1>
    class secondPredicate{
    public:
      secondPredicate( bool(*pred) (const T1&, const T1&) ) : pred(pred) {}
      bool operator()( const std::pair<T0, T1>& p0, const std::pair<T0, T1>& p1 ) const{
	return pred( p0.second, p1.second );
      }
    private:
      bool(*pred) (const T1&, const T1&);
    };
  }; // end namespace pairPredicate
}; // end namespace cbrc


/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2008, Paul B. Horton, All rights reserved.
 *  Creation Date: 2008.1.15
 *  Last Modified: $Date: 2009/06/09 02:18:57 $
 *
 *  Description: Container holding information about sequence typically found
 *               in fasta files. Namely, the sequence itself, and a list starting
 *               with a name (sometimes called an ID) possibly followed with other
 *               optional fields
 *
 *
 */
#ifndef LABELEDASCIISEQ_HH
#define LABELEDASCIISEQ_HH
#include <string>
#include <vector>
#include <iostream>
#include "utils/stl/stringUtils.hh"

namespace cbrc{

class LabeledAsciiSeq{
public:
  /* ********** CONSTRUCTORS ********** */
  LabeledAsciiSeq()
    : _seq( stringUtils::null ),
      _labels(0)
  {}

  LabeledAsciiSeq( const std::string&  seq,
		   const std::vector<std::string>&   labels ) :
    _seq(seq){
    std::copy(   labels.begin(), labels.end(),
		 std::back_inserter( _labels )   );
  }

  LabeledAsciiSeq( const LabeledAsciiSeq& labeledAsciiSeq ) :
    _seq( labeledAsciiSeq._seq )
  {
    _labels.reserve( labeledAsciiSeq.labels().size() );
    std::copy( labeledAsciiSeq.labels().begin(),
	       labeledAsciiSeq.labels().end(),
	       std::back_inserter( _labels ) );
  }

  explicit LabeledAsciiSeq( const std::string& seq,
			    const std::string name = defaultSeqName() ) :
    _seq(seq){
    _labels.push_back( name );
  }

  // Assignment
  const LabeledAsciiSeq&  operator=( const LabeledAsciiSeq& labSeq ){
    _seq = labSeq.seq();
    _labels.clear();
    _labels.reserve( labSeq.labels().size() );
    std::copy( labSeq.labels().begin(),
	       labSeq.labels().end(),
	       std::back_inserter( _labels ) );
    return *this;
  }

  /* ********** ACCESSORS ********** */
  const std::string& seq() const                      {  return _seq;  }
  /***/ void         setSeq( const std::string& seq ) {  _seq = seq;   }

  const std::string& name() const                    { return label(0);       }
  void setName( const std::string& name )            { _labels[0] = name;     }

  size_t             size() const                     {  return seq().size();  }

  const char&        operator()( const size_t& idx )  {  return _seq[idx];     }


  const std::string&              label( const size_t& i ) const  { return _labels.at(i);  }
  const std::vector<std::string>& labels() const                  { return _labels;        }
  const size_t                    numLabels() const               { return _labels.size(); }

  void setLabels( const std::vector<std::string>& labels ){
      _labels = labels;
  }

private:

  /* ** static methods ** */
  static const std::string& defaultSeqName(){
    static const std::string _defaultSeqName( "unknown" );
    return _defaultSeqName;
  }

  /* ** object data ** */
  std::string _seq;
  std::vector<std::string> _labels;
};



inline std::ostream& operator<<( std::ostream& os, const LabeledAsciiSeq& namedSeq ){
  os << "> ";
  for( size_t i = 0; i < namedSeq.numLabels(); ++i ){
    os << namedSeq.label(i) << "\t";
  }
  os << std::endl << namedSeq.seq() << std::endl;
  return os;
}

} // end namespace cbrc
#endif // LABELEDASCIISEQ_HH
#if 0 // ** start pod **
=pod

=head1 NAME

B<LabeledAsciiSeq>

=head1 SYNOPSIS

Container holding ascii sequence and array of string labels.

=head1 DESCRIPTION

Container holding sequence and array of string labels. The
sequence is stored ascii representation. The first label is
the name of the sequence.

=head1 Constructors

  LabeledAsciiSeq( const std::string&              seq,
		   const std::vector<std::string>& labels ) :

  LabeledAsciiSeq( const LabeledAsciiSeq& labeledAsciiSeq ) :

  explicit LabeledAsciiSeq( const std::string& seq,
			    const std::string name = "LABELEDASCIISEQ_DEFAULT_NAME" ) :

  const LabeledAsciiSeq&  operator=( const LabeledAsciiSeq& labSeq );


=head1 ACCESSORS

  const std::string& seq() const;
  /***/ void setSeq( const std::string& seq );

  const std::string& name() const;
  void setName( const std::string& name );

  const char& operator()( const size_t& idx );
  size_t size() const;

C<size()> returns the sequence length.

=head2 Label Accessors

  const std::string& label( const size_t& i ) const;
  const std::vector<std::string>& labels() const;
  const size_t numLabels() const;
  void setLabels( const std::vector<std::string>& labels );

These all do what you would expect.

=head1 AUTHOR

Paul B. Horton

=head1 COPYRIGHT

Copyright (C) 2008 Paul B. Horton, All rights reserved.

=cut

#endif // ** end of pod **

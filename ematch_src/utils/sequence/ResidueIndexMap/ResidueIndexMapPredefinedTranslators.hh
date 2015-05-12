/*  
 *  Author: Paul B. Horton
 *  Organization: Computational Biology Research Center, AIST, Japan
 *  Copyright (C) 2008, Paul B. Horton, All rights reserved.
 *  Creation Date: 2008.8.12
 *  Last Modified: $Date: 2008/08/13 13:01:50 $
 *
 *  Description: Predefined codon translators.
 *
 */
#ifndef RESIDUEINDEXMAPPREDEFINEDTRANSLATORS_HH_
#define RESIDUEINDEXMAPPREDEFINEDTRANSLATORS_HH_
#include "ResidueIndexMapPredefined.hh"
#include "ResidueIndexMapTranslator.hh"

namespace cbrc{


ResidueIndexMapTranslator standardRNACodonTranslator(  RNAProperResidueIndexMap,
						       aminoResidueIndexMap,
"\
gca:A	gcc:A	gcg:A	gcu:A	aga:R	agg:R	cga:R	cgc:R	\
cgg:R	cgu:R	gac:D	gau:D	aac:N	aau:N	ugc:C	ugu:C	\
gaa:E	gag:E	caa:Q	cag:Q	gga:G	ggc:G	ggg:G	ggu:G	\
cau:H	aua:I	auc:I	auu:I	uua:L	uug:L	cua:L	cuc:L	\
cug:L	cuu:L	aaa:K	aag:K	aug:M	uuc:F	uuu:F	cca:P	\
ccc:P	ccg:P	ccu:P	agc:S	agu:S	uca:S	ucc:S	ucg:S	\
ucu:S	aca:T	acc:T	acg:T	acu:T	ugg:W	uac:Y	uau:Y	\
gua:V	guc:V	gug:V	guu:V	uaa:stop	uag:stop	uga:stop\
" );



} // end namespace cbrc
#endif // RESIDUEINDEXMAPPREDEFINEDTRANSLATORS_HH_

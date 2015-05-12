  /* Fields separated by splitChar with length at least 1 are returned. */
  std::vector<std::string> split( const std::string s, const char splitChar ){
    std::vector<std::string> fields;
    if( s.size() == 0 ) return( fields );
    bool prevMatched;
    std::string curField = "";
    int i;

    // skip leading delimiter chars.
    for( i = -1; (i < (int) s.size()-1) && (s[++i] == splitChar);  );
    // at this point s[i] is either the first non delim char,
    // or if no such char exists, s[i] is the last character.
    if( i >= (int) s.size() - 1 ){ // if s[i] is the last character
      if( s[i] != splitChar ){
	fields.push_back(  s.substr( i,s.size() )  );
      }
      return( fields );
    }
    prevMatched = false;
    curField = s[i];
    for( ++i; i < (int) s.size(); ++i ){
      if( prevMatched ){
	if( s[i] != splitChar ){
	  curField = s[i];
	  prevMatched = false;
	}
      }else{
	if( s[i] == splitChar ){
	  fields.push_back( curField );
	  curField = "";
	  prevMatched = true;
	}else{
	  curField += s[i];
	}
      }
    }
    if( !prevMatched )  fields.push_back( curField );

    return fields;
  }

  /* Up to LIMIT fields, separated by one or more occurrences of splitChar are returned. */
  /* if the number of fields in S is more than LIMIT, then the remained of S is returned */
  /* in retval[LIMIT-1]. */
  std::vector<std::string> split( const std::string s, const char splitChar, const size_t limit ){
    size_t curFieldNum = 0;
    std::vector<std::string> fields;
    if( s.size() == 0 ) return( fields );
    if( !limit ){
      return( fields );
    }
    bool prevMatched;
    std::string curField = "";
    int i;

    // skip leading delimiter chars.
    for( i = -1; (i < (int) s.size()-1) && (s[++i] == splitChar);  );
    // at this point s[i] is either the first non delim char,
    // or if no such char exists, s[i] is the last character.
    if( i >= (int) s.size() -1 ){ // if s[i] is the last character
      if( s[i] != splitChar ){
	fields.push_back(  s.substr( i,s.size() )  );
      }
      return fields;
    }
    prevMatched = false;
    curField = s[i];
    for( ++i; i < (int) s.size(); ++i ){
      if( prevMatched ){
	if( s[i] != splitChar ){
	  curField = s[i];
	  prevMatched = false;
	}
      }else{
	if( s[i] == splitChar ){
	  fields.push_back( curField );
	  if( ++curFieldNum >= limit ){
	    // reached limit, so concatenate the rest of the string to the last field and return.
	    fields[fields.size()-1] = fields[fields.size()-1] + s.substr( i );
	    return fields;
	  }
	  curField = "";
	  prevMatched = true;
	}else{
	  curField += s[i];
	}
      }
    }
    if( !prevMatched )  fields.push_back( curField );

    return fields;
  }

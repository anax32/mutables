// FIXME: make these a global variable set (i.e., environment vars) which the user can amend.
// FIXME: scale the text accounting for screen size/dpi
#define MUTABLE_FLOAT_PRECISION    (6)      // number of precision digits
#define MUTABLE_FLOAT_ROUNDING          // do rounding
#define MUTABLE_SELECTED_TEXT_SCALE  (25.0f)
#define MUTABLE_TEXT_SCALE      (20.0f)
#undef MUTABLE_LIST_TYPES

#include <iomanip>

namespace Mutables
{
/**
 * tokenisation support functions
 * FIXME: make an container with iterators?
 */
class Token
{
public:
  static const unsigned char    MARKER = '@';

  static size_t next (const Param& p, const size_t s)        {return p.find (MARKER, s);}
  static size_t begin (const Param& p)              {return 0;}
  static const bool end (const Param& p, const size_t s)      {return (s == p.npos);}
  static const bool has (const Param& p, const size_t s)      {return (!end (p, s));}
  /**
   * Tokenises strings around the # character.
   * @param p the string to tokenise.
   * @param n the index of the token to retrieve (starting at zero).
   * @param outBuffer a Param object to which the new token will be appended.
   * @returns true if a token was extracted, false otherwise.
   */
  static bool get (const Param& p, const unsigned int n, Param& outBuffer)
  {
    unsigned int  i;
    size_t      s, e;

    // search the input to find the token.
#ifdef TOK_FNCS
    for (i=0,s=begin (p), e=next (p, s);
       has (p, s) && (i<n);
       s=e, e=next (p,e+1),i++)
#else
    for (s=0,e=p.find (MARKER),i=0;      // s = begining of string, e = first marker sighting.
       (i<n)&&(s!=p.npos);        // stop when we have n token sightings, or the end of the string
       s=e,e=p.find (MARKER,s+1),i++)    // s = last marker, e = next marker, i++
#endif
    {
      ;;  // doesn't process the text at all
    }

    // the start position is still within the string, so copy the substring
    if (s<p.npos)
    {
      if (s==0)    {outBuffer.append (p.substr (s, e-s));}
      else      {outBuffer.append (p.substr (s+1, e-(s+1)));}

      return true;
    }

    // something didn't work out
    return false;
  }
  // FIXME: this is not optimal!!
  template<typename T>
  static bool get (T& out, const Param& p, const unsigned int tokenIndex = 0)
  {
    Param  buf ("");

    // set the output to the error value
  //  out = std::numeric_limits<T>::infinity ();

    // read the value
    if (Token::get (p, tokenIndex, buf) == false)        {return false;}

    // attempt to convert the string to type T
    if ((std::istringstream (buf) >> out).fail () == true)    {return false;}

    // seemed to go ok
    return true;
  }
  /**
   * writes a token "newValue" to location "index" in the string "p".
   * @param p string to be ammended
   * @param index location to change the value of
   * @param newValue data to put in the string (must have the string input operator << available)
   * @returns true if the value was inserted into the string, false otherwise.
   */
  template<class T> static bool put (Param& p, const unsigned int index, const T newValue)
  {
    unsigned int    i;
    size_t        s, e;
    std::stringstream  ss;
    Param        buf ("");

#ifdef TOK_FNCS
    for (i=0,s=begin (p),e=next (p, s);
       has (p, s);
       s=e, e=next (p,e+1),i++)
#else
    for (s=0,e=p.find(MARKER),i=0;
      (s!=p.npos);
       s=e,e=p.find (MARKER,s+1),i++)
#endif
    {
      // write a token to the list
      if (i == index)
      {
        ss << std::fixed << std::setprecision (MUTABLE_FLOAT_PRECISION) << newValue;
      }
      else
      {
        buf.clear ();

        if (get (p, i, buf) == true)    {ss << buf;}
        else                {ss << UNSET;}
      }

      // place a marker if we've written a token and not gone to the end of the string
      if (e!=p.npos)      {ss << MARKER;}
    }

    // clear the old value
    p.clear ();
    // set the new value
    p.assign (ss.str ());
    // return true
    return true;
  }

#if 0
  // this was only used for list types
  template<class T> bool appendToken (const T newValue)
  {
    std::stringstream  ss;

    ss << value ();
    ss << MARKER;
    ss << std::fixed << newValue;

    clear ();
    return setValue (ss.str ());
  }
#endif
// EXTENSIONS
  static void nameAndTypeFromPath (const Param& path, Param& dir, Param& name, Param& type)
  {

    // get the file name and extension
    dir.assign  (path.substr (0, path.rfind ('\\')));
    name.assign (path.substr (path.rfind ('\\')+1, path.npos));
    type.assign (path.substr (path.rfind ('.')+1, path.npos));
  }
};


// explicit specialisations
template<>
bool Token::get (Param& out, const Param& p, const unsigned int tokenIndex)
{
  if (Token::get (p, tokenIndex, out) == false)        {return false;}

  return true;
}

};

// FIXME: use a different namespace, these are not core features of mutables
namespace Mutables
{
/**
 * VALIDATORS
 */
class VLD_Unknown : public Validator
{
  public:
    virtual void operator() (void)
    {
      setResult (true);
    }
};
class VLD_Bottom : public Validator
{
  public:
    virtual void operator() (void)
    {
      setResult (descriptionSet[Types::bottom ()].defaultValue () == value ());
    }
};
class VLD_ByList : public Validator
{
protected:
  Param    ty;
public:
  VLD_ByList (const Param& type)
    : ty (type)
  {}
  void operator() (void)
  {
  //  setResult (contains (Property::descriptionSet[ty].values (), value ()));
    setResult (descriptionSet[ty].values () && value ());
  }
};
// do case insensitive string comparions,
// if the value is found, put the correct case layout into the input value
// so the caller should use that instead of the test value
class VLD_ByListCaseless : public Validator
{
protected:
  Param    ty;
public:
  VLD_ByListCaseless (const Param& type)
    : ty (type)
  {}
  void operator() (void)
  {
    Description::ValueList::ConstParamList  pl;
    Description::ValueList& vs = descriptionSet[qualifier ()].values ();

    setResult (false);

    for (pl=vs.begin (); pl!=vs.end (); pl++)
    {
    //  if (tl == a)
      if (pl->first.length () != value ().length ())
      {
        continue;
      }

      // NB: case insensitive comparison.
      // _mbsicmp and _mbsicmp_l cannot be used in applications that execute in the Windows Runtime. For more information, see CRT functions not supported with /ZW.
      // http://msdn.microsoft.com/en-us/library/k59z8dwe.aspx
#ifdef _WIN32_
      if (_stricmp (pl->first.c_str (), value ().c_str ()) == 0)
#else
      if (pl->first == value ())
#endif
      {
        setResult (true);
        val.assign (pl->first);
        break;
      }
    }
  }
};
class VLD_Label : public Validator
{
public:
  static bool test (const Param testValue)
  {
    return std::isalpha (testValue.at (0), languageLocale);
  }
  virtual void operator() ()
  {
    setResult (test (value ()));
  }
};
class VLD_Numeric : public Validator
{
  public:
    virtual void operator() (void)
    {
      float v;
      setResult (Mutables::Property::getNumber (v, value ()));
    }
};
class VLD_Numeric_Pos : public Validator
{
  public:
    virtual void operator() (void)
    {
      float v;
      setResult (Mutables::Property::getNumber (v, value ()) &&
                 (v > 0.0f - std::numeric_limits<float>::epsilon ()));
    }
};
class VLD_Numeric_Neg : public Validator
{
  public:
    virtual void operator() (void)
    {
      float v;
      setResult (Mutables::Property::getNumber (v, value ()) &&
                 (v < 0.0f));
    }
};
class VLD_Numeric_Unit : public Validator
{
  public:
    virtual void operator() (void)
    {
      float  v;
      setResult (Mutables::Property::getNumber (v, value (), 0) &&
                 (v > 0.0f - std::numeric_limits<float>::epsilon ()) &&
                 (v < 1.0f + std::numeric_limits<float>::min ()));
    }
};
class VLD_Colour : public Validator
{
public:
  virtual void operator() (void)
  {
    if ((value ().at (0) == '#') && (value ().length () == 7))
    {
      setResult (true);
    }
    else
    {
      setResult (false);
    }
  }
};
class VLD_Image : public Validator
{
public:
  virtual void operator() (void)
  {
#ifdef __GL_H__
    GLUtils::TextureDescription  txd;

    setResult (Mutables::Property::getImage (txd, value ()));
    setResult (glIsTexture (txd.index ()) == GL_TRUE);
#endif
  }
};
class VLD_Buffer : public Validator
{
public:
  virtual void operator() (void)
  {
#ifdef __GL_H__
    GLUtils::BufferDescription  buf;

    setResult (Mutables::Property::getBuffer (buf, value ()));
    setResult (glIsBuffer (buf.index ()) == GL_TRUE);
#endif
  }
};
class VLD_Range : public Validator
{
  public:
    virtual void operator() (void)
    {
      float  v;
      setResult (Mutables::Property::getNumber (v, value (), 0) &&
                 Mutables::Property::getNumber (v, value (), 1));
    }
};
/**
 * Tests if a given attribute name is an input attribute to the mutable set.
 * These are assigned to individual mutable property sets, NOT descriptions of types.
 */
class VLD_IsInputAttribute : public Validator
{
protected:
  const PropertySet&  ps;

public:
  VLD_IsInputAttribute (const PropertySet& propertySet)
    : ps (propertySet)
  {}
  void operator() (void)
  {
    if (value () == "?\0")
    {
      setResult (false);
      return;
    }
    if ((ps && Param("Input\0")) == false)
    {
      setResult (false);
      return;
    }
    if (ps["Input\0"].getAttribute () != value ())
    {
      setResult (false);
      return;
    }

    setResult (true);
    return;
  }
};

};
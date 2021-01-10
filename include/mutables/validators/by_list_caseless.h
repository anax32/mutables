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

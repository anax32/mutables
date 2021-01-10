/**
 * Tests if a given attribute name is an input attribute to the mutable set.
 * These are assigned to individual mutable property sets, NOT descriptions of types.
 */
class VLD_IsInputAttribute : public Validator
{
protected:
  const PropertySet&  ps;

public:
  VLD_IsInputAttribute (const Mutables::PropertySet& propertySet)
    : ps (propertySet)
  {}
  void operator() (void)
  {
    if (value () == "?\0")
    {
      setResult (false);
      return;
    }
    // FIXME: restore
    //if ((ps && "Input\0") == false)
    //{
    //  setResult (false);
    //  return;
    //}
    //if (ps["Input\0"].getAttribute () != value ())
    //{
    //  setResult (false);
    //  return;
    //}

    setResult (true);
    return;
  }
};

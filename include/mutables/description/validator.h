/**
 * Base class to validate whether a Param value is a valid form for a given type.
 * Each validator should be associated with a type.
 * The validator is then associated with a property and queried each time a value is updated.
 * Default types have default validators.
 */
class Validator : public Utils::Callback
{
protected:
  Param   val;  // last test value
  Param   qwl;  // qualifier
  bool    res;  // last result

  void setResult (const bool newRes)
  {
    res = newRes;
  }
public:
  Validator ()
    : val (UNSET), qwl (UNSET), res (false)
  {}
  Validator (const Param& qualifierParameter)
    : val (UNSET), qwl (qualifierParameter), res (false)
  {}
  // mutators
  void clear ()
  {
    val.assign (UNSET);
  }
  void set (const Param& input)
  {
    val.assign (input);
  }
  void set (const Param& input, const Param& param)
  {
    val.assign (input);
    qwl.assign (param);
  }

  const Param& value () const
  {
    return val;
  }
  const Param& qualifier () const
  {
    return qwl;
  }
  void get (Param& out)
  {
    out.assign (val);
    clear ();
  }

//  virtual bool operator() (const Param testValue) const = 0;
  bool valid () const
  {
    return res;
  }
};

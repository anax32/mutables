class MUT_Numeric_Prev : public Mutator
{
protected:
  const Param type;
public:
  MUT_Numeric_Prev (const Param typeTarget)
    : type (typeTarget)
  {}
  void operator() (void)
  {
    float v;

    if (Property::getNumber (v, value ()) == false)    {return;}

    if (alt () == true)
    {
      v -= MUT_Numeric_Next::increment_small ();
    }
    else
    {
      v -= MUT_Numeric_Next::increment_large ();
    }

    Property::set (val, v);
  }
};

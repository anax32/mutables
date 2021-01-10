class MUT_Numeric_Next : public Mutator
{
protected:
  const Param type;

public:
  static const float increment_large ()  {return 1.0f;}
  static const float increment_small ()  {return 0.01f;}
public:
  MUT_Numeric_Next (const Param typeTarget)
    : type (typeTarget)
  {}
  void operator() (void)
  {
    float  v;

    if (Property::getNumber (v, value ()) == false)    {return;}

    if (alt () == true)
    {
      v += increment_small ();
    }
    else
    {
      v += increment_large ();
    }

    Property::set (val, v);
  }
};

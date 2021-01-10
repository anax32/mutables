class MUT_Unit_Prev : public Mutator
{
public:
  MUT_Unit_Prev ()
  {}
  void operator() (void)
  {
    Validator  *vld;
    float    v, o;

    if (Property::getNumber (v, value ()) == false)    {return;}

    o = v;
    v -= MUT_Numeric_Next::increment_small ();
    Property::set (val, v);

    // validate the input
    vld = dynamic_cast<Validator*>(&descriptionSet[Types::numeric ()]["unitValue\0"]);

    if (vld == NULL)                  {return;}

    vld->set (value ());
    (*vld) ();

    if (vld->valid () == false)
    {
      Property::set (val, o);
    }
  }
};

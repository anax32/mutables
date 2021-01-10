class MUT_Unit_Next : public Mutator
{
public:
  MUT_Unit_Next ()
  {}
  void operator() (void)
  {
    Validator  *vld;
    float v, o;

    if (Property::getNumber (v, value ()) == false)    {return;}

    o = v;  // store the old value
    v += MUT_Numeric_Next::increment_small ();  // do the increment
    Property::set (val, v);  // update the stored value

    // validate the input
    vld = dynamic_cast<Validator*>(&descriptionSet[Types::numeric ()]["unitValue\0"]);

    if (vld == NULL)                  {return;}

    vld->set (value ());
    (*vld) ();

    if (vld->valid () == false)  // input is not valid
    {
      Property::set (val, o);  // restore the old value
    }
  }
};

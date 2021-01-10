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

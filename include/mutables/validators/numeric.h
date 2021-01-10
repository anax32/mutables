class VLD_Numeric : public Validator
{
  public:
    virtual void operator() (void)
    {
      float v;
      setResult (Mutables::Property::getNumber (v, value ()));
    }
};

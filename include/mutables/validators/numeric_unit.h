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

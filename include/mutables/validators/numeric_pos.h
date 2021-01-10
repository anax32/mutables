class VLD_Numeric_Pos : public Validator
{
  public:
    virtual void operator() (void)
    {
      float v;
      setResult (Mutables::Property::getNumber (v, value ()) &&
                 (v > 0.0f - std::numeric_limits<float>::epsilon ()));
    }
};

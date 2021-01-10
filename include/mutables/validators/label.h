class VLD_Label : public Validator
{
public:
  static bool test (const Param testValue)
  {
    return std::isalpha (testValue.at (0), languageLocale);
  }
  virtual void operator() ()
  {
    setResult (test (value ()));
  }
};

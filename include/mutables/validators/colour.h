class VLD_Colour : public Validator
{
public:
  virtual void operator() (void)
  {
    if ((value ().at (0) == '#') && (value ().length () == 7))
    {
      setResult (true);
    }
    else
    {
      setResult (false);
    }
  }
};

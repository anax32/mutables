class VLD_ByList : public Validator
{
protected:
  Param    ty;
public:
  VLD_ByList (const Param& type)
    : ty (type)
  {}
  void operator() (void)
  {
  //  setResult (contains (Property::descriptionSet[ty].values (), value ()));
    setResult (descriptionSet[ty].values () && value ());
  }
};

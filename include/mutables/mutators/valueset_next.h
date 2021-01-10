class MUT_ValueSet_Next : public Mutator
{
public:
  MUT_ValueSet_Next (const Param qualifierParam)
    : Mutator (qualifierParam)
  {}
  void operator() (void)
  {
#ifdef _DEBUG
    if ((descriptionSet && qualifier ()) == false)    {return;}
#endif
    set (Description::nextValue (descriptionSet[qualifier ()].values (), value ()));
  }
};

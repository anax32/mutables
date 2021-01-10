class MUT_ValueSet_Prev : public Mutator
{
public:
  MUT_ValueSet_Prev (const Param qualifierParam)
    : Mutator (qualifierParam)
  {}
  void operator() (void)
  {
#ifdef _DEBUG
    if ((descriptionSet && qualifier ()) == false)    {return;}
#endif
    set (Description::previousValue (descriptionSet[qualifier ()].values (), value ()));
  }
};

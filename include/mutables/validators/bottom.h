class VLD_Bottom : public Validator
{
  public:
    virtual void operator() (void)
    {
      setResult (descriptionSet[Types::bottom ()].defaultValue () == value ());
    }
};

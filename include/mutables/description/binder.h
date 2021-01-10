/**
 * Base class for a binding process
 */
class Binder : public Validator
{
protected:
  Utils::CallbackSet*  trg;

  Utils::CallbackSet& target () const
  {
    return *trg;
  }
public:
  Binder ()
    : trg (NULL)
  {}
  void setTarget (Utils::CallbackSet* newTarget)
  {
    trg = newTarget;
  }
  void operator() (void)
  {
    if (trg == NULL)
    {
      setReturnValue (FALSE);
    }
    else
    {
      setReturnValue (TRUE);
    }
    return;
  }
};

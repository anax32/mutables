/**
 * Base class for a mutator of a type.
 * This is for unary functional mutation, such as increments, decrements and so on.
 * Default types have default mutators, which may be noops.
 * value () is should be set by the caller and is cleared after each invocation.
 * qualifier () is set during construction and is not cleared after each invocation (but may be set by the caller if required).
 */
class Mutator : public Validator
{
protected:
  bool  altm;    // shift
public:
  Mutator ()
    : Validator(), altm (false)
  {}
  Mutator (const Param& qualifierParameter)
    : Validator (qualifierParameter), altm (false)
  {}
  void alt (const bool torf)
  {
    altm = torf;
  }

  // accessors
  const bool alt () const
  {
    return altm;
  }
};

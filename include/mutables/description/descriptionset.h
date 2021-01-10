class DescriptionSet : public ParameterSet<Description>
{
protected:
  void set (const Param& type,
        const Param& displayName,
        const Param& description,
        const Param& defaultValue,
        const Param& defaultAccess)
  {
#if 0
    // this causes a problem because descriptions contain a pointer,
    // creating them willy-nilly like this means pointer-subjects can't be
    // deleted in the class destructor and must be managed elsewhere. avoid.
    Description  d;

    d.displayName = displayName;
    d.description = description;
    d.defaultValue = defaultValue;
    d.defaultValidator = defaultValidator;

    ParameterSet::set (type, d);
#else
    //(*this)[type].dispname = displayName;
    //(*this)[type].descrip = description;
    //(*this)[type].defval= defaultValue;
    //(*this)[type].vld = defaultValidator;

    (*this)[type].set (displayName, description, defaultValue, defaultAccess);
#endif
  }
public:
  DescriptionSet ()
  {}
};

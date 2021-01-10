/**
 * Description of a class containing display name and description for the user interface,
 * default values, acceptable values and the default validator.
 */
class Description : public Utils::CallbackSet
{
// FIXME: provide a generator function which produces a list of values for a mutable property.
//      How should discrete and continuous values be differentiated? all values are discrete.
public:
  // set of mutators defined for a type. i.e., "increment", "decrement", etc
  // FIXME: include validatation in here? its not a mutator, but...
  // FIXME: include a generator too?
  typedef IntegerSet  ValueList;

  static Param nextValue (const ValueList& values, const Param& ref)
  {
    ValueList::ConstParamList  bg = values.begin ();
    ValueList::ConstParamList  nd = values.end ();
    ValueList::ConstParamList  it = values.find (ref);//std::find (bg, nd, value ());

    if ((it == nd) || (++it == nd))
    {
      it = bg;
    }

    return it->first;
  }
  static Param previousValue (const ValueList& values, const Param& ref)
  {
    ValueList::ConstParamList  bg = values.begin ();
    ValueList::ConstParamList  nd = values.end ();
    ValueList::ConstParamList  it = values.find (ref); //std::find (bg, nd, value ());

    if (it == nd)
    {
      it = bg;
      std::advance (it, values.parameterCount ()-1);
    }
    else if (it == bg)
    {
      std::advance (it, std::distance (bg, nd)-1);
    }
    else
    {
      if (--it == nd)
      {
        it = bg;
      }
    }

    return it->first;
  }

protected:
  Param        dispname;  ///< nice name for the user
  Param        descrip;  ///< description for the user
  Param        defval;    ///< initial value
  ValueList    vals;    ///< list of discrete values accepted as input, second value is a resource index
  Param        defacc;    ///< initial accessibility type

public:
  Description ()
    : dispname (UNSET), descrip (UNSET), defval (UNSET), defacc (UNSET)
  {
#ifdef MUTABLES_VERBOSE
    Note::notemsgn ("Description::cnstctr01\0");
#endif
  }
  Description (const Param initialDisplayName, const Param initialDescription, const Param initialDefaultValue, const Param initialDefaultAccess)
    : dispname (initialDisplayName), descrip (initialDescription), defval (initialDefaultValue), defacc (initialDefaultAccess)
  {
#ifdef MUTABLES_VERBOSE
    Note::notemsgn ("Description::cnstctr02\0");
#endif
  }
  virtual ~Description ()    {}
  /**
   * appends a set of values to the value list of the type
   */
  template<class T> void appendValues (typename T::ConstParamList begin, typename T::ConstParamList end)
  {
    while (begin!=end)
    {
      values ()[begin->first] = begin->second;
      ++begin;
    }
  }
  template<class T> void appendValues (const T valueList)
  {
    appendValues<T> (valueList.begin (), valueList.end ());
  }
  /**
   * sets the set the of values for a type (clears any previous set)
   */
  template<class T> void setValues (typename T::ConstParamList begin, typename T::ConstParamList end)
  {
    values ().clear ();

    appendValues<T> (begin, end);
  }
  template<class T> void setValues (const T valueList)
  {
    setValues<T> (valueList.begin (), valueList.end ());
  }

  void setDefault (const Param& newDefaultValue)        {defval.assign (newDefaultValue);}
// ACCESSORS
  const Param& displayName () const       {return dispname;}
  const Param& description () const       {return descrip;}
  const Param& defaultValue () const      {return defval;}
  const Param& defaultAccess () const     {return defacc;}
  const ValueList& values () const        {return vals;}
  ValueList& values ()                    {return vals;}
// MUTATORS
  void set (const Param& displayName, const Param& description, const Param& defaultValue, const Param& defaultAccess)
  {
    dispname = displayName;
    descrip = description;
    defval = defaultValue;
    defacc = defaultAccess;
  }
  void set (const Param& mutatorName, Mutator* mutator)         {Utils::CallbackSet::set (mutatorName, mutator);}
  void set (const Param& drawerName, Drawer* drawer)            {Utils::CallbackSet::set (drawerName, drawer);}
  void set (const Param& validatorName, Validator* validator)   {Utils::CallbackSet::set (validatorName, validator);}
  void set (const Param& binderName, Binder* binder)            {Utils::CallbackSet::set (binderName, binder);}
};

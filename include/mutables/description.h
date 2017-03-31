namespace Mutables
{
/**
 * Base class to validate whether a Param value is a valid form for a given type.
 * Each validator should be associated with a type.
 * The validator is then associated with a property and queried each time a value is updated.
 * Default types have default validators.
 */
class Validator : public Utils::Callback
{
protected:
	Param	val;	// last test value
	Param	qwl;	// qualifier
	bool	res;	// last result

	void setResult (const bool newRes)		{res = newRes;}
public:
	Validator ()
		: val (UNSET), qwl (UNSET), res (false)
	{}
	Validator (const Param& qualifierParameter)
		: val (UNSET), qwl (qualifierParameter), res (false)
	{}
	// mutators
	void clear ()										{val.assign (UNSET);}
	void set (const Param& input)						{val.assign (input);}
	void set (const Param& input, const Param& param)	{val.assign (input); qwl.assign (param);}

	const Param& value () const							{return val;}
	const Param& qualifier () const						{return qwl;}
	void get (Param& out)								{out.assign (val); clear ();}

//	virtual bool operator() (const Param testValue) const = 0;
	bool valid () const									{return res;}
};
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
	bool	altm;		// shift
public:
	Mutator ()
		: Validator(), altm (false)
	{}
	Mutator (const Param& qualifierParameter)
		: Validator (qualifierParameter), altm (false)
	{}
	void alt (const bool torf)							{altm = torf;}

	// accessors
	const bool alt () const								{return altm;}
};
/**
 * Base class for a binding process
 */
class Binder : public Validator
{
protected:
	Utils::CallbackSet*	trg;

	Utils::CallbackSet& target () const	{return *trg;}
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
/**
 * Base class for a drawer of the type.
 * Takes a property as a parameter and draws it to the screen.
 */
class Drawer : public Utils::Callback
{
protected:
	Param		val;
	UI::Bounds	area;

	const Param& value () const				{return val;}
	const UI::Bounds& drawRegion () const	{return area;}
public:
	Drawer ()
		: val (UNSET)
	{}
	void set (const Param& newName)			{val = newName;}
	void set (const UI::Bounds& newBounds)	{area.setRegion (newBounds);}
};
/**
 * Description of a class containing display name and description for the user interface,
 * default values, acceptable values and the default validator.
 */
class Description : public Utils::CallbackSet
{
// FIXME: provide a generator function which produces a list of values for a mutable property.
//		  How should discrete and continuous values be differentiated? all values are discrete.
public:
	// set of mutators defined for a type. i.e., "increment", "decrement", etc
	// FIXME: include validatation in here? its not a mutator, but...
	// FIXME: include a generator too?
	typedef IntegerSet	ValueList;

	static Param nextValue (const ValueList& values, const Param& ref)
	{
		ValueList::ConstParamList	bg = values.begin ();
		ValueList::ConstParamList	nd = values.end ();
		ValueList::ConstParamList	it = values.find (ref);//std::find (bg, nd, value ());

		if ((it == nd) || (++it == nd))
		{
			it = bg;
		}

		return it->first;
	}
	static Param previousValue (const ValueList& values, const Param& ref)
	{
		ValueList::ConstParamList	bg = values.begin ();
		ValueList::ConstParamList	nd = values.end ();
		ValueList::ConstParamList	it = values.find (ref); //std::find (bg, nd, value ());

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
	Param				dispname;	///< nice name for the user
	Param				descrip;	///< description for the user
	Param				defval;		///< initial value
	ValueList			vals;		///< list of discrete values accepted as input, second value is a resource index
	Param				defacc;		///< initial accessibility type

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
	virtual ~Description ()		{}
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

	void setDefault (const Param& newDefaultValue)				{defval.assign (newDefaultValue);}
// ACCESSORS
	const Param& displayName () const							{return dispname;}
	const Param& description () const							{return descrip;}
	const Param& defaultValue () const							{return defval;}
	const Param& defaultAccess () const							{return defacc;}
	const ValueList& values () const							{return vals;}
	ValueList& values ()										{return vals;}
// MUTATORS
	void set (const Param& displayName, const Param& description, const Param& defaultValue, const Param& defaultAccess)
	{
		dispname = displayName;
		descrip = description;
		defval = defaultValue;
		defacc = defaultAccess;
	}
	void set (const Param& mutatorName, Mutator* mutator)		{Utils::CallbackSet::set (mutatorName, mutator);}
	void set (const Param& drawerName, Drawer* drawer)			{Utils::CallbackSet::set (drawerName, drawer);}
	void set (const Param& validatorName, Validator* validator)	{Utils::CallbackSet::set (validatorName, validator);}
	void set (const Param& binderName, Binder* binder)			{Utils::CallbackSet::set (binderName, binder);}
};

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
		Description	d;

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
	DescriptionSet ();
};

// global storage
// FIXME: make a singleton or something, we definitely want to call a function to access it
DescriptionSet	descriptionSet;

};

namespace Mutables
{
#ifndef PropertySet
class PropertySet;
#endif

/**
 * Properties are simply a type-value pair where value is interepretted according to type.
 * Value is always stored as a string encoding, with multiple elements separated by a hash.
 * Helper functions exist to process specific type-value combinations.
 * FIXME: In the description, input and output functors could be attached to
 *		  process particular types, meaning types would not be hardcoded and
 *		  would be more extensible.
 */
class Property : public Utils::Callback,
				 public Patterns::Touch
{
public:
	// UTILITY FUNCTIONS
	static float nearestMutableValue_down (float x)
	{
#ifdef MUTABLE_FLOAT_ROUNDING
		x *= std::pow (10.0f, MUTABLE_FLOAT_PRECISION);
		x = std::floor (x);
		x /= std::pow (10.0f, MUTABLE_FLOAT_PRECISION);
#endif
		return x;
	}
	static float nearestMutableValue_up (float x)
	{
#ifdef MUTABLE_FLOAT_ROUNDING
		x *= std::pow (10.0f, MUTABLE_FLOAT_PRECISION);
		x = std::ceil (x);
		x /= std::pow (10.0f, MUTABLE_FLOAT_PRECISION);
#endif
		return x;
	}

	static bool verifyStorage (const float x, const Param& p, const unsigned int index)
	{
	//	float	ts;
		Param	t ("\0");

		if (Token::get (p, index, t) == false)		{std::cerr << "Could not get () token" << std::endl; return false;}
/*
		if (getMin (ts) == false)	{Note::notemsg ("setMin () {GetMin}\0");}
		if (ts > newMin)			{Note::notemsg ("MIN FAIL\0");}
		*/
		return false;
	}
//public:
//	static DescriptionSet	descriptionSet;	///< mutable type library
//	static Repository		repositorySet;	///< mutable library			// FIXME: move both of these to Mutables namespace
private:
	///< name of the property is in the parameter set container
	Param				_value;		///< value of the property
	Param				_type;		///< type of the property
	Param				_access;	///< accessibilty rights of the property

	// returns the string representation used to store the value
	const std::string& value () const								{return _value;}
	bool setValue (const std::string& newValue)						{_value.assign (newValue); return touch ();}
	/**
	 * sets a given token of the property to a new value.
	 * Updates the touch marker (NB: this is the important part, DO NOT use Token::put)
	 * @param index, marked location of the property to update in the string.
	 * @param newValue, value to place into the marked location
	 * @returns true if static setToken modifies the string.
	 * FIXME: This does not call setValue to update the string, ALL updates should use setValue ().
	 */
	template<class T> bool setToken (const unsigned int index, const T newValue)
	{
		if (Token::put<T> (_value, index, newValue) == true)
		{
			touch ();
			return true;
		}

		return false;
	}
public:
	/**
	 * compares the hash value of value () against the stored touch change detector.
	 * Disjunction indicates the value has update, and the touch flag is set to true.
	 * @returns true if the hash codes disagree, false otherwise.
	 */
	bool touch ()													{return setTouchCD (_value.hash ());}
public:
	Property ()
		: _value (Types::unknown ()), _type (Types::unknown ()), _access (Types::unknown ())
	{
		touch ();
	}
	Property (const Param type)
		: _value (Types::unknown ()), _type (type), _access (Types::unknown ())
	{
		_value.assign (descriptionSet[type].defaultValue ());
		_access.assign (descriptionSet[type].defaultAccess ());
		touch ();
	}
	Property (const Property& p)
		: _value (p.value ()), _type (p._type), _access (p._access)
	{
		touch ();
	}
	Property (const Property *p)
		: _value (p->value ()), _type (p->_type), _access (p->_access)
	{
		touch ();
	}
// DESTRUCTORS
	~Property ()													{}
// OPERATORS
	Property& operator= (const Property& p)							{this->_type.assign (p._type);	setValue (p._value);	return (*this);}
	Property* operator= (const Property* p)							{this->_type.assign (p->_type);	setValue (p->_value);	return this;}
	bool operator==(const Property& p) const 						{return ((value () == p.value ()) && (type () == p.type ()));}
	bool operator!=(const Property& p) const						{return (!((value () == p.value ()) && (type () == p.type ())));}
	void operator() (void)											{std::cout << "property invoked" << std::endl;}
	/**
	 * Obtains the "defaultValidator" for this type and tests the input.
	 * @param testValue, value to validate against this properties type
	 * @returns true if the value represents the type, false otherwise
	 */
#if 0
// FIXME: restore this
	bool isValid (const Param testValue)
	{
		Validator	*v = NULL;

		if ((descriptionSet[type ()] && "defaultValidator\0") == false)		{return false;}

		v = dynamic_cast<Validator*>(&descriptionSet[type ()]["defaultValidator\0"]);

		if (v == NULL)														{return false;}

		// validate the input
		v->set (testValue);
		(*v) ();
		return v->valid ();
	}
#else
	bool isValid (const Param testValue)
	{
		return true;
	}
#endif
	/**
	 * @returns true if the value is the default value for its type, false otherwise
	 */
	bool isDefault () const											{return (value ().compare (descriptionSet[type ()].defaultValue ()) == 0);}
	/**
	 * CALLS THE DEFAULTDRAWER for a type
	 * FIXME: just have a "draw" callback? that would require properties to have callbacks.
	 * FIXME: doDraw () ? isDraw () ?
	 */
	void draw (const UI::Bounds& drawRegion) const
	{
		Drawer	*d;

#if 0
		float	cx[2];

		// get the center of the bounds
		drawRegion.center (cx[0], cx[1]);

		// write the label
		glPushMatrix ();
		glTranslatef (cx[0], cx[1], 1.0f);
		glScalef (30.0f, -30.0f, 1.0f);
		GLUtils::Text::glPrintf (str ().c_str ());
		glPopMatrix ();
#endif

		if ((descriptionSet[type ()] && "defaultDrawer\0") == false)		{return;}

		d = dynamic_cast<Drawer*>(&descriptionSet[type ()]["defaultDrawer\0"]);

		if (d == NULL)						{return;}

		d->set (value ());
		d->set (drawRegion);

		(*d) ();				// draw
	}
// ACCESSORS
	const Param& type () const										{return _type;}
	const Param& access () const									{return _access;}
	const std::string str () const									{return value ();}
// MUTATORS
	void setType (const Param newType)
	{
		_type = newType;

#ifdef _DEBUG
	//	if (contains (descriptionSet[Types::type ()].values (), newType) == false)
		if ((descriptionSet[Types::type ()].values () && newType) == false)
		{
			std::cout << "UNKNOWN type: '" << newType << "'" << std::endl;
		}
#endif
		setValue (descriptionSet[type ()].defaultValue ());
		setAccess (descriptionSet[type ()].defaultAccess ());
	}
	void setAccess (const Param newAccess)							{_access = newAccess;}
	void clear ()													{setValue (Types::bottom ());}
// STATICS
	static bool set (Param& propertyString, const float newValue)	{return Token::put (propertyString, 0, newValue);}
	/**
	 * sets a mutable value to a given string representation of that value.
	 * Note: no validation is performed.  The newValue is copied into the
	 * holding variable directly, therefore, the value should be validated
	 * before passing to this function.
	 * @param newValue string representation of the new value
	 * @returns true if the variable was set, false otherwise.
	 */
	bool set (const Param newValue)
	{
#if 0
		if ((descriptionSet[type ()] && "defaultValidator\0") == true)
		{
			Validator*	v;

			v = dynamic_cast<Validator*>(&descriptionSet[type ()]["defaultValidator\0"]);

			v->set (newValue);
			(*v) ();

			if (v->valid () == false)				{return false;}
		}
#endif
		// set the value
		if (type () == Types::unknown ())
		{
			std::cerr << "Unknown mutable type" << std::endl;
			return false;
		}
		else if (type () == Types::label ())		{return setToken (0, newValue);}
		else if (type () == Types::numeric ())		{return setToken (0, newValue);}
		else if (type () == Types::boolean ())		{return setToken (0, newValue);}
		else if (type () == Types::colour ())		{return setToken (0, newValue);}
		else if (type () == Types::colourMap ())	{return setToken (0, newValue);}
		else if (type () == Types::image ())		{return setToken (0, newValue);}
		else if (type () == Types::program ())		{return setToken (0, newValue);}
		else if (type () == Types::properties ())	{return setToken (0, newValue);}
		else if (type () == Types::function ())		{return setToken (0, newValue);}
		else if (type () == Types::attribute ())	{return setToken (0, newValue);}
		else if (type () == Types::plot ())			{return setToken (0, newValue);}
		else if (type () == Types::mesh ())			{return setToken (0, newValue);}
		else if (type () == Types::range ())
		{
			Param	mn ("?\0"), mx ("?\0");

			return (Token::get (newValue, 0, mn) &&
					Token::get (newValue, 1, mx) &&
					setToken (0, mn) && 
					setToken (1, mx));
		}
		else if (type () == Types::type ())			{return setToken (0, newValue);}
		else if (type () == Types::subType ())		{return setToken (0, newValue);}
		else if (type () == Types::glyph ())		{return setToken (0, newValue);}
		else if (type () == Types::dataSource ())	{return setToken (0, newValue);}
		else if (type () == Types::statSource ())	{return setToken (0, newValue);}
		else
		{
			std::cerr << "mutable type '" << type () << "' NIMPL" << std::endl;
			return false;
		}

		return false;
	}
	/**
	 * Sets the value as a float
	 * @param newValue new numeric value to set for the property
	 * @returns true if the value could be set, false otherwise (when the property is not numeric)
	 */
	bool set (const float newValue)									{return ((type () == Types::numeric ())	&& (setToken (0, newValue)));}
	bool set (const size_t newValue)								{return ((type () == Types::numeric ())	&& (set (static_cast<const float>(newValue))));}
	bool set (const unsigned int newValue)							{return ((type () == Types::numeric ()) && (set (static_cast<const float>(newValue))));}	// FIXME: alter precision? any benefits?
/*	bool set (const IntegerSetType newValue)
	{
		if ((type () == Types::image ()) ||
			(type () == Types::colourMap ()))
		{
			return set (static_cast<unsigned int>(newValue));
		}
		else if (type () == Types::numeric ())
		{
			return set (static_cast<float>(newValue));
		}

		return false;
	}*/
	bool set (const bool newValue)									{return ((type () == Types::boolean ()) && (newValue==true)) ? (set (Param ("true\0"))) : (set (Param ("false\0")));}
	/**
	 * Given four colour values in the range [0..1], encodes the values as HTML hex format
	 * in the value of the property.
	 * @param rgba array of four float values describing the red green and blue in the range [0..1]
	 * @return true if the value can be set, false otherwise (where the type () is not COLOUR)
	 */
#ifdef COLOUR_UTILS
	bool set (const float *rgba)									{return ((type () == Types::colour ()) && (set (ColourUtils::float2hex (rgba))));}
#endif
#ifdef _WIN32_
	bool set (COLORREF& ref)
	{
		float	rgba[4] = {static_cast<float>(GetRValue (ref))/255.0f,
						   static_cast<float>(GetGValue (ref))/255.0f,
						   static_cast<float>(GetBValue (ref))/255.0f,
						   1.0f};
		return set (rgba);	// handles (type () != COLOUR)
	}
#endif
	/**
	 * Range is a pair of floats separated by a hash symbol
	 */
	bool set (const float min, const float max)						{return ((type () == Types::range ()) && (setToken (0, min) && setToken (1, max)));}
	/**
	 * Bounds
	 */
	bool set (const float mnx, const float mny, const float mxx, const float mxy)
	{
		if (type () == Types::colour ())
		{
			float	rgba[4] = {mnx, mny, mxx, mxy};

			return set (rgba);
		}
		else if (type () == Types::bound ())
		{
			return (setToken (0, mnx) && setToken (1, mny) && setToken (2, mxx) && setToken (3, mxy));
		}

		return false;
	}
	bool set (const UI::Bounds& bnd)
	{
		return set (bnd.left (), bnd.top (), bnd.right (), bnd.bottom ());
	}
#ifdef GLUTILS_H
#if 0
	/**
	 * Shader Profile is a list of integers
	 */
	bool set (const GLUtils::ShaderProfile& shader)
	{
/*
		VERTEX_SHADER,
		GEOMETRY_SHADER,
		FRAGMENT_SHADER,
		SHADER_PROGRAM,		// NB: bad code references "shaderProfile[3]" sometimes to get the shader program id :(
		COMPUTE_SHADER,
		TESS_CTRL_SHADER,
		TESS_EVAL_SHADER
*/
#if 0
		if (setToken (GLUtils::VERTEX_SHADER, shader[GLUtils::VERTEX_SHADER]) == false)				{return false;}
		else if (setToken (GLUtils::GEOMETRY_SHADER, shader[GLUtils::GEOMETRY_SHADER]) == false)	{return false;}
		else if (setToken (GLUtils::FRAGMENT_SHADER, shader[GLUtils::FRAGMENT_SHADER]) == false)	{return false;}
		else if (setToken (GLUtils::SHADER_PROGRAM, shader[GLUtils::SHADER_PROGRAM]) == false)		{return false;}
		else if (setToken (GLUtils::COMPUTE_SHADER, shader[GLUtils::COMPUTE_SHADER]) == false)		{return false;}
#else
		if (type () == Types::program ())
		{
			return ((setToken (GLUtils::VERTEX_SHADER, shader[GLUtils::VERTEX_SHADER])) &&
					(setToken (GLUtils::GEOMETRY_SHADER, shader[GLUtils::GEOMETRY_SHADER])) &&
					(setToken (GLUtils::FRAGMENT_SHADER, shader[GLUtils::FRAGMENT_SHADER])) &&
					(setToken (GLUtils::SHADER_PROGRAM, shader[GLUtils::SHADER_PROGRAM])) &&
					(setToken (GLUtils::COMPUTE_SHADER, shader[GLUtils::COMPUTE_SHADER])));
		}
#endif
		return false;
	}
#endif
	bool set (const GLUtils::TextureDescription& texture)			{return ((type () == Types::image ()) && (setToken (0, texture.index ()) && setToken (1, texture.type ())));}
	bool set (const GLUtils::BufferDescription& buffer)				{return ((type () == Types::buffer ()) && (setToken (0, buffer.index ())));}
#endif
// LABEL
	const bool getLabel (Param& out, const unsigned int tokenIndex = 0) const
	{
#if 0
		if ((type () == Types::label ()) ||
			(type () == Types::attribute ()) ||
			(type () == Types::colourMap ()) ||
			(type () == Types::set ()) ||
			(type () == Types::type ()) ||
			(type () == Types::subType ()) ||
			(type () == Types::dataSource ()) ||
			(type () == Types::mesh ()))
		{
			return Token::get (value (), tokenIndex, out);
		}

		return false;
#else
		Param	ts[] = {Types::label (),
						Types::attribute (),
						Types::colourMap (),
						Types::program (),
						Types::type (),
						Types::properties (),
						Types::subType (),
						Types::dataSource (),
						Types::statSource (),
						Types::mesh ()};

		if (std::any_of (std::begin (ts), std::end (ts), [&] (Param& p) {return (p == type ());}) == true)
		{
			return Token::get (out, value (), tokenIndex);
		}

		return false;
#endif
	}
	const Param getLabel (const unsigned int tokenIndex = 0) const
	{
		Param	p ("");
		getLabel (p, tokenIndex);
		return p;
	}
// NUMERIC/UNIT
	template<typename T>
	static bool getNumber (T& out, const Param& p, const unsigned int tokenIndex = 0)
	{
		return Token::get (out, p, tokenIndex);
	}
	// FIXME: get absolute number, get number sign, get number fraction?
	bool getNumber (float& out, const unsigned int tokenIndex = 0) const
	{
		if ((type () == Types::numeric ()) ||
			(type () == Types::range ()) ||
			(type () == Types::bound ()))
		{
			return Token::get (out, value (), tokenIndex);
		}
		else if ((type () == Types::attribute ()) ||
				 (type () == Types::glyph ()) ||
				 (type () == Types::colourMap ()) ||
				 (type () == Types::type ()) ||
				 (type () == Types::boolean ()))
		{
		//	out = indexOf (descriptionSet[type ()].values (), value ());
			out = static_cast<float>(descriptionSet[type ()].values ()[value ()]);
			return true;
		}
		else
		{
			out = std::numeric_limits<float>::infinity ();
			return false;
		}
	}
	template<class T>
	bool getNumber (T& out, const unsigned int tokenIndex = 0) const
	{
		float	v;

		if (getNumber (v, tokenIndex) == false)		{return false;}

		out = static_cast<T>(v);
		return true;
	}
// BOOLEAN
	bool getBoolean (bool& out) const
	{
#if 1
		if (type () != Types::boolean ())			{return false;}

		// FIXME: compare to descriptionSet[BOOL].values ().front (), so we can use values other than "true" and "false"
		if (value ().compare ("true\0") == 0)
			out = true;
		else
			out = false;

		return true;
#else
		return get (out, value (), 0);
#endif
	}
// COLOUR
#ifdef COLOUR_UTILS
	void getColour (float *rgba) const
	{
		if (type () == Types::colour ())
		{
			ColourUtils::hex2float (rgba, value ());
		}
	}
	void getColour (float& red, float& green, float& blue, float& alpha) const
	{
		float	rgba[4];
		getColour (rgba);
		red = rgba[0];
		green = rgba[1];
		blue = rgba[2];
		alpha = rgba[3];
	}
#endif
#ifdef _WIN32_
	void getColour (COLORREF& ref) const
	{
		float	rgba[4];

		getColour (rgba);

		ref = RGB (static_cast<BYTE>(rgba[0] * 255.0f),
				   static_cast<BYTE>(rgba[1] * 255.0f),
				   static_cast<BYTE>(rgba[2] * 255.0f));
	}
#endif
	// input must be tokenised
#ifdef COLOUR_UTILS
	static void getColour (const Param& p, float *rgba)				{ColourUtils::hex2float (rgba, p);}
#endif
// IMAGE
#ifdef GLUTILS_H
	static bool getImage (GLUtils::TextureDescription& texture, const Param& p, const unsigned int tokenIndex = 0)
	{
		GLint	idx, typ;

		if ((Token::get (idx, p, tokenIndex) == false) || (Token::get (typ, p, tokenIndex+1) == false))
		{
			return false;
		}

		texture = GLUtils::TextureDescription (typ, idx);

		return true;
	}
	const bool getImage (GLUtils::TextureDescription& txd) const
	{
		if (type () == Types::image ())
		{
			return getImage (txd, value ());
		}
		else if (type () == Types::colourMap ())
		{
#if 1
			txd = GLUtils::TextureDescription (GL_TEXTURE_1D, static_cast<GLuint>(descriptionSet[type ()].values ()[value ()]));
#else
			if ((repositorySet && value ()) == false)		{return false;}

			PropertySet&	ps = repositorySet[value ()];

			txd = ps["Image\0"].getImage ();
#endif
			return true;
		}
		else
		{
			return false;
		}
	}
	const GLUtils::TextureDescription getImage () const
	{
		GLUtils::TextureDescription	txd;

		getImage (txd);

		return txd;
	}
// PROGRAM
#if 0
	static bool getProgram (GLUtils::ShaderProfile& outProfile, const Param& p)
	{
		return (
		getNumber (outProfile[GLUtils::VERTEX_SHADER], p, GLUtils::VERTEX_SHADER) &&
		getNumber (outProfile[GLUtils::GEOMETRY_SHADER], p, GLUtils::GEOMETRY_SHADER) &&
		getNumber (outProfile[GLUtils::FRAGMENT_SHADER], p, GLUtils::FRAGMENT_SHADER) &&
		getNumber (outProfile[GLUtils::SHADER_PROGRAM], p, GLUtils::SHADER_PROGRAM) &&
		getNumber (outProfile[GLUtils::COMPUTE_SHADER], p, GLUtils::COMPUTE_SHADER));
	}
	bool getProgram (GLUtils::ShaderProfile& outProfile) const
	{
		if (type () == Types::program ())
		{
			return getProgram (outProfile, value ());
		}

		return false;
	}
#endif
// BUFFER
	static bool getBuffer (GLUtils::BufferDescription& outBuffer, const Param& p, const unsigned int tokenIndex = 0)
	{
		GLint	idx;

		if (getNumber (idx, p, tokenIndex) == false)		{return false;}

		outBuffer = GLUtils::BufferDescription (idx);

		return true;
	}
	const bool getBuffer (GLUtils::BufferDescription& outBuffer) const
	{
		if (type () != Types::buffer ())					{return false;}

		return getBuffer (outBuffer, value (), 0);
	}
	const GLUtils::BufferDescription getBuffer () const
	{
		GLUtils::BufferDescription	b (0);

		getBuffer (b);

		return b;
	}
#endif
// PROPERTYSET

	static bool getPropertySet (PropertySet **ps, const Param& p, const unsigned int tokenIndex = 0)
	{
		Param	lbl ("\0");

		if (Token::get (lbl, p, tokenIndex) == false)
		{
			return false;
		}

#ifdef repositorySet
		if ((repositorySet && lbl) == false)
		{
			std::cout << "no property set for '" << lbl << "'" << std::endl;
#ifdef _DEBUG
			assert (0);
#endif
			return false;
		}

		*ps = &repositorySet[lbl];
#endif

		return true;
	}
	bool getPropertySet (PropertySet **ps) const					{return getPropertySet (ps, value ());}
	bool getPropertySet (PropertySet **ps)							{return getPropertySet (ps, value ());}
	PropertySet& ps ()												{PropertySet	*ps;	getPropertySet (&ps);	return (*ps);}
	const PropertySet& ps () const									{PropertySet	*ps;	getPropertySet (&ps);	return (*ps);}
// RANGE
	static bool getRange (float& min, float& max, const Param& p, const unsigned int tokenIndex = 0)
	{
		return (getNumber (min, p, tokenIndex+0) && getNumber (max, p, tokenIndex+1));
	}
	bool getRange (float& min, float& max) const					{return ((type () == Types::range ()) && (getRange (min, max, value (), 0)));}
	bool getRange (unsigned int& min, unsigned int& max) const
	{
		float	rng[2];

		if (getRange (rng[0], rng[1]) == true)
		{
			min = static_cast<unsigned int>(rng[0]);
			max = static_cast<unsigned int>(rng[1]);
			return true;
		}

		return false;
	}
	bool getMin (float& out) const									{return ((type () == Types::range ()) && (getNumber (out, 0)));}
	bool getMax (float& out) const									{return ((type () == Types::range ()) && (getNumber (out, 1)));}
	/**
	 * Stores the new float as a string value ROUNDING DOWN
	 * Small rounding errors in string encoding meant that the stored min value
	 * would be greater than the min value passed to the function.
	 * Rounding down to MUTABLE_FLOAT_PRECISION fixed this.
	 */
	bool setMin (const float newMin)
	{
		if (type () != Types::range ())		{return false;}

		setToken (0, nearestMutableValue_down (newMin));

//#ifdef _DEBUG
#if 0
		float	ts;
		if (getMin (ts) == false)	{Note::notemsg ("setMin () {GetMin}\0");}
		if (ts > newMin)			{Note::notemsg ("MIN FAIL\0");}
#endif
		return true;
	}
	/**
	 * Stores the new float as a string value ROUNDING UP
	 * Small rounding errors in string encoding meant that the stored max value
	 * would be less than the max value passed to the function.
	 * Rounding up to MUTABLE_FLOAT_PRECISION fixed this.
	 */
	bool setMax (const float newMax)
	{
		if (type () != Types::range ())		{return false;}

		setToken (1, nearestMutableValue_up (newMax));

//#ifdef _DEBUG
#if 0
		float	ts;

		if (getMax (ts) == false)	{Note::notemsg ("setMax () {GetMax}\0");}
		if (ts < newMax)			{Note::notemsg ("MAX FAIL\0");}
#endif
		return true;
	}
// BOUND
	bool getBound (UI::Bounds& b) const
	{
		if (type () == Types::bound ())
		{
			float	f[4];

			getNumber (f[0], 0);
			getNumber (f[1], 1);
			getNumber (f[2], 2);
			getNumber (f[3], 3);

			b.setRegion (f[0], f[1], f[2], f[3]);

			return true;
		}

		return false;
	}
// TYPE PARTICULARS
	const Param getAttribute () const								{return (type () == Types::attribute ())	? getLabel () : Types::bottom ();}
	const Param getFunction () const								{return (type () == Types::function ())		? getLabel () : Types::bottom ();}
	const Param getPlot () const									{return (type () == Types::plot ())			? getLabel () : Types::bottom ();}
	const Param getColourmap () const								{return (type () == Types::colourMap ())	? getLabel () : Types::bottom ();}
	const Param getType () const									{return (type () == Types::type ())			? getLabel () : Types::bottom ();}
	const Param getSubType () const									{return (type () == Types::subType ())		? getLabel () : Types::bottom ();}
	const Param getGlyph () const									{return (type () == Types::glyph ())		? getLabel () : Types::bottom ();}
#ifdef MUTABLE_LIST_TYPES
	// LISTS
	bool initialiseList (const unsigned int initialLength)
	{
		unsigned int	i;

		if ((type () != Types::labelList ()) &&
			(type () != Types::numberList ()))
		{
			return false;
		}

		clear ();
		setToken (0, static_cast<float>(initialLength));

		for (i=0;i<initialLength;i++)
		{
			appendToken (descriptionSet[type ()].defaultValue ());
		}

		return true;
	}
	const unsigned int getListLength () const
	{
		if ((type () != Types::labelList ()) &&
			(type () != Types::numberList ()))
		{
			return 0;
		}

		return static_cast<unsigned int>(getNumber (0));
	}
	bool setListLength (const unsigned int newLength)
	{
		if ((type () != Types::labelList ()) &&
			(type () != Types::numberList ()))
		{
			return false;
		}

		// FIXME: validate the number of items in the list by trimming?
#if 1
		setToken (0, static_cast<float>(newLength));
#else
		clear ();
		setToken (0, static_cast<float>(newLength));

		for (unsigned int i=0;i<newLength;i++)
		{
			appendToken (descriptionSet[type ()].defaultValue);
		}
#endif
		return true;
	}
// LABEL LIST
	// zero based indexing!!!
	template<class T>
	bool setListItem (const unsigned int index, const T newValue)
	{
		if ((type () != Types::labelList ()) &&
			(type () != Types::numberList ()))
		{
			return false;
		}

		// index 0 is the length of the list
//		if (index == 0)			{return false;}

		if (index >= getListLength ())	// append an item
		{
			appendToken (newValue);
		//	setToken (index, newValue);
		//	setToken (0, getListLength ()+1);
			setListLength (getListLength ()+1);
		}
		else
		{
			setToken<T> (index+1, newValue);
		}

		return true;
	}
	const Param getListItem (const unsigned int index) const
	{
		if ((type () != Types::labelList ()) &&
			(type () != Types::numberList ()))
		{
			return descriptionSet[type ()].defaultValue ();
		}
		// else
		return token (index+1);
	}
	template<class T> bool setListItems (T begin, T end)
	{
		unsigned int		i=0;

		while (begin!=end)
		{
			if (setListItem<typename T::value_type> (i++,*begin++) == false)
				return false;
		}
		return true;
	}
#ifdef _LIST_
	template<class T> const bool getListItems (std::list<T>& vs) const
	{
		unsigned int	i;
		unsigned int	N;

		N = getListLength ();

		for (i=0;i<N;i++)
		{
			vs.push_back (getListItem (i));
		}

		return true;
	}
	template<> const bool getListItems (std::list<float>& vs) const
	{
		unsigned int	i;
		unsigned int	N;

		N = getListLength ();

		for (i=0;i<N;i++)
		{
			vs.push_back (getNumber (i+1));
		}

		return true;
	}
#endif
#endif
// MUTATORS
	bool mutate (const Param& mutation)
	{
		Mutator	*m = NULL;

		if ((descriptionSet[type ()] && mutation) == false)			{return false;}

		m = dynamic_cast<Mutator*>(&descriptionSet[type ()][mutation]);

		if (m == NULL)												{return false;}

		m->alt (false);		/// never do an alternate at the moment

		// set the current value
		m->set (str ());

		// perform the update of the value
		(*m) ();

		// if the new value is different from the old, update it
		if (str () != m->value ())
		{
			// replace the old value with the new
			set (m->value ());
			return true;
		}
		else
		{
			// the update event did not change the value, so ignore it.
			return false;
		}

		return false;	// should not be reached...
	}
};

#if 0
// STREAM IO
	friend std::ostream& operator<< (std::ostream& os, const Mutables::Property& mp)
	{
	//	Patterns::Persistable::pushParameter<Property>(os, "type\0", mp.type ().c_str ());
	//	Patterns::Persistable::pushParameter<Property>(os, "value\0", mp.value ().c_str ());
		Patterns::Persistable::writeXMLAttributeValuePair (os, "type\0", mp.type ().c_str ());
		Patterns::Persistable::writeXMLAttributeValuePair (os, "value\0", mp.value ().c_str ());
		return os;
	}
	friend std::istream& operator>> (std::istream& is, Mutables::Property& mp)
	{
		std::string		t, tv, v, vv;

		Patterns::Persistable::fetchNextAttributeValuePair (is, t, tv);	// type
		Patterns::Persistable::consumeWhite (is);
		Patterns::Persistable::fetchNextAttributeValuePair (is, v, vv);	// value
		Patterns::Persistable::consumeWhite (is);

		// set the type
		if (mp.type () != Param (tv))
		{
			mp.setType (tv);
		}

		// set the value
		if (tv == Mutables::Types::image ())
		{

		}
		else
		{
			mp.set (vv);
		}

		// return
		return is;
	}
#endif
};

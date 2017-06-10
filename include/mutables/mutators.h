/**
 * MUTATORS
 */
class MUT_Numeric_Next : public Mutator
{
protected:
  const Param type;

public:
  static const float increment_large ()  {return 1.0f;}
  static const float increment_small ()  {return 0.01f;}
public:
  MUT_Numeric_Next (const Param typeTarget)
    : type (typeTarget)
  {}
  void operator() (void)
  {
    float  v;

    if (Property::getNumber (v, value ()) == false)    {return;}

    if (alt () == true)
    {
      v += increment_small ();
    }
    else
    {
      v += increment_large ();
    }

    Property::set (val, v);
  }
};
class MUT_Numeric_Prev : public Mutator
{
protected:
  const Param type;
public:
  MUT_Numeric_Prev (const Param typeTarget)
    : type (typeTarget)
  {}
  void operator() (void)
  {
    float v;

    if (Property::getNumber (v, value ()) == false)    {return;}

    if (alt () == true)
    {
      v -= MUT_Numeric_Next::increment_small ();
    }
    else
    {
      v -= MUT_Numeric_Next::increment_large ();
    }

    Property::set (val, v);
  }
};
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
class MUT_SubType_Next : public Mutator
{
protected:
  const Param type;
public:
  MUT_SubType_Next (const Param typeTarget)
    : type (typeTarget)
  {}
  void operator() (void)
  {
    const Param&  pType = SubTypes::parentType (value ());
    unsigned int  bg, nd, p;

    // find the first subtype of this type
    for (bg=0;  bg<SubTypes::count ();  bg++)   {if (SubTypes::parentType (SubTypes::byIndex (bg)) == pType)      {break;}}
    for (nd=bg;  nd<SubTypes::count ();  nd++)  {if (SubTypes::parentType (SubTypes::byIndex (nd)) != pType)      {break;}}

    if (bg == nd)    {return;}

    p = SubTypes::index (value ());

    if (p == nd-1)
    {
      p = bg;
    }
    else
    {
      if (++p == nd)
      {
        p = bg;
      }
    }

    set (SubTypes::byIndex (p));
  }
};
class MUT_SubType_Prev : public Mutator
{
protected:
  const Param type;
public:
  MUT_SubType_Prev (const Param typeTarget)
    : type (typeTarget)
  {}
  void operator() (void)
  {
    const Param&  pType = SubTypes::parentType (value ());
    unsigned int  bg, nd, p;

    // find the first subtype of this type
    for (bg=0;  bg<SubTypes::count ();  bg++)   {if (SubTypes::parentType (SubTypes::byIndex (bg)) == pType)      {break;}}
    for (nd=bg;  nd<SubTypes::count ();  nd++)  {if (SubTypes::parentType (SubTypes::byIndex (nd)) != pType)      {break;}}

    if (bg == nd)    {return;}

    p = SubTypes::index (value ());

    if (p == bg)
    {
      p = nd-1;
    }
    else
    {
      --p;
    }

    set (SubTypes::byIndex (p));
  }
};
class MUT_Colour_Next : public Mutator
{
protected:
  const Param type;
public:
  MUT_Colour_Next (const Param typeTarget)
    : type (typeTarget)
  {}
  void operator() (void)
  {
    float  rgba[4];
    float  rgb[3], hsv[3];

    Property::getColour (value (), rgba);
    rgb[0]=rgba[0];rgb[1]=rgba[1];rgb[2]=rgba[2];
    ColourUtils::rgb2hsv (rgb, hsv);

    if (alt () == true)    {hsv[1]+=0.01f;}
    else          {hsv[0]+=2.00f;}

    while (hsv[0]>360.0f)  {hsv[0]-=360.0f;}
    while (hsv[1]>1.0f)    {hsv[1]-=1.0f;}

    ColourUtils::hsv2rgb (hsv, rgb);
    rgba[0]=rgb[0];rgba[1]=rgb[1];rgba[2]=rgb[2];
    ColourUtils::float2hex (rgba, val);
  }
};
class MUT_Colour_Prev : public Mutator
{
protected:
  const Param type;
public:
  MUT_Colour_Prev (const Param typeTarget)
    : type (typeTarget)
  {}
  void operator() (void)
  {
    float  rgba[4];
    float  rgb[3], hsv[3];

    Property::getColour (value (), rgba);
    rgb[0]=rgba[0];rgb[1]=rgba[1];rgb[2]=rgba[2];
    ColourUtils::rgb2hsv (rgb, hsv);

    if (alt () == true)    {hsv[1]-=0.01f;}
    else          {hsv[0]-=2.00f;}

    while (hsv[0]<0.0f)    {hsv[0]+=360.0f;}
    while (hsv[1]<0.0f)    {hsv[1]+=1.0f;}

    ColourUtils::hsv2rgb (hsv, rgb);
    rgba[0]=rgb[0];rgba[1]=rgb[1];rgba[2]=rgb[2];
    ColourUtils::float2hex (rgba, val);
  }
};
class MUT_Unit_Next : public Mutator
{
public:
  MUT_Unit_Next ()
  {}
  void operator() (void)
  {
    Validator  *vld;
    float v, o;

    if (Property::getNumber (v, value ()) == false)    {return;}

    o = v;  // store the old value
    v += MUT_Numeric_Next::increment_small ();  // do the increment
    Property::set (val, v);  // update the stored value

    // validate the input
    vld = dynamic_cast<Validator*>(&descriptionSet[Types::numeric ()]["unitValue\0"]);

    if (vld == NULL)                  {return;}

    vld->set (value ());
    (*vld) ();

    if (vld->valid () == false)  // input is not valid
    {
      Property::set (val, o);  // restore the old value
    }
  }
};
class MUT_Unit_Prev : public Mutator
{
public:
  MUT_Unit_Prev ()
  {}
  void operator() (void)
  {
    Validator  *vld;
    float    v, o;

    if (Property::getNumber (v, value ()) == false)    {return;}

    o = v;
    v -= MUT_Numeric_Next::increment_small ();
    Property::set (val, v);

    // validate the input
    vld = dynamic_cast<Validator*>(&descriptionSet[Types::numeric ()]["unitValue\0"]);

    if (vld == NULL)                  {return;}

    vld->set (value ());
    (*vld) ();

    if (vld->valid () == false)
    {
      Property::set (val, o);
    }
  }
};

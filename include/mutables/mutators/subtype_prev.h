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
/**
 * Base class for a drawer of the type.
 * Takes a property as a parameter and draws it to the screen.
 */
class Drawer : public Utils::Callback
{
protected:
  Param    val;
  UI::Bounds  area;

  const Param& value () const             {return val;}
  const UI::Bounds& drawRegion () const   {return area;}
public:
  Drawer ()
    : val (UNSET)
  {}
  void set (const Param& newName)         {val = newName;}
  void set (const UI::Bounds& newBounds)  {area.setRegion (newBounds);}
};

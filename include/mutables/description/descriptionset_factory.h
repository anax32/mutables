/**
 * DESCRIPTION SET
 *  holds all the validators, nice names and descriptions for the mutable types.
 *  This is a parameter set indexed by the type.
 * FIXME: do not acces member variables of the descriptions.
 * FIXME: create proper validators for each type, ideally based on regular expressions
 *      which can parse the new input string.
 * FIXME: there is a really nasty circular dependency between this and the validators,
 */

/**
 * FIXME: This is here because the validators rely on a static instance
 * of a DescriptionSet.
 * We don't want it here, we don't want this dependency to exist.
 */
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
  DescriptionSet ();
};

// global storage
// FIXME: make a singleton or something, we definitely want to call a function to access it
DescriptionSet  descriptionSet;

DescriptionSet::DescriptionSet ()
{
#ifdef MUTABLES_VERBOSE
  Note::notemsgn ("Initialising types:\0");
#endif

#ifdef MUTABLES_VERBOSE
  Note::notemsgn ("[% 3i] %s\0", this->parameterCount (), Types::unknown ().c_str ());
#endif

  // FIXME: the first set creates a new description object, which is then copied over invoking the copy constructor on the underlying parameter set objects.
  //      this is no problem, but should not be necessary.
  (*this)[Types::unknown ()].set    ("Unknown\0", "Unknown\0",  UNSET,  AccessTypes::unknown ());
  (*this)[Types::unknown ()].set    ("defaultValidator\0",  new Mutables::VLD_Unknown ());
  (*this)[Types::unknown ()].set    ("defaultDrawer\0",     new Mutables::DRW_Unknown ());
  (*this)[Types::unknown ()].set    ("defaultBinder\0",     new Mutables::BND_Unknown ());

#ifdef MUTABLES_VERBOSE
  Note::notemsgn ("[% 3i] %s\0", this->parameterCount (), Types::bottom ().c_str ());
#endif
  (*this)[Types::bottom ()].set    ("Bottom\0", "Bottom value\0", UNSET,  AccessTypes::bottom ());
  (*this)[Types::bottom ()].set    ("defaultValidator\0",   new Mutables::VLD_Bottom ());
  (*this)[Types::bottom ()].set    ("defaultDrawer\0",      new Mutables::DRW_Unknown ());
  (*this)[Types::bottom ()].set    ("defaultBinder\0",      new Mutables::BND_Unknown ());

// basics
#ifdef MUTABLES_VERBOSE
  Note::notemsgn ("[% 3i] %s\0", this->parameterCount (), Types::label ().c_str ());
#endif
  (*this)[Types::label ()].set    ("Label\0", "Text description of data\0", UNSET,  AccessTypes::full ());
  (*this)[Types::label ()].set    ("defaultValidator\0",    new Mutables::VLD_Label ());
  (*this)[Types::label ()].set    ("defaultDrawer\0",       new Mutables::DRW_Unknown ());
  (*this)[Types::label ()].set    ("defaultBinder\0",       new Mutables::BND_Unknown ());

#ifdef MUTABLES_VERBOSE
  Note::notemsgn ("[% 3i] %s\0", this->parameterCount (), Types::numeric ().c_str ());
#endif
  (*this)[Types::numeric ()].set    ("Numeric\0", "A numeric value applied to data\0",  UNSET,  AccessTypes::full ());
  (*this)[Types::numeric ()].set    ("defaultValidator\0",  new Mutables::VLD_Numeric ());
  (*this)[Types::numeric ()].set    ("positiveValue\0",     new Mutables::VLD_Numeric_Pos ());
  (*this)[Types::numeric ()].set    ("negativeValue\0",     new Mutables::VLD_Numeric_Neg ());
  (*this)[Types::numeric ()].set    ("unitValue\0",         new Mutables::VLD_Numeric_Unit ());
  (*this)[Types::numeric ()].set    ("defaultDrawer\0",     new Mutables::DRW_Unknown ());
  (*this)[Types::numeric ()].set    ("defaultBinder\0",     new Mutables::BND_Unknown ());

#ifdef MUTABLES_VERBOSE
  Note::notemsgn ("[% 3i] %s\0", this->parameterCount (), Types::boolean ().c_str ());
#endif
  (*this)[Types::boolean ()].set    ("Option\0",  "True of false value\0",  UNSET,  AccessTypes::full ());
  (*this)[Types::boolean ()].set    ("defaultValidator\0",  new Mutables::VLD_ByList (Types::boolean ()));
  (*this)[Types::boolean ()].set    ("defaultDrawer\0",     new Mutables::DRW_Unknown ());
  (*this)[Types::boolean ()].set    ("defaultBinder\0",     new Mutables::BND_Unknown ());

#ifdef MUTABLES_VERBOSE
  Note::notemsgn ("[% 3i] %s\0", this->parameterCount (), Types::colour ().c_str ());
#endif
  (*this)[Types::colour ()].set    ("Colour\0", "Colour used to visually depict data\0",  "#?\0", AccessTypes::full ());
  (*this)[Types::colour ()].set    ("defaultValidator\0",   new Mutables::VLD_Colour ());
  (*this)[Types::colour ()].set    ("defaultDrawer\0",      new Mutables::DRW_Unknown ());
  (*this)[Types::colour ()].set    ("defaultBinder\0",      new Mutables::BND_Unknown ());

#ifdef MUTABLES_VERBOSE
  Note::notemsgn ("[% 3i] %s\0", this->parameterCount (), Types::image ().c_str ());
#endif
  (*this)[Types::image ()].set    ("Image\0", "Imagery\0",  "?@?\0",  AccessTypes::readonly ());
  (*this)[Types::image ()].set    ("defaultValidator\0",    new Mutables::VLD_Image ());
  (*this)[Types::image ()].set    ("defaultDrawer\0",       new Mutables::DRW_Image ());
  (*this)[Types::image ()].set    ("defaultBinder\0",       new Mutables::BND_Unknown ());

#if 0
#ifdef MUTABLES_VERBOSE
  Note::notemsg ("[% 3i] %s\0", this->parameterCount (), Types::program ().c_str ());
#endif
  (*this)[Types::program ()].set    ("Program\0",      "A shader program\0",            "?@?@?@?@?@?@?\0",  AccessTypes::hidden ());
  (*this)[Types::program ()].set    ("defaultValidator\0",  new VLD_Unknown ());    // FIXME: lets validate programs in here
  (*this)[Types::program ()].set    ("defaultDrawer\0",    new DRW_Unknown ());    // and do the processing in here!!!
  (*this)[Types::program ()].set    ("defaultBinder\0",    new BND_Unknown ());    // FIXME: lets do something here for when programs are loaded!
#else
#ifdef MUTABLES_VERBOSE
  Note::notemsgn ("[% 3i] %s\0", this->parameterCount (), Types::program ().c_str ());
#endif
  (*this)[Types::program ()].set    ("Program\0", "A shader program\0", "?\0",  AccessTypes::hidden ());
  (*this)[Types::program ()].set    ("defaultValidator\0",  new Mutables::VLD_ByList (Types::program ()));
  (*this)[Types::program ()].set    ("defaultDrawer\0",     new Mutables::DRW_Unknown ());
  (*this)[Types::program ()].set    ("defaultBinder\0",     new Mutables::BND_Unknown ());
#endif

#ifdef MUTABLES_VERBOSE
  Note::notemsgn ("[% 3i] %s\0", this->parameterCount (), Types::buffer ().c_str ());
#endif
  (*this)[Types::buffer ()].set    ("Buffer\0", "A data buffer\0",  "?\0",  AccessTypes::readonly ());
  (*this)[Types::buffer ()].set    ("defaultValidator\0",   new Mutables::VLD_Buffer ());
  (*this)[Types::buffer ()].set    ("defaultDrawer\0",      new Mutables::DRW_Unknown ());
  (*this)[Types::buffer ()].set    ("defaultBinder\0",      new Mutables::BND_Unknown ());

#ifdef MUTABLES_VERBOSE
  Note::notemsgn ("[% 3i] %s\0", this->parameterCount (), Types::properties ().c_str ());
#endif
  (*this)[Types::properties ()].set  ("Property Set\0", "A set of properties\0",  "?\0",  AccessTypes::full ());
  (*this)[Types::properties ()].set  ("defaultValidator\0", new Mutables::VLD_ByList (Types::properties ()));
  (*this)[Types::properties ()].set  ("defaultDrawer\0",    new Mutables::DRW_Unknown ());    // FIXME: default drawer is the mutables::propertyset::draw functor?
  (*this)[Types::properties ()].set  ("defaultBinder\0",    new Mutables::BND_Unknown ());

#ifdef MUTABLES_VERBOSE
  Note::notemsgn ("[% 3i] %s\0", this->parameterCount (), Types::type ().c_str ());
#endif
  (*this)[Types::type ()].set      ("Type\0", "A type of displayable object\0", UNSET,  AccessTypes::hidden ());
  (*this)[Types::type ()].set      ("defaultValidator\0",   new Mutables::VLD_ByList (Types::type ()));
  (*this)[Types::type ()].set      ("defaultDrawer\0",      new Mutables::DRW_Unknown ());
  (*this)[Types::type ()].set      ("defaultBinder\0",      new Mutables::BND_Unknown ());

#ifdef MUTABLES_VERBOSE
  Note::notemsgn ("[% 3i] %s\0", this->parameterCount (), Types::subType ().c_str ());
#endif
  (*this)[Types::subType ()].set    ("SubType\0", "A minor variation\0",  UNSET,  AccessTypes::hidden ());
  (*this)[Types::subType ()].set    ("defaultValidator\0",  new Mutables::VLD_ByList (Types::subType ()));
  (*this)[Types::subType ()].set    ("defaultDrawer\0",     new Mutables::DRW_Unknown ());
  (*this)[Types::subType ()].set    ("defaultBinder\0",     new Mutables::BND_Unknown ());
/*
// compositions
#ifdef MUTABLES_VERBOSE
  Note::notemsg ("[% 3i] Unit\0", this->parameterCount ());
#endif
  (*this)[Types::unit ()].set      ("Interval\0",      "An interval in the range [0..1]\0",    UNSET,      AccessTypes::full ());
  (*this)[Types::unit ()].set      ("defaultValidator\0",  new VLD_Unit ());
  (*this)[Types::unit ()].set      ("defaultDrawer\0",    new DRW_Unknown ());
*/
#ifdef MUTABLES_VERBOSE
  Note::notemsgn ("[% 3i] %s\0", this->parameterCount (), Types::colourMap ().c_str ());
#endif
  (*this)[Types::colourMap ()].set  ("Colour map\0",  "A colour mapping scale\0", "?\0",  AccessTypes::full ());
  (*this)[Types::colourMap ()].set  ("defaultValidator\0",  new Mutables::VLD_ByList (Types::colourMap ()));
  (*this)[Types::colourMap ()].set  ("defaultDrawer\0",     new Mutables::DRW_ColourMap ());
  (*this)[Types::colourMap ()].set  ("defaultBinder\0",     new Mutables::BND_Unknown ());

#ifdef MUTABLES_VERBOSE
  Note::notemsgn ("[% 3i] %s\0", this->parameterCount (), Types::function ().c_str ());
#endif
  (*this)[Types::function ()].set    ("Function\0", "A named function\0", UNSET,  AccessTypes::full ());
  (*this)[Types::function ()].set    ("defaultValidator\0", new Mutables::VLD_ByList (Types::function ()));
  (*this)[Types::function ()].set    ("defaultDrawer\0",    new Mutables::DRW_Unknown ());
  (*this)[Types::function ()].set    ("defaultBinder\0",    new Mutables::BND_Unknown ());    // RESET LATER

// FIXME: replace the defaultDrawer with one that takes a statistics object as a constructor parameter, so all attributes
// will be drawn the same way as the attributes are drawn in the DataSourceControl.
#ifdef MUTABLES_VERBOSE
  Note::notemsgn ("[% 3i] %s\0", this->parameterCount (), Types::attribute ().c_str ());
#endif
  (*this)[Types::attribute ()].set  ("Data Attribute\0",  "An attribute from the data source\0",  "::index\0",  AccessTypes::full ());
//  (*this)[Types::attribute ()].set  ("defaultValidator\0",  new VLD_ByList (Types::attribute ()));
  (*this)[Types::attribute ()].set  ("defaultValidator\0",  new Mutables::VLD_ByListCaseless (Types::attribute ()));
  (*this)[Types::attribute ()].set  ("defaultDrawer\0",     new Mutables::DRW_Attribute ());
  (*this)[Types::attribute ()].set  ("defaultBinder\0",     new Mutables::BND_Unknown ());

#ifdef MUTABLES_VERBOSE
  Note::notemsgn ("[% 3i] %s\0", this->parameterCount (), Types::plot ().c_str ());
#endif
  (*this)[Types::plot ()].set      ("Plot\0", "A data plot\0",  "::none\0", AccessTypes::full ());
  (*this)[Types::plot ()].set      ("defaultValidator\0",   new Mutables::VLD_ByList (Types::plot ()));
  (*this)[Types::plot ()].set      ("defaultDrawer\0",      new Mutables::DRW_Unknown ());
  (*this)[Types::plot ()].set      ("defaultBinder\0",      new Mutables::BND_Unknown ());    // RESET LATER

#ifdef MUTABLES_VERBOSE
  Note::notemsgn ("[% 3i] %s\0", this->parameterCount (), Types::mesh ().c_str ());
#endif
  (*this)[Types::mesh ()].set      ("Mesh\0", "A mesh\0", UNSET,  AccessTypes::full ());
  (*this)[Types::mesh ()].set      ("defaultValidator\0",   new Mutables::VLD_ByList (Types::mesh ()));
  (*this)[Types::mesh ()].set      ("defaultDrawer\0",      new Mutables::DRW_Unknown ());
  (*this)[Types::mesh ()].set      ("defaultBinder\0",      new Mutables::BND_Unknown ());

#ifdef MUTABLES_VERBOSE
  Note::notemsgn ("[% 3i] %s\0", this->parameterCount (), Types::range ().c_str ());
#endif
  (*this)[Types::range ()].set    ("Range\0", "A minimum and maximum\0",  "?@?\0",  AccessTypes::full ());
  (*this)[Types::range ()].set    ("defaultValidator\0",    new Mutables::VLD_Range ());
  (*this)[Types::range ()].set    ("defaultDrawer\0",       new Mutables::DRW_Range ());
  (*this)[Types::range ()].set    ("defaultBinder\0",       new Mutables::BND_Unknown ());

#ifdef MUTABLES_VERBOSE
  Note::notemsgn ("[% 3i] %s\0", this->parameterCount (), Types::bound ().c_str ());
#endif
  (*this)[Types::bound ()].set    ("Bound\0", "A 2D bound region\0",  "?@?@?@?\0",  AccessTypes::full ());
  (*this)[Types::bound ()].set    ("defaultValidator\0",    new Mutables::VLD_Unknown ());
  (*this)[Types::bound ()].set    ("defaultDrawer\0",       new Mutables::DRW_Bound ());
  (*this)[Types::bound ()].set    ("defaultBinder\0",       new Mutables::BND_Unknown ());

#ifdef MUTABLES_VERBOSE
  Note::notemsgn ("[% 3i] %s\0", this->parameterCount (), Types::glyph ().c_str ());
#endif
  (*this)[Types::glyph ()].set    ("Glyph\0", "Style of point representation\0",  UNSET,  AccessTypes::full ());
  (*this)[Types::glyph ()].set    ("defaultValidator\0",    new Mutables::VLD_ByList (Types::glyph ()));
  (*this)[Types::glyph ()].set    ("defaultDrawer\0",       new Mutables::DRW_Unknown ());
  (*this)[Types::glyph ()].set    ("defaultBinder\0",       new Mutables::BND_Unknown ());

#ifdef MUTABLES_VERBOSE
  Note::notemsgn ("[% 3i] %s\0", this->parameterCount (), Types::dataSource ().c_str ());
#endif
  (*this)[Types::dataSource ()].set  ("DataSource\0", "A set of data buffers\0",  UNSET,  AccessTypes::full ());
  (*this)[Types::dataSource ()].set  ("defaultValidator\0", new Mutables::VLD_ByList (Types::dataSource ()));
//  (*this)[Types::dataSource ()].set  ("defaultDrawer\0",  new Mutables::DRW_Unknown ());
  (*this)[Types::dataSource ()].set  ("defaultDrawer\0",    new Mutables::DRW_DataSource ());
  (*this)[Types::dataSource ()].set  ("defaultBinder\0",    new Mutables::BND_Unknown ());  // SET LATER

#ifdef MUTABLES_VERBOSE
  Note::notemsgn ("[% 3i] %s\0", this->parameterCount (), Types::statSource ().c_str ());
#endif
  (*this)[Types::statSource ()].set  ("StatSource\0", "A set of statistical descriptions\0",  UNSET,  AccessTypes::full ());
  (*this)[Types::statSource ()].set  ("defaultValidator\0", new Mutables::VLD_ByList (Types::statSource ()));
  (*this)[Types::statSource ()].set  ("defaultDrawer\0",    new Mutables::DRW_Unknown ());  // FIXME: draw stat sets like data sources!
  (*this)[Types::statSource ()].set  ("defaultBinder\0",    new Mutables::BND_Unknown ());  

// VALUES
  unsigned int  i;

  for (i=0;i<Types::count (); i++)  {(*this)[Types::type ()].values ().push_back (Types::byIndex (i));}

  for (i=0;i<SubTypes::count (); i++)  {(*this)[Types::subType ()].values ().push_back (SubTypes::byIndex (i));}

  // FIXME: provide a mapping from string representation to internal rep. for ALL types
  (*this)[Types::boolean ()].values ().push_back ("false\0");
  (*this)[Types::boolean ()].values ().push_back ("true\0");

  (*this)[Types::plot ()].values ().push_back ((*this)[Types::plot ()].defaultValue ());

  (*this)[Types::glyph ()].values ().push_back (SubTypes::point ());
  (*this)[Types::glyph ()].values ().push_back (SubTypes::triangle ());
  (*this)[Types::glyph ()].values ().push_back (SubTypes::square ());
  (*this)[Types::glyph ()].values ().push_back (SubTypes::circle ());
  (*this)[Types::glyph ()].values ().push_back (SubTypes::star ());

  (*this)[Types::glyph ()].setDefault (SubTypes::point ());

  // HACK
  (*this)[Types::statSource ()].values ().push_back ("::data\0");
//  (*this)[Types::statSource ()].values ().push_back ("::user\0");
  (*this)[Types::statSource ()].setDefault ("::data\0");

// MUTATORS
  (*this)[Types::numeric ()].set    ("next\0", new Mutables::MUT_Numeric_Next (Types::numeric ()));
  (*this)[Types::numeric ()].set    ("prev\0", new Mutables::MUT_Numeric_Prev (Types::numeric ()));
  (*this)[Types::boolean ()].set    ("next\0", new Mutables::MUT_ValueSet_Next (Types::boolean ()));
  (*this)[Types::boolean ()].set    ("prev\0", new Mutables::MUT_ValueSet_Prev (Types::boolean ()));
//  (*this)[Types::unit ()].set      ("next\0", new Mutables::MUT_Unit_Next ());
//  (*this)[Types::unit ()].set      ("prev\0", new Mutables::MUT_Unit_Prev ());
  (*this)[Types::colourMap ()].set  ("next\0", new Mutables::MUT_ValueSet_Next (Types::colourMap ()));
  (*this)[Types::colourMap ()].set  ("prev\0", new Mutables::MUT_ValueSet_Prev (Types::colourMap ()));
  (*this)[Types::function ()].set   ("next\0", new Mutables::MUT_ValueSet_Next (Types::function ()));
  (*this)[Types::function ()].set   ("prev\0", new Mutables::MUT_ValueSet_Prev (Types::function ()));
//  (*this)[Types::program ()].set    ("next\0", new Mutables::MUT_ValueSet_Next (Types::program ()));
//  (*this)[Types::program ()].set    ("prev\0", new Mutables::MUT_ValueSet_Prev (Types::program ()));
  (*this)[Types::attribute ()].set  ("next\0", new Mutables::MUT_ValueSet_Next (Types::bottom ()));
  (*this)[Types::attribute ()].set  ("prev\0", new Mutables::MUT_ValueSet_Prev (Types::bottom ()));
  (*this)[Types::plot ()].set       ("next\0", new Mutables::MUT_ValueSet_Next (Types::plot ()));
  (*this)[Types::plot ()].set       ("prev\0", new Mutables::MUT_ValueSet_Prev (Types::plot ()));
  (*this)[Types::mesh ()].set       ("next\0", new Mutables::MUT_ValueSet_Next (Types::mesh ()));
  (*this)[Types::mesh ()].set       ("prev\0", new Mutables::MUT_ValueSet_Prev (Types::mesh ()));
  (*this)[Types::colour ()].set     ("next\0", new Mutables::MUT_Colour_Next (Types::colour ()));
  (*this)[Types::colour ()].set     ("prev\0", new Mutables::MUT_Colour_Prev (Types::colour ()));
  (*this)[Types::glyph ()].set      ("next\0", new Mutables::MUT_ValueSet_Next (Types::glyph ()));
  (*this)[Types::glyph ()].set      ("prev\0", new Mutables::MUT_ValueSet_Prev (Types::glyph ()));
  (*this)[Types::subType ()].set    ("next\0", new Mutables::MUT_SubType_Next (Types::subType ()));
  (*this)[Types::subType ()].set    ("prev\0", new Mutables::MUT_SubType_Prev (Types::subType ()));
  (*this)[Types::dataSource ()].set ("next\0", new Mutables::MUT_ValueSet_Next (Types::dataSource ()));
  (*this)[Types::dataSource ()].set ("prev\0", new Mutables::MUT_ValueSet_Prev (Types::dataSource ()));
  (*this)[Types::statSource ()].set ("next\0", new Mutables::MUT_ValueSet_Next (Types::statSource ()));
  (*this)[Types::statSource ()].set ("prev\0", new Mutables::MUT_ValueSet_Prev (Types::statSource ()));
}

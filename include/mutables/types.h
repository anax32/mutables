/**
 * Basic properties of mutables.
 *  Always string representation.
 */
static std::locale  languageLocale;  ///< Locale for some string functions

const Param  UNSET ("?\0");
/**
 * String representations of the mutable types
 */
const Param TypeNames[] = 
{
  "UNKNOWN\0",    // default
  UNSET,        // bottom

// BASIC TYPES
  "LABEL\0",      // text value
  "NUMERIC\0",    // numeric (float) value      // stored as string representation
  "BOOLEAN\0",    // boolean value          // stored as strings ("true" or "false")
  "COLOUR\0",      // #hex                // colour value, stored as #<red><green><blue> 2 char hex values
  "IMAGE\0",      // integer              // opengl texture index
  "PROGRAM\0",    // label              // opengl shader program from the list of loaded programs (FIXME: move to composed types)
  "BUFFER\0",      // integer              // opengl data buffer

// COMPOSED TYPES with special operators
  "SET\0",      // LABEL              // the name of a contained set (allows hierarchies of property sets)
  "TYPE\0",      // LABEL              // basically, "this"
  "COLOUR_MAP\0",    // IMAGE              // colour map (1D-texture)
  "FUNCTION\0",    // LABEL              // function name; the mutableset should contain name-type-value parameters for the function
  "DATA_ATTRIBUTE\0",  // LABEL              // data attribute name: <string-from-set>
  "PLOT\0",      // LABEL              // plot name: <string-from-set>
  "MESH\0",      // LABEL              // mesh name: <string-from-set>
  "RANGE\0",      // NUMERIC#NUMERIC          // pair of values denoting a range: <min-value>#<max-value>
  "BOUND\0",      // NUMERIC#NUMERIC#NUMERIC#NUMERIC  // quad of values denoting a 2D bound
//  "LABEL_LIST\0",    // NUMERIC#LABEL#...#LABEL      // list of labels: <number of labels in list><label 1><label 2>...<label n>
//  "NUMBER_LIST\0",  // NUMERIC#NUMERIC#...#NUMERIC    // list of numbers: <number of numbers in list><number 1><number 2>...<number n>
  "SUBTYPE\0",    // LABEL              // subtype for an element, should come from the subtype list
  "GLYPH\0",      // NUMERIC              // point representation
  "DATASOURCE\0",    // LABEL LIST            // one of the items in the data list
  "STATSOURCE\0"    // LABEL LIST            // one of the items in the stat list

  // FIXME: some types (colourmap, function, plot, datasource) are actually compositions
  //      of lots of types, i.e., mutable property sets. Reflect this somehow.
};
const Param SubTypeNames[] =
{
  "UNKNOWN\0",    // default
  UNSET,        // bottom
// plots
  "lineplot\0",    //2
  "scatterplot\0",
  "crystal ball\0",
  "point cloud\0",
  "pcp\0",
  "raycaster\0",
  "dataset viewer\0",
  "mesh viewer\0",
// glyphs
  "point\0",      // 10
  "triangle\0",
  "square\0",
  "circle\0",
  "star\0",
// colourmaps
  "solid\0",      // 15
  "rising\0",
  "centered\0",
  "rainbow\0"
};
const Param AccessTypeNames[] =
{
  "UNKNOWN\0",    // default
  UNSET,        // bottom
// misc
  "open\0",      // can be viewed and changed
  "readonly\0",    // can be viewed but not changed
  "hidden\0"      // can not be viewed and can not be changed
};
/**
 * A utility class to return string names for types (as functions),
 * the number of types and an index into the list to enumerate types.
 */
class Types
{
public:
  static const Param& unknown ()        {return TypeNames[0];}    // invalid query value
  static const Param& bottom ()        {return TypeNames[1];}    // unset storage marker
// BASICS
  static const Param& label ()        {return TypeNames[2];}
  static const Param& numeric ()        {return TypeNames[3];}
  static const Param& boolean ()        {return TypeNames[4];}
  static const Param& colour ()        {return TypeNames[5];}
  static const Param& image ()        {return TypeNames[6];}    // gl texture identifier
  static const Param& program ()        {return TypeNames[7];}    // shader program
  static const Param& buffer ()        {return TypeNames[8];}    // gl buffer identifier
// COMPOSED
  static const Param& properties ()      {return TypeNames[9];}
  static const Param& type ()          {return TypeNames[10];}
  static const Param& colourMap ()      {return TypeNames[11];}
  static const Param& function ()        {return TypeNames[12];}
  static const Param& attribute ()      {return TypeNames[13];}
  static const Param& plot ()          {return TypeNames[14];}
  static const Param& mesh ()          {return TypeNames[15];}
  static const Param& range ()        {return TypeNames[16];}
  static const Param& bound ()        {return TypeNames[17];}
  static const Param& subType ()        {return TypeNames[18];}
  static const Param& glyph ()        {return TypeNames[19];}
  static const Param& dataSource ()      {return TypeNames[20];}
  static const Param&  statSource ()      {return TypeNames[21];}
// INFO
  static unsigned int count ()        {return 22;}
  static unsigned int index (const Param& t)
  {
    unsigned int  i;

    for (i=0;i<count ();i++)        {if (t == TypeNames[i]) {return i;}}

    return 0;  // return index to "unknown"
  }
  static const Param& byIndex (const unsigned int idx)
  {
    if ((idx > 0) && (idx < count ()))    {return TypeNames[idx];}
    else                  {return TypeNames[0];}
  }
};
/**
 * subtypes of brushes, plots and functions (i.e., the things the user uses).
 * FIXME: these should be entries in the correct type, e.g. type=PLOT, value=scatterplot.
 *    but if we do that we can't use subtype as a lookup. Or can we? argh, overcomp
 */
class SubTypes
{
public:
  static const Param& unknown ()        {return SubTypeNames[0];}
  static const Param& bottom ()        {return SubTypeNames[1];}
  static const Param& lineplot ()        {return SubTypeNames[2];}
  static const Param& scatterplot ()      {return SubTypeNames[3];}
  static const Param& crystalBall ()      {return SubTypeNames[4];}
  static const Param& pointCloud ()      {return SubTypeNames[5];}
  static const Param& PCP ()          {return SubTypeNames[6];}
  static const Param& rayCaster ()      {return SubTypeNames[7];}
  static const Param& datasetViewer ()    {return SubTypeNames[8];}
  static const Param& meshsetViewer ()    {return SubTypeNames[9];}
  static const Param& point ()        {return SubTypeNames[10];}
  static const Param& triangle ()        {return SubTypeNames[11];}
  static const Param& square ()        {return SubTypeNames[12];}
  static const Param& circle ()        {return SubTypeNames[13];}
  static const Param& star ()          {return SubTypeNames[14];}
  static const Param& solid ()        {return SubTypeNames[15];}
  static const Param& rising ()        {return SubTypeNames[16];}
  static const Param& centered ()        {return SubTypeNames[17];}
  static const Param& rainbow ()        {return SubTypeNames[18];}

  static unsigned int count ()        {return 19;}
  static unsigned int index (const Param& t)
  {
    unsigned int  i;

    for (i=0;i<count (); i++)        {if (t == SubTypeNames[i])  {return i;}  }

    return 0;
  }
  static const Param& byIndex (const unsigned int idx)
  {
    if ((idx > 0) && (idx < count ()))    {return SubTypeNames[idx];}
    else                  {return SubTypeNames[0];}
  }
  static const Param& parentType (const Param& subType)
  {
    if (subType == SubTypes::unknown ())        {return Types::unknown ();}
    else if (subType == SubTypes::bottom ())      {return Types::bottom ();}
    else if ((subType == SubTypes::lineplot ())  ||
           (subType == SubTypes::scatterplot ()) ||
         (subType == SubTypes::crystalBall ()) ||
         (subType == SubTypes::pointCloud ()) ||
         (subType == SubTypes::PCP ()) ||
         (subType == SubTypes::rayCaster ()) ||
         (subType == SubTypes::datasetViewer ()) ||
         (subType == SubTypes::meshsetViewer ()))  {return Types::plot ();}
    else if ((subType == SubTypes::point ()) ||
         (subType == SubTypes::triangle ()) ||
         (subType == SubTypes::square ()) ||
         (subType == SubTypes::circle ()) ||
         (subType == SubTypes::star ()))      {return Types::glyph ();}
    else if ((subType == SubTypes::solid ()) ||
         (subType == SubTypes::rising ()) ||
         (subType == SubTypes::centered ()) ||
         (subType == SubTypes::rainbow ()))      {return Types::colourMap ();}
    else                        {return Types::unknown ();}
  }
};
/**
 * things which control access to variables.
 * If unset, it is assumed to be open to all.
 */
class AccessTypes
{
public:
  static const Param& unknown ()        {return AccessTypeNames[0];}
  static const Param& bottom ()        {return AccessTypeNames[1];}
  static const Param& full ()          {return AccessTypeNames[2];}
  static const Param& readonly ()        {return AccessTypeNames[3];}
  static const Param& hidden ()        {return AccessTypeNames[4];}

  static unsigned int count ()        {return 5;}
  static unsigned int index (const Param& t)
  {
    unsigned int  i;

    for (i=0;i<count ();i++)        {if (t==AccessTypeNames[i])  {return i;}}
    return 0;
  }
  static const Param& byIndex (const unsigned int idx)
  {
    if ((idx > 0) && (idx < count ()))    {return AccessTypeNames[idx];}
    else                  {return AccessTypeNames[0];}
  }
};

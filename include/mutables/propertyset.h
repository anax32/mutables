#include <locale>
#include <algorithm>  // any_of

#if _DEBUG
#define MUTABLES_LIST_BOUNDS
#define MUTABLES_LIST_PROPERTIES
#define MUTABLE_FULL_LIST
//#undef MUTABLE_FULL_LIST

#include <assert.h>
#else
#define MUTABLES_LIST_BOUNDS
#define MUTABLES_LIST_PROPERTIES
#undef MUTABLE_FULL_LIST
#endif

#ifndef MUTABLES_H
#define MUTABLES_H
#endif

#include "description.h"

#define TOK_FNCS

#ifndef _WIN32_
// keycodes for linux
#include <ncurses.h>

#define VK_ESCAPE    KEY_CANCEL
#define VK_UP      KEY_UP
#define VK_DOWN      KEY_DOWN
#define VK_LEFT      KEY_LEFT
#define VK_RIGHT    KEY_RIGHT
#define VK_SPACE    KEY_BACKSPACE
#define VK_SEPARATOR  '-'
#define VK_RETURN    KEY_ENTER
#define VK_NUMPAD0    '0'
#define VK_NUMPAD1    '1'
#define VK_NUMPAD2    '2'
#define VK_NUMPAD3    '3'
#define VK_NUMPAD4    '4'
#define VK_NUMPAD5    '5'
#define VK_NUMPAD6    '6'
#define VK_NUMPAD7    '7'
#define VK_NUMPAD8    '8'
#define VK_NUMPAD9    '9'
#define VK_DECIMAL    '.'
#endif

namespace Mutables
{
DescriptionSet    descriptionSet;

/**
 * Repository of active mutable property sets which allows recursive containers
 */
class PropertySet;        // FIXME: find a way not to do this forward definition

#if 0 //_DEBUG
class Repository : public ParameterSet<PropertySet*>
{
public:
  ~Repository ()
  {
    Note::notemsg ("REPO CLEARING");

    clear ();

    Note::notemsg ("REPO CLEANED\0");
  }
};
#else
typedef ParameterSet<PropertySet*>  Repository;
#endif

//Repository    Property::repositorySet;
Repository      repositorySet;

class Functor : public Utils::Callback
{
protected:
  const Param    nm;

  const Param& name () const      {return nm;}
  const bool hasPS () const      {return (repositorySet && name ());}
  PropertySet& getPS ()        {return repositorySet[name ()];}
  const PropertySet& getPS () const  {return repositorySet[name ()];}
public:
  Functor (const Param& mutableName)
    : nm (mutableName)
  {}
};

#include "property.h"

/**
 * FIXME: Make the property set a typedef of the parameterset.
 * The set of valid values for a restricted parameter should be set by
 * using a second class which can be passed to the mutable property which
 * contains the valid values and verifies input matches the constraints.
 */
class PropertySet : public ParameterSet<Property>,
          public Patterns::Touch
{
public:
  static Param hotspot ()    {return "::hotspot\0";}
  static Param header ()    {return "::header\0";}
protected:
  UI::BoundsSet    bounds;    ///< display areas for mutables, FIXME: put in the individual properties

  Utils::CallbackSet  cbks;    ///< callbacks
  Utils::CallbackSet  vlds;    ///< validators, hardly used.

private:
  Param getMemLabel ()
  {
    IntegerSetType  mem_addr = reinterpret_cast<IntegerSetType>(this);
    char      buf[32] = {0};

    snprintf (buf, 32, "%0x16\0", mem_addr);

    return Param (buf);
  }

// CONSTRUCTOR
  PropertySet ()
  {
#if 0
    Param  buf = getMemLabel ();

    Note::notemsg ("Property set constructed at %s\0", buf.c_str ());

    // add the new set to the repo
    if ((repositorySet && buf) == true)
    {
      Note::notemsg ("A property set exists with that name...\0");
    }
    else
    {
      repositorySet.set (buf, this);
    }
#endif
  }
public:
// CREATION
  static Param create (Param label)
  {
    if ((repositorySet && label) == true)    {std::cout << "A property set called '" << label << "' already exists" << std::endl;}
    else
    {
      repositorySet.set (label, new PropertySet ());
      std::cout << "Created '" << label << "'" << std::endl;
    }

    return label;
  }
  virtual ~PropertySet ()
  {
#if 0
    // we no longer use memory locations as repo identifiers
    Param  buf = getMemLabel ();

    Note::notemsg ("Property set deleted at %s\0", buf.c_str ());

    repositorySet.wemove (buf);
#endif
  }
// MONITOR
  bool touch (const bool force = false)
  {
    ParamList  pl;
    bool    f = false;

    if (force == true)
    {
      setTouchCD (clock ());
      return true;
    }
    else
    {
      // loop through the properties to see if any have changed.
      // if they have, use the name-hash to update the change value.
      for (pl=begin (); pl!=end (); pl++)
      {
        // if the property has been touched, record the name and change value
        if (pl->second.touched () == true)
        {
          ParameterSet::setLCV (pl->first, pl->second);

          f = true;
        }

        // mark everything as untouched
        pl->second.untouch ();
      }

      // what if we want to force it?
      return f;
    }
  }
  void touchByForce ()                  {touch (true);}
  void setLCV (const Param& changedPropertyName, const Property& changedPropertyValue)
  {
    setTouchCD (changedPropertyName.hash ());
    ParameterSet::setLCV (changedPropertyName, changedPropertyValue);

    touch ();
  }
// ACCESSORS
  Property& operator[] (const Param& name)
  {
    if (((*this) && name) == false)
    {
      // create a new bounds for this mutable
      display ()[name].setRegion (0,0,0,0);
    //  access () [name] = AccessTypes::full ().hash ();
    }

    return ParameterSet::operator[] (name);
  }
  const Property& operator[] (const Param& name) const  {return ParameterSet::operator[] (name);}
  const UI::BoundsSet& display () const          {return bounds;}
  UI::BoundsSet& display ()                {return bounds;}
  const Utils::CallbackSet& callbacks () const      {return cbks;}
  Utils::CallbackSet& callbacks ()            {return cbks;}
  const Utils::CallbackSet& validators () const      {return vlds;}
  Utils::CallbackSet& validators ()            {return vlds;}
  /**
   * @returns true if the property set contains a parameter param set to the given value, false otherwise.
   */
  const bool is (const Param& param, const Param& value) const
  {
    return ((((*this) && param) == true) && ((*this)[param].str () == value));
  }
// OPERATORS
  PropertySet& operator= (const PropertySet& ps)      {this->copy (ps); return (*this);}
  PropertySet* operator= (const PropertySet* ps)      {this->copy (*ps); return this;}
  /**
   * Copies the values of matching parameters from ps into this
   */
  void copyMatchingValues (const PropertySet& ps)
  {
    ParamList  pl;

    for (pl=begin (); pl!=end (); pl++)
    {
      if ((ps && pl->first) == false)    {continue;}

      // DO NOT copy limited access variables
      // FIXME: make an access limiter? next () and prev () have the same problem
      if (pl->second.access () != Mutables::AccessTypes::full ())    {continue;}

      pl->second.set (ps[pl->first].str ());
    }
  }
  const IntegerSetType operator() (const Param& name) const
  {
    IntegerSetType    retval;

    retval = cbks (name);

#if 0
    if (retval == TRUE)
    {
      Note::notemsgn ("effective: '%s'\0", name.c_str ());
      const_cast<Mutables::PropertySet*>(this)->touch ();
    }
#else
    ConstParamList    pl;

    if (const_cast<Mutables::PropertySet*>(this)->touch () == true)
    {
      if (retval == FALSE)
      {
        retval = TRUE;
      }
      else
      {
        retval = TRUE;
      }
    }
#endif

    return retval;
  }
  void copy (const PropertySet& ps)
  {
    PropertySet::ConstParamList    pl;
    UI::BoundsSet::ConstParamList  bl;

    clear ();

    for (pl=ps.begin (); pl!=ps.end (); pl++)
    {
      (*this)[pl->first] = pl->second;
    }

    for (bl=ps.bounds.begin (); bl!=ps.bounds.end (); bl++)
    {
      (*this).bounds[bl->first].setRegion (bl->second);
    }
  }
  void clear ()
  {
    callbacks () ("clear\0");
    bounds.clear ();
    ParameterSet::clear ();
  }
// MISC
#if 0
  const char* persistableNodeID () const          {return "property\0";}
  friend std::ostream& operator<< (std::ostream& os, const PropertySet& ps)
  {
    PropertySet::ConstParamList  pl;

    for (pl=ps.begin (); pl!=ps.end (); pl++)
    {
      Patterns::Persistable::writeXMLBeginAttributeHang<const char *> (os, ps.persistableNodeID ());
      Patterns::Persistable::writeXMLAttributeValuePair (os, ps.persistableNodeLabelID (), pl->first.c_str ());
      os << pl->second;
      Patterns::Persistable::writeXMLBeginAttributeUnhangTerm (os);
    }

    return os;
  }
  void read (std::istream& is, const XMLNode& xn)
  {
    std::vector<XMLNode>::const_iterator  it;
    std::string    p, n, nv;

    for (it=xn.children.begin ();it!=xn.children.end ();it++)
    {
      is.seekg (it->offset ()+1);

      if ((p = Patterns::Persistable::fetchNextToken (is)).compare (persistableNodeID ()) != 0)
      {
        continue;
      }

      Patterns::Persistable::fetchNextAttributeValuePair (is, n, nv);

      if (n.compare (persistableNodeLabelID ()) != 0)
      {
        continue;
      }

      is >> ((*this)[nv]);
    }
  }
#endif
// SELECTION
  void next ()
  {
    ConstParamList  pl = find (selected ());

    if ((pl == end ()) || (++pl == end ()))
    {
      pl = begin ();
    }

    select (pl->first);
  }
  void prev ()
  {
    ConstParamList  pl = find (selected ());

    if ((pl == end ()) || (--pl == end ()))
    {
      pl = begin ();
      std::advance (pl, parameterCount () - 1);
    }

    select (pl->first);
  }
  void nextOf (const Param& type, const Param& access)
  {
#if 0
    ConstParamList  pl;

    // init
    if (interest () == false)
    {
      pl = begin ();

      if (pl->second.type () == type)
      {
        select (pl->first);
        return;
      }
      // else continue searching
    }
    else
    {
      pl = selected ();
    }

    // step
    do
    {
      pl++;
    } while ((pl != end ()) && (pl->second.type () != type));

    // final
     if (pl == end ())
     {
       deselect ();
     }
     else
     {
       select (pl->first);
     }
#else
    const size_t  mx = parameterCount ();
    int        i = 0;

    do
    {
      next ();

    } while (((type == Types::unknown ()) || (find (selected ())->second.type () != type)) &&        // accept all types for unknown
           ((access == AccessTypes::unknown ()) || (find (selected ())->second.access () != access)) &&  // accept all access for unknown
         (i<mx-1));
#endif
  }
  void prevOf (const Param& type, const Param& access)
  {
#if 0
    ConstParamList  pl;

    // init
    if (interest () == false)
    {
      pl = begin ();
      std::advance (pl, parameterCount () - 1);

      if (pl->second.type () == type)
      {
        select (pl->first);
        return;
      }
    }
    else
    {
      pl = selected ();
    }

    // step
    do
    {
      pl--;
    } while ((pl != end ()) && (pl->second.type () != type));

    if (pl == end ())
    {
      deselect ();
    }
    else
    {
      select (pl->first);
    }
#else
    const size_t  mx = parameterCount ();
    int        i = 0;

    do
    {
      prev ();
    } while (((type == Types::unknown ()) || (find (selected ())->second.type () != type)) &&
         ((access == AccessTypes::unknown ()) || (find (selected ())->second.access () != access)) &&
         (i++<mx-1));
#endif
  }
  void nextNotOf (const Param& type, const Param& access)
  {
    const size_t  mx = parameterCount ();
    int        i=0;

    do
    {
      next ();
    } while ((find (selected ())->second.type () == type) &&
           (find (selected ())->second.access () == access) && (i++<mx-1));
  }
  void prevNotOf (const Param& type, const Param& access)
  {
    const size_t  mx = parameterCount ();
    int        i = 0;

    do
    {
      prev ();
    } while ((find (selected ())->second.type () != type) &&
           (find (selected ())->second.access () == access) && (i++<mx-1));
  }
// BEHAVIOURS
  /**
   *  This function should always be executed.
   */
  class InitialMutableMapping : public Mutables::Functor
  {
  public:
    InitialMutableMapping (const Param& mutableName)
      : Mutables::Functor (mutableName)
    {}
    void operator() (void)
    {
      if (hasPS () == false)    {return;}

      Mutables::PropertySet&  ps = getPS ();

      ps[header ()].setType (Types::bound ());
      ps[hotspot ()].setType (Types::bound ());

      setReturnValue (FALSE);
      return;
    }
  };
  class Key : public Functor
  {
  public:
    static const unsigned int  MAX_BUF=32;
    static const unsigned int  MAX_BUF_TIMEOUT = 2*CLOCKS_PER_SEC;  // 2 second timeout
  protected:
    std::string    buf;
    unsigned int  buf_t;

  public:
    Key (const Param& mutableName)
      : Functor (mutableName), buf_t (0)
    {}
    void operator() (void)
    {
      if (hasPS () == false)            {setReturnValue (FALSE); return;}

      PropertySet&  ps = getPS ();
      IntegerSetType  vk = (*this)["keycode\0"];

      //Note::tmmark ();

      switch (vk)
      {
        // NB: return value is only true if something changed, altering the selection is not a change in value
        case VK_ESCAPE:    ps.deselect ();                      ps.setLCV ("::sel\0", ps.begin ()->second);  break;
#ifdef MUTABLE_FULL_LIST
        case VK_DOWN:    ps.next ();                        ps.setLCV ("::sel\0", ps.begin ()->second);  break;
        case VK_UP:      ps.prev ();                        ps.setLCV ("::sel\0", ps.begin ()->second);  break;
#else
        case VK_DOWN:    ps.nextOf (Types::unknown (), AccessTypes::full ());  ps.setLCV ("::sel\0", ps.begin ()->second);  setReturnValue (TRUE);  break;
        case VK_UP:      ps.prevOf (Types::unknown (), AccessTypes::full ());  ps.setLCV ("::sel\0", ps.begin ()->second);  setReturnValue (TRUE);  break;
#endif
        // this is a change in value...
        case VK_RIGHT:
        case VK_LEFT:
        {
          Mutator  *m = NULL;

          if (ps.interest () == false)                    {break;}
        //  if (ps.selected () == ps.end ())                  {break;}  // FIXME: interest doesn't take care of "PlotArea", which is a bound, but not an element.
          if ((ps && ps.selected ()) == false)                {break;}
          if (ps[ps.selected ()].access () != AccessTypes::full ())      {break;}

          if (vk == VK_RIGHT)
          {
            if ((descriptionSet[ps[ps.selected ()].type ()] && "next\0") == false)      {break;}
            m = dynamic_cast<Mutator*>(&descriptionSet[ps[ps.selected ()].type ()]["next\0"]);
          }
          else if (vk == VK_LEFT)
          {
            if ((descriptionSet[ps[ps.selected ()].type ()] && "prev\0") == false)      {break;}
            m = dynamic_cast<Mutator*>(&descriptionSet[ps[ps.selected ()].type ()]["prev\0"]);
          }

          if (m == NULL)                  {break;}

          if ((((*this) && "shift\0") == true) && ((*this)["shift\0"] == 1))  {m->alt (true);}
          else                                {m->alt (false);}

          // HACK!!
          {
          if ((ps[ps.selected ()].type () == Mutables::Types::attribute ()) &&
            ((ps && "Source\0") == true))
          {
            m->set (ps[ps.selected ()].str (), ps["Source\0"].getLabel ());
          }
          else
          {
            m->set (ps[ps.selected ()].str ());
          }
          }

          // about to change the value
          // FIXME: only if the new value is different from the current!
          ps.callbacks () ("prechange\0");

          // perform the update of the value
          (*m) ();

          // if the new value is different from the old, update it
          if (ps[ps.selected ()].str () != m->value ())
          {
            // copy the old value into the change variable
            ps.setLCV (ps.selected (), ps[ps.selected ()]);

            // replace the old value with the new
            ps[ps.selected ()].set (m->value ());

            // fire the post change event
            ps.callbacks () ("postchange\0");

            // record the touch
            std::cout << "'" << ps.selected () << "' value changed" << std::endl;
            ps.touch ();
            setReturnValue (TRUE);
          }
          else
          {
            // the update event did not change the value, so ignore it.
            setReturnValue (FALSE);
          }

          break;
        }
        case VK_SPACE:    ps.callbacks () ("update\0");              setReturnValue (FALSE); break;
      // BUFFERS
        case VK_SEPARATOR:
        case VK_RETURN:
        {
          unsigned int  t = clock ();

          if (t-buf_t > MAX_BUF_TIMEOUT)
          {
            //std::fill (buf, buf+MAX_BUF, 0);
            buf.clear ();
            break;
          }
          else if (ps.interest () == false)
          {
            break;
          }
          else
          {
            // VALIDATE THE INPUT
            Validator  *v;

            v = dynamic_cast<Validator*>(&descriptionSet[ps[ps.selected ()].type ()]["defaultValidator\0"]);

            // HACK!!
            {
            if ((ps[ps.selected ()].type () == Mutables::Types::attribute ()) &&
              ((ps && "Source\0") == true))
            {
              v->set (buf, ps["Source\0"].getLabel ());
            }
            else
            {
              v->set (buf);
            }
            }

          //  if (ps[ps.selected ()->first].isValid (Param (buf)) == false)
            (*v) ();

            if (v->valid () == false)
            {
              std::cout << "Invalid entry for type: '" << buf << ", " << ps[ps.selected ()].type () << "'" << std::endl;
              setReturnValue (FALSE);
              break;
            }
            else
            {
              setReturnValue (ps[ps.selected ()].set (v->value ()));
            }

            // END - VALIDATE

            buf.clear ();
            buf_t = 0;
          }
          break;
        }
        case VK_NUMPAD0:
        case VK_NUMPAD1:
        case VK_NUMPAD2:
        case VK_NUMPAD3:
        case VK_NUMPAD4:
        case VK_NUMPAD5:
        case VK_NUMPAD6:
        case VK_NUMPAD7:
        case VK_NUMPAD8:
        case VK_NUMPAD9:
        case VK_DECIMAL:
        default:
        {
          unsigned int  t = clock ();
#ifdef _WIN32_
          char      two[2] = {MapVirtualKey (static_cast<UINT>(vk), MAPVK_VK_TO_CHAR), '\0'};
#else
          char      two[2] = {static_cast<char>(vk), '\0'};
#endif

          if (t-buf_t > MAX_BUF_TIMEOUT)
          {
            buf.clear ();
          }

#ifdef _WIN32_
          strcat_s (buf, MAX_BUF, two);
#else
          //strncat (buf, two, MAX_BUF);
          buf.append (std::string (two));
#endif
          buf_t = clock ();
          break;
        }
      // END BUFFERS
        //default:
        //{
        //  setReturnValue (FALSE);
        //  break;
        //}
      }

      //Note::tmfntm ();

      return;
    }
  };
  class Draw : public Functor
  {
  protected:
    const UI::Bounds&  rgn;

    const UI::Bounds& drawRegion () const    {return rgn;}

    bool drawProperty (const Param& name)
    {
      UI::BoundsSet::ConstParamList      bl;

      if (hasPS () == false)                        {return false;}
      Mutables::PropertySet& ps = getPS ();

      if ((ps && name) == false)                      {return false;}

      if ((bl = ps.bounds.find (name)) == ps.bounds.end ())        {return false;}

      // ignore zero sized bounds
      if ((bl->second.width () == 0) && (bl->second.height () == 0))    {return false;}

      // draw a selection boks, if selected
      if (ps.isSelected (bl->first) == true)
      {
#ifdef __GL_H__
        float  c[4];

        glGetFloatv (GL_CURRENT_COLOR, c);
#if 0
        glLineWidth (4.0f);
        glColor4f (1.0f, 0.0f, 0.0f, 1.0f);
        UI::Bounds::draw (GL_LINE_LOOP, bl->second);
#else
        UI::Bounds::drawHighlighted (bl->second);
#endif

      //  glColor4f (1.0f, 1.0f, 1.0f, 1.0f);
        glLineWidth (1.0f);
        glColor4f (c[0], c[1], c[2], c[3]);
#endif
      }

      // call the property specific draw functor
      ps[bl->first].draw (bl->second);

      // return
      return true;
    }
    void preDraw ()
    {
#ifdef __GL_H__
      glPushMatrix ();
      glLoadIdentity ();
      glTranslatef (drawRegion ().left (), drawRegion ().top (), 0.0f);
#endif
    }
    void postDraw ()
    {
      if (hasPS () == false)            {return;}

      Mutables::PropertySet& ps = getPS ();

      // draw the list of properties on the right hand side of the mutable.
      if ((((*this) && "style\0") == true) && ((*this)["style\0"] == Param ("focus\0").hash ()))
      {
        propertyList (ps, drawRegion ());
      }

#ifdef __GL_H__
      glPopMatrix ();
#endif
    }
  public:
    Draw (const Param& mutableName, const UI::Bounds& mutableDrawRegion)
      : Functor (mutableName), rgn (mutableDrawRegion)
    {

    }
    void operator() (void)
    {
      if (hasPS () == false)          {setReturnValue (FALSE); return;}

      Mutables::PropertySet&  ps = getPS ();

      Mutables::PropertySet::ConstParamList  pl;
    //  Mutables::PropertySet& ps = getPS ();

      // setup the draw area
      preDraw ();

      // draw the properties (basic)
      for (pl=ps.begin (); pl!=ps.end (); pl++)
      {
        drawProperty (pl->first);
      }

      // cleanup
      postDraw ();

      // return
      // nothing changed
    //  setReturnValue (TRUE);
      return;
    }
    /**
     * draws the list of PropertySet parameters along the right hand side of drawRegion
     * FIXME: draw nested lists
     */
    static void propertyList (const PropertySet& ps, const UI::Bounds& drawRegion)
    {
#ifdef MUTABLES_LIST_PROPERTIES
#ifdef __GL_H__
      // draw a long list of mutables along one side the property list
      Mutables::PropertySet::ConstParamList  pl;
      UI::Bounds                plb;
      char                  buf[MAX_PATH]={0};
      float                  col[4];

      glPushMatrix ();
      glLoadIdentity ();
      glTranslatef (drawRegion.horzMin (), drawRegion.vertMax (), 0.0f);

      plb.setRegion (drawRegion.width (), 0.0f, drawRegion.width () + 140.0f, 0.0f);
      float  scale;
      float  bbox[4];

      glColor4f (0.0f, 0.0f, 0.0f, 1.0f);

      for (pl=ps.begin (); pl!=ps.end (); pl++)
      {
        // set the colour based on the access type
        {
        if (ps[pl->first].access () == Mutables::AccessTypes::full ())
        {
#ifdef MUTABLE_FULL_LIST
          col[0] = 0.0f; col[1] = 0.6f; col[2] = 0.0f; col[3] = 1.0f;
#else
          col[0] = 0.0f; col[1] = 0.0f; col[2] = 0.0f; col[3] = 1.0f;
#endif
        }
        else if (ps[pl->first].access () == Mutables::AccessTypes::readonly ())
        {
#ifdef MUTABLE_FULL_LIST
          col[0] = 1.0f; col[1] = 0.0f; col[2] = 0.0f; col[3] = 0.3f;
#else
          continue;
#endif
        }
        else if (ps[pl->first].access () == Mutables::AccessTypes::hidden ())
        {
#ifdef MUTABLE_FULL_LIST
          col[0] = 0.0f; col[1] = 0.0f; col[2] = 1.0f; col[3] = 0.3f;
#else
          continue;
#endif
        }
        else
        {
          col[0] = 0.0f; col[1] = 0.0f; col[2] = 0.0f; col[3] = 1.0f;
        }
        }

#if 0
        glPushMatrix ();
        glTranslatef (plb.left () + UI::Display::spacing (), plb.top (), 0.0f);

        // draw the text
        if (ps.isSelected (pl->first) == true)
        {
          glScalef (30.0f, -30.0f, 1.0f);
          sprintf_s (buf, MAX_PATH, "%s [%s]\0", pl->first.c_str (), pl->second.str ().c_str ());
        //  GLUtils::Text::glPrintf ("%s [%s]\0", pl->first.c_str (), pl->second.str ().c_str ());
        }
        else
        {
          glScalef (20.0f, -20.0f, 1.0f);
          sprintf_s (buf, MAX_PATH, pl->first.c_str ());
        //  GLUtils::Text::glPrintf (pl->first.c_str ());
        }

        GLUtils::Text::glPrintf (buf);
#else
        if (ps.isSelected (pl->first) == true)      {scale = MUTABLE_SELECTED_TEXT_SCALE; sprintf_s (buf, MAX_PATH, "%s [%s]\0", pl->first.c_str (), pl->second.str ().c_str ());}
        else                      {scale = MUTABLE_TEXT_SCALE; sprintf_s (buf, MAX_PATH, pl->first.c_str ());}

        glColorMask (GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
        UI::Display::drawText (plb, buf, GLUtils::Text::LEFT, GLUtils::Text::MIDDLE, scale);
        GLUtils::Text::boundingBox (bbox[0], bbox[1], bbox[2], bbox[3]);
        glColorMask (GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

        plb.setRegion (drawRegion.width () + UI::Display::borderSize (), plb.bottom (),
                drawRegion.width () + UI::Display::spacing () + (bbox[2]-bbox[0]) * scale + 5.0f,
                plb.bottom () + (bbox[3]-bbox[1]) * scale + 7.0f);

        glColor4f (1,1,1,1);
        UI::Bounds::draw (GL_QUADS, plb);

        glLineWidth (static_cast<float>(UI::Display::borderSize ()));
        glColor4fv (UI::drawStyleSet["focus\0"].frame ());
        UI::Bounds::draw (GL_LINE_LOOP, plb);
        glLineWidth (1);

      //  glColor4f (0,0,0,1);
        glColor4fv (col);
        UI::Display::drawText (plb, buf, GLUtils::Text::LEFT, GLUtils::Text::MIDDLE, scale);
#endif
      }

      glPopMatrix ();
#endif
#endif
    }
  };
  class Hover : public Functor
  {
  public:
    Hover (const Param& mutableName)
      : Functor (mutableName)
    {}
    void operator() (void)
    {
      setReturnValue (FALSE);
      return;
    }
  };
  class Click : public Functor
  {
  protected:
    const UI::Bounds&    rgn;

    const UI::Bounds& drawRegion () const    {return rgn;}
  public:
    Click (const Param& mutableName, const UI::Bounds& mutableDrawRegion)
      : Functor (mutableName), rgn (mutableDrawRegion)
    {}
    void operator() (void)
    {
      const float    x = static_cast<float>((*this)["x\0"]) - drawRegion ().left ();
      const float    y = static_cast<float>((*this)["y\0"]) - drawRegion ().top ();
      UI::BoundsSet::ConstParamList  pl;

      if (hasPS () == false)          {setReturnValue (FALSE); return;}

      Mutables::PropertySet& ps = getPS ();

      // check the header or hotspot aren't selected
      {
        UI::Bounds      header;
        UI::Bounds      hotspot;

        UI::Display::titleArea (drawRegion (), header);
        UI::Display::hotspotArea (drawRegion (), hotspot);

        // use the original x y values, not the ones translated into this drawable
        if (header.pointInside (static_cast<float>((*this)["x\0"]), static_cast<float>((*this)["y\0"])))
        {
          ps.select (Mutables::PropertySet::header ());
          setReturnValue (TRUE);
          return;
        }
        else if (hotspot.pointInside (static_cast<float>((*this)["x\0"]), static_cast<float>((*this)["y\0"])))
        {
          ps.select (Mutables::PropertySet::hotspot ());
          setReturnValue (TRUE);
          return;
        }
        else
        // go through the items in the list
        {
          UI::BoundsSet::ConstParamList  fn = ps.display ().end (); // found element

          for (pl = ps.display ().begin (); pl!=ps.display ().end (); pl++)
          {
            if (ps.display ()[pl->first].pointInside (x, y) == true)
            {
              if ((fn == ps.display ().end ()) ||
                (fn->second.area () > pl->second.area ()))  // favour the smallest bound item
              {
                fn = pl;
              }
            }
          }

          // select the item
          if (fn == ps.display ().end ())      {setReturnValue (FALSE); return;}
          else                  {ps.select (fn->first); setReturnValue (TRUE); return;}
        }
      }
    }
  };
  class Scroll : public Functor
  {
  public:
    Scroll (const Param& mutableName)
      : Functor (mutableName)
    {}
    void operator() (void)
    {
      const float  wheeldelta = static_cast<float>(Utils::wsgn ((*this)["w\0"]));

      if (hasPS () == false)            {setReturnValue (FALSE); return;}

      Mutables::PropertySet& ps = getPS ();

      if (ps.interest () == false)        {setReturnValue (FALSE); return;}
      if ((ps.callbacks () && "key\0") == false)  {setReturnValue (FALSE); return;}

      if (wheeldelta > 0)
      {
        const_cast<Utils::CallbackSet&>(ps.callbacks ())["key\0"]["keycode\0"] = VK_RIGHT;
        ps ("key\0");
      }
      else
      {
        const_cast<Utils::CallbackSet&>(ps.callbacks ())["key\0"]["keycode\0"] = VK_LEFT;
        ps ("key\0");
      }

      setReturnValue (ps.callbacks ()["key\0"].returnValue ());
      return;
    }
  };
  class Drag : public Functor
  {
  public:
    Drag (const Param& mutableName)
      : Functor (mutableName)
    {}
    void operator() (void)
    {
      setReturnValue (FALSE);
      return;
    }
  };
  class Export : public Functor
  {
  public:
    Export (const Param& mutableName)
      : Functor (mutableName)
    {}
    void operator() (void)
    {
      std::cout << "NIMPL" << std::endl;
      setReturnValue (FALSE);
      return;
    }
  };
  /**
   * This functor is invoked before a propertyset value is changed
   */
  class PreChange : public Functor
  {
  public:
    PreChange (const Param& mutableName)
      : Functor (mutableName)
    {}
    void operator() (void)
    {
      setReturnValue (FALSE);
      return;
    }
  };
  /**
   * This functor is invoked after a propertyset value is changed
   */
  class PostChange : public Functor
  {
  public:
    PostChange (const Param& mutableName)
      : Functor (mutableName)
    {}
    void operator() (void)
    {
      setReturnValue (FALSE);
      return;
    }
  };
  /**
   * This functor implements any execution that should happen when the propertyset values
   * are changed/updated/appended etc.
   */
  class Update : public Functor
  {
  public:
    Update (const Param& mutableName)
      : Functor (mutableName)
    {}
    void operator() (void)
    {
      setReturnValue (FALSE);
      return;
    }
  };
  /**
   * Update the display bounds
   */
  class UpdateDisplayElements : public Functor
  {
  protected:
    const UI::Bounds&  rgn;

    const UI::Bounds& drawRegion () const    {return rgn;}
  public:
    UpdateDisplayElements (const Param& mutableName, const UI::Bounds& mutableDrawRegion)
      : Mutables::Functor (mutableName), rgn (mutableDrawRegion)
    {}
    void operator() (void)
    {
      if (hasPS () == false)      {return;}

      PropertySet&  ps = getPS ();

      // draw the outline
      UI::Bounds  hdr, hsp;

      UI::Display::titleArea (drawRegion (), ps.display ()[header ()]);
      UI::Display::hotspotArea (drawRegion (), ps.display ()[hotspot ()]);

      ps.display ()[header ()].offset (-drawRegion ().left (), -drawRegion ().top ());
      ps.display ()[hotspot ()].offset (-drawRegion ().left (), -drawRegion ().top ());

      ps[header ()].set (ps.display ()[header ()]);
      ps[hotspot ()].set (ps.display ()[hotspot ()]);

      // return
      setReturnValue (FALSE);
      return;
    }
  };
  /**
   * This function fires when the property set is cleared
   */
  class Clear : public Functor
  {
  public:
    Clear (const Param& mutableName)
      : Functor (mutableName)
    {}
    void operator() (void)
    {
      setReturnValue (TRUE);
      return;
    }
  };
  /**
   * function for reading property set descriptions from a file stream
   */
  class Read : public Functor
  {
  protected:
    std::istream* is;

    const bool hasInputStream () const  {return (is != NULL);}
    std::istream& inputStream ()    {return *is;}
  public:
    Read (const Param& mutableName)
      : Functor (mutableName), is (NULL)
    {}
    void setInputStream (std::istream& newInputStream)  {is = &newInputStream;}
    void closeInputStream ()              {is = NULL;}
    void operator() (void)
    {
      setReturnValue (FALSE);
      return;
    }
  };
};
#if 0
/**
 * A collection of property sets
 * FIXME: can we make this recursive? should we make this recursive?
 */
class Container : public ParameterSet<PropertySet*>,
          public Patterns::Touch
{
protected:
  Utils::CallbackSet    cbks;
public:
  const Utils::CallbackSet& callbacks () const      {return cbks;}
  Utils::CallbackSet& callbacks ()            {return cbks;}
};
#endif


#include "validators.h"
#include "mutators.h"
#include "drawers.h"

/**
 * BINDERS
 */
class BND_Unknown : public Binder
{
public:
  BND_Unknown ()
  {}
  void operator() (void)
  {
    setReturnValue (TRUE);
    return;
  }
};

#include "descriptionset.h"

// MISC UTILS
#ifdef GLUTILS_H
static bool getProgram (GLUtils::ShaderProfile& outProfile, const Param& programName)
{
  if ((repositorySet && "Shaders\\\0") == false)        {return false;}

  const PropertySet&  sh = repositorySet["Shaders\\\0"];

  if ((sh && programName) == false)              {return false;}

  const PropertySet&  pr = sh[programName].ps ();

  const PropertySet&  pr2 = repositorySet[sh[programName].getLabel ()];

  if (outProfile.from (pr2["Program\0"].str ()) == false)    {return false;}

  return true;
}
#endif
};

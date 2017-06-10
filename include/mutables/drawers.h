/**
 * DRAWERS
 */
class DRW_Unknown : public Drawer
{
protected:
public:
  DRW_Unknown ()
  {}
  void operator() (void)
  {
    // get the display region
    const UI::Bounds&  b = drawRegion ();

    if (b.width () == 0)              {return;}
    if (b.height () == 0)             {return;}

    switch (b.orientation ())
    {
#ifdef __GL_H__
      case UI::ZERO:     UI::Display::drawText (drawRegion (), value ().c_str (), GLUtils::Text::LEFT, GLUtils::Text::MIDDLE, 30.0f);  break;
      case UI::LEFT90:   UI::Display::drawText (drawRegion (), value ().c_str (), GLUtils::Text::LEFT, GLUtils::Text::MIDDLE, 30.0f);  break;
      case UI::RIGHT90:  UI::Display::drawText (drawRegion (), value ().c_str (), GLUtils::Text::LEFT, GLUtils::Text::MIDDLE, 30.0f);  break;
      default:                                                              break;
#endif
    }
  }
};
class DRW_Range : public Drawer
{
public:
  static void drawRange (const UI::Bounds& region, const Param& min, const Param& max)
  {
#ifdef __GL_H__
    const float      lw=0.4f;  // label width
//    float        rng[2];
    char        buf[64]={0};
    UI::Bounds      b;
    const UI::Rotation  r = region.orientation ();

    if ((region.width () == 0) || (region.height () == 0))        {return;}

    // MIN
    switch (r)
    {
    case UI::ZERO:
    case UI::INVERT:  b.setRegion (region.left (), region.top (), region.left () + region.width () * lw, region.bottom ()); break;
    case UI::LEFT90:  b.setRegion (region.left (), region.top (), region.right (), region.top () + region.height () * lw);  break;
    case UI::RIGHT90: b.setRegion (region.left (), region.top () + region.height () * lw, region.right (), region.top ());  break;
    }

    UI::Display::drawText (b, min.c_str (), GLUtils::Text::LEFT, GLUtils::Text::MIDDLE);

    // MAX
    switch (r)
    {
    case UI::ZERO:
    case UI::INVERT:  b.setRegion (region.right () - region.width () * lw, region.top (), region.right (), region.bottom ());    break;
    case UI::LEFT90:  b.setRegion (region.left (), region.bottom () - region.height () * lw, region.right (), region.bottom ()); break;
    case UI::RIGHT90: b.setRegion (region.left (), region.bottom (), region.right (), region.bottom () - region.height () * lw); break;
    }

    UI::Display::drawText (b, max.c_str (), GLUtils::Text::RIGHT, GLUtils::Text::MIDDLE);
#endif
  }
  static void drawRange (const UI::Bounds& region, const Param& rangeValue)
  {
#ifdef __GL_H__
    const float      lw=0.4f;  // label width
    float        rng[2];
    char        buf[64]={0};
    UI::Bounds      b;
    const UI::Rotation  r = region.orientation ();

    if ((region.width () == 0) || (region.height () == 0))            {return;}
    if (Property::getRange (rng[0], rng[1], rangeValue, 0) == false)  {return;}

    // MIN
    switch (r)
    {
    case UI::ZERO:
    case UI::INVERT:  b.setRegion (region.left (), region.top (), region.left () + region.width () * lw, region.bottom ()); break;
    case UI::LEFT90:  b.setRegion (region.left (), region.top (), region.right (), region.top () + region.height () * lw);  break;
    case UI::RIGHT90: b.setRegion (region.left (), region.top () + region.height () * lw, region.right (), region.top ());  break;
    }

    sprintf_s (buf, 64, "%0.4f\0", rng[0]);
  //  UI::Bounds::draw (GL_LINE_LOOP, b);
    UI::Display::drawText (b, buf, GLUtils::Text::LEFT, GLUtils::Text::MIDDLE);

    // MAX
    switch (r)
    {
    case UI::ZERO:
    case UI::INVERT:  b.setRegion (region.right () - region.width () * lw, region.top (), region.right (), region.bottom ());    break;
    case UI::LEFT90:  b.setRegion (region.left (), region.bottom () - region.height () * lw, region.right (), region.bottom ()); break;
    case UI::RIGHT90: b.setRegion (region.left (), region.bottom (), region.right (), region.bottom () - region.height () * lw); break;
    }

    sprintf_s (buf, 64, "%0.4f\0", rng[1]);
  //  UI::Bounds::draw (GL_LINE_LOOP, b);
    UI::Display::drawText (b, buf, GLUtils::Text::RIGHT, GLUtils::Text::MIDDLE);
#endif
  }
protected:
public:
  DRW_Range ()
  {}
  void operator() (void)
  {
#if 0
//#ifdef __GL_H__
    const float    lw=0.4f;  // label width
    float      rng[2];
    char      buf[64]={0};
    UI::Bounds    b;
    const UI::Rotation  r = drawRegion ().orientation ();

    if ((drawRegion ().width () == 0) || (drawRegion ().height () == 0))  {return;}

    if (Property::getRange (rng[0], rng[1], value (), 0) == false)      {return;}

    // MIN
    switch (r)
    {
    case UI::ZERO:
    case UI::INVERT:  b.setRegion (drawRegion ().left (), drawRegion ().top (), drawRegion ().left () + drawRegion ().width () * lw, drawRegion ().bottom ());  break;
    case UI::LEFT90:  b.setRegion (drawRegion ().left (), drawRegion ().top (), drawRegion ().right (), drawRegion ().top () + drawRegion ().height () * lw); break;
    case UI::RIGHT90:  b.setRegion (drawRegion ().left (), drawRegion ().top () + drawRegion ().height () * lw, drawRegion ().right (), drawRegion ().top ()); break;
    }

    sprintf_s (buf, 64, "%0.4f\0", rng[0]);
  //  UI::Bounds::draw (GL_LINE_LOOP, b);
    UI::Bounds::drawText (b, buf, GLUtils::Text::LEFT, GLUtils::Text::MIDDLE);

    // MAX
    switch (r)
    {
    case UI::ZERO:
    case UI::INVERT:  b.setRegion (drawRegion ().right () - drawRegion ().width () * lw, drawRegion ().top (), drawRegion ().right (), drawRegion ().bottom ());  break;
    case UI::LEFT90:  b.setRegion (drawRegion ().left (), drawRegion ().bottom () - drawRegion ().height () * lw, drawRegion ().right (), drawRegion ().bottom ()); break;
    case UI::RIGHT90:  b.setRegion (drawRegion ().left (), drawRegion ().bottom (), drawRegion ().right (), drawRegion ().bottom () - drawRegion ().height () * lw); break;
    }

    sprintf_s (buf, 64, "%0.4f\0", rng[1]);
  //  UI::Bounds::draw (GL_LINE_LOOP, b);
    UI::Bounds::drawText (b, buf, GLUtils::Text::RIGHT, GLUtils::Text::MIDDLE);
#else
    drawRange (drawRegion (), value ());
#endif
  }
};
class DRW_Image : public Drawer
{
public:
  DRW_Image ()
  {}
  void operator() (void)
  {
#ifdef __GL_H__
    GLUtils::TextureDescription  txd;

    if (Property::getImage (txd, Drawer::value (), 0) == false)
    {
      return;
    }

    draw (txd, drawRegion ());
#endif
  }
#ifdef __GL_H__
  static void draw (const GLUtils::TextureDescription& txd, const UI::Bounds& bound)
  {
    glColor4f (1,1,1,1);
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable (GL_TEXTURE_2D);
    glTexEnvi (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    txd.bind ();
  //  UI::Bounds::draw (GL_QUADS, drawRegion);
  //  UI::Bounds::draw (GL_QUADS, vis.dels["Plot\0"]);

    // FIXME: need a texturemapping shader, or at least know which vertex attribute
    // glTexCoord maps onto (in fixed func)
    glBegin (GL_QUADS);
    // FIXME: This colourmap size and location is a ratio of the draw region, make it fixed
    glTexCoord2i (0, 0); glVertex2f (bound.left (), bound.bottom ());
    glTexCoord2i (1, 0); glVertex2f (bound.right (), bound.bottom ());
    glTexCoord2i (1, 1); glVertex2f (bound.right (), bound.top ());
    glTexCoord2i (0, 1); glVertex2f (bound.left (), bound.top ());
    glEnd ();

    glDisable (GL_TEXTURE_2D);
    glDisable (GL_BLEND);
  }
#endif
};
class DRW_Attribute : public Drawer
{
public:
  static void attributeBound (const Param& name, const Mutables::Property& min, const Mutables::Property& max, const UI::Bounds& drawRegion, UI::Bounds& dataRange, UI::Bounds& userRange)
  {
    float      frng[2], brng[2], mn[2], mx[2];
//    PropertySet    *ds, *us;

#if 0
    if ((statistics && "::data\0") == false)                {Note::notemsg ("No Range parameter in stats\0"); return;}
    if ((statistics && "::user\0") == false)                {Note::notemsg ("No UserRange parameter in stats\0"); return;}

    if (statistics["::data\0"].getPropertySet (&ds) == false)        {Note::notemsg ("Could not get ::data property set\0"); return;}
    if (statistics["::user\0"].getPropertySet (&us) == false)        {Note::notemsg ("Could not get ::user property set\0"); return;}

    if ((*ds)[name].getRange (frng[0], frng[1]) == false)          {Note::notemsg ("Could not read Range parameter\0"); return;}
    if ((*us)[name].getRange (brng[0], brng[1]) == false)          {Note::notemsg ("Could not read UserRange parameter\0"); return;}

    mn[0] = std::min (frng[0], brng[0]);
    mx[0] = std::max (frng[0], brng[0]);
    mn[1] = std::min (frng[1], brng[1]);
    mx[1] = std::max (frng[1], brng[1]);
#else
    min.getNumber (frng[0]);
    max.getNumber (frng[1]);
    brng[0] = 0.0f;
    brng[1] = 1.0f;
#endif

    switch (drawRegion.orientation ())
    {
      case UI::ZERO:
      {
        userRange.setRegion (drawRegion.left () + (drawRegion.width () * (brng[0] - mn[0])/(mx[1]-mn[0])),
                             drawRegion.top (),
                             drawRegion.left () + (drawRegion.width () * (brng[1] - mn[0])/(mx[1]-mn[0])),
                             drawRegion.bottom ());
        dataRange.setRegion (drawRegion.left () + (drawRegion.width () * (frng[0] - mn[0])/(mx[1]-mn[0])),
                             drawRegion.top (),
                             drawRegion.left () + (drawRegion.width () * (frng[1] - mn[0])/(mx[1]-mn[0])),
                             drawRegion.bottom ());
        break;
      }
      case UI::RIGHT90:
      {
        userRange.setRegion (drawRegion.left (),
                             drawRegion.bottom () - (drawRegion.height () * (brng[0] - mn[0])/(mx[1]-mn[0])),
                             drawRegion.right (),
                             drawRegion.bottom () - (drawRegion.height () * (brng[1] - mn[0])/(mx[1]-mn[0])));
        dataRange.setRegion (drawRegion.left (),
                             drawRegion.bottom () - (drawRegion.height () * (frng[0] - mn[0])/(mx[1]-mn[0])),
                             drawRegion.right (),
                             drawRegion.bottom () - (drawRegion.height () * (frng[1] - mn[0])/(mx[1]-mn[0])));
        break;
      }
      case UI::LEFT90:
      {
        userRange.setRegion (drawRegion.left (),
                             drawRegion.right (),
                             drawRegion.bottom () - (drawRegion.height () * (brng[1] - mn[0])/(mx[1]-mn[0])));
                             drawRegion.bottom () - (drawRegion.height () * (brng[0] - mn[0])/(mx[1]-mn[0])),
        dataRange.setRegion (drawRegion.left (),
                             drawRegion.bottom () - (drawRegion.height () * (frng[0] - mn[0])/(mx[1]-mn[0])),
                             drawRegion.right (),
                             drawRegion.bottom () - (drawRegion.height () * (frng[1] - mn[0])/(mx[1]-mn[0])));
        break;
      }
      case UI::INVERT:
      {
        userRange.setRegion (drawRegion.left () + (drawRegion.width () * (brng[0] - mn[0])/(mx[1]-mn[0])),
                             drawRegion.top (),
                             drawRegion.left () + (drawRegion.width () * (brng[1] - mn[0])/(mx[1]-mn[0])),
                             drawRegion.bottom ());
        dataRange.setRegion (drawRegion.left () + (drawRegion.width () * (frng[0] - mn[0])/(mx[1]-mn[0])),
                             drawRegion.top (),
                             drawRegion.left () + (drawRegion.width () * (frng[1] - mn[0])/(mx[1]-mn[0])),
                             drawRegion.bottom ());
        break;
      }
      default:
      {
        mn[0] = 0.0f;
        mn[1] = 0.0f;
        mx[0] = 1.0f;
        mx[1] = 1.0f;
        break;
      }
    }
  }
#if 0
  static void drawSparkmash (const GLUtils::TextureDescription& data, const GLUtils::TextureDescription& maxs, const GLUtils::ShaderProfile& histogramShader, const UI::Bounds& region)
  {
  //  if ((gpuMemBacking () && sparkmashData) == false)  {Note::notemsg ("Spark mash not found\0"); return;}
  //  if ((gpuMemBacking () && sparkmashMaxs) == false)  {Note::notemsg ("Spark maxs not found\0"); return;}
  //  if ((shaders () && "HistogramPlot2D\0") == false)  {return;}

  //  const GLUtils::ShaderProfile&  hist = shaders ()["HistogramPlot2D\0"];

    float      vp[4] ={0.0f};
    float      mv[16]={0.0f};
    float      pr[16]={0.0f};
    float      otl[4] = {0.0f};
    float      itl[2] = {0.0f};
    float      col[4] = {0.0f};

    glGetFloatv (GL_VIEWPORT, vp);
    glGetFloatv (GL_PROJECTION_MATRIX, pr);
    glGetFloatv (GL_MODELVIEW_MATRIX, mv);
    glGetFloatv (GL_CURRENT_COLOR, col);

    glEnable (GL_BLEND);

    // set the shader up
  //  GLUtils::linkProgram (hist);
    GLUtils::attachProgram (histogramShader);
    // set the uniforms
    glUniformMatrix4fv (0, 1, GL_FALSE, pr);
    glUniformMatrix4fv (1, 1, GL_FALSE, mv);
    glUniform4fv (2, 1, col);

    // set the textures
    glActiveTexture (GL_TEXTURE0);  data.bind ();
    glActiveTexture (GL_TEXTURE1);  maxs.bind ();

    UI::Bounds::draw (GL_QUADS, region);

    GLUtils::detachProgram ();
    glActiveTexture (GL_TEXTURE0);

    glDisable (GL_BLEND);
    glColor4fv (col);
    glLineWidth (1.0f);

  }
#endif
#ifdef __GL_H__
  //FIXME: read the textures from a mutable property set
  static void drawSparkline (const GLUtils::ShaderProfile& plotHistogramShader,    /// program for drawing histograms
                             const GLUtils::TextureDescription& minValues,      /// minimum values for attribute
                             const GLUtils::TextureDescription& maxValues,      /// maximum values for attribute
                             const GLUtils::TextureDescription& data,          /// histogram data
                             const GLUtils::TextureDescription& maxs,          /// histogram maximums for scaling
                             const UI::Bounds& region,                /// screenspace draw region
                             const IntegerSetType attributeID)            /// index of the attribute within the textures
  {
    UI::Bounds    or;      // oriented bound
    float      vp[4] ={0.0f};
    float      mv[16]={0.0f};
    float      pr[16]={0.0f};
    float      otl[4] = {0.0f};
    float      itl[2] = {0.0f};
    float      col[4] = {0.0f};

    glGetFloatv (GL_VIEWPORT, vp);
    glGetFloatv (GL_PROJECTION_MATRIX, pr);
    glGetFloatv (GL_MODELVIEW_MATRIX, mv);
    glGetFloatv (GL_CURRENT_COLOR, col);

    glEnable (GL_BLEND);

    // attach the shader
    GLUtils::attachProgram (plotHistogramShader);

    // set the uniforms
    glUniformMatrix4fv (0, 1, GL_FALSE, pr);
    glUniformMatrix4fv (1, 1, GL_FALSE, mv);

    // set the textures
    minValues.bind (GL_TEXTURE0);  // use the user ranges
    maxValues.bind (GL_TEXTURE1);
    data.bind (GL_TEXTURE2);    // data values (histogram, intensities, etc)
    maxs.bind (GL_TEXTURE3);    // data ranges

    region.getRegionOriented (or);

    // set the uniforms
    glUniform2f (2, or.left (), or.right ());      // min
    glUniform2f (3, or.top (), or.bottom ());      // max
    glUniform1i (4, TSY);                // count of entries in the hgram
    glUniform1i (5, static_cast<GLint>(attributeID));  // data attribute
    glUniform4fv (6, 1, col);

    // push BIN_COUNT number of points onto the gpu and let the shaders do their stuff
    glDrawArrays (GL_LINE_STRIP, 0, TSY);

    GLUtils::detachProgram ();

    glDisable (GL_BLEND);
    glColor4fv (col);
    glLineWidth (1.0f);
  }
#endif
#if 0
  void drawHistogram (const UI::Bounds& region, const Param& attributeName, const IntegerSetType attributeID, const bool fill = true) const
  {
#if 1
    drawSparkline (region, attributeID, "hst\0", "hmx\0");
#else
  //  if ((maps && brushName) == false)          {return;}
    if ((gpuMemBacking () && "hst\0") == false)      {return;}
    if ((shaders () && "HistogramPlot\0") == false)    {return;}

    const GLUtils::ShaderProfile&  hist = shaders ()["HistogramPlot\0"];

    float      vp[4] ={0.0f};
    float      mv[16]={0.0f};
    float      pr[16]={0.0f};
    float      otl[4] = {0.0f};
    float      itl[2] = {0.0f};
    float      col[4] = {0.0f};

    glGetFloatv (GL_VIEWPORT, vp);
    glGetFloatv (GL_PROJECTION_MATRIX, pr);
    glGetFloatv (GL_MODELVIEW_MATRIX, mv);
    glGetFloatv (GL_CURRENT_COLOR, col);

    glEnable (GL_BLEND);

    // set the shader up
    GLUtils::linkProgram (hist);
    GLUtils::attachProgram (hist);
    // set the uniforms
    glUniformMatrix4fv (0, 1, GL_FALSE, pr);
    glUniformMatrix4fv (1, 1, GL_FALSE, mv);              GLE;

    // set the textures
    glActiveTexture (GL_TEXTURE0);  glBindTexture (GL_TEXTURE_1D, gpuMemBacking ()["min\0"]);
    glActiveTexture (GL_TEXTURE1);  glBindTexture (GL_TEXTURE_1D, gpuMemBacking ()["max\0"]);
    glActiveTexture (GL_TEXTURE2);  glBindTexture (GL_TEXTURE_2D, gpuMemBacking ()["hst\0"]);
    glActiveTexture (GL_TEXTURE3);  glBindTexture (GL_TEXTURE_1D, gpuMemBacking ()["hmx\0"]);
#if 0
    UI::Bounds::draw (GL_PATCHES, region);
#else
    float  mn[2];
    float  mx[2];

    switch (region.orientation ())
    {
    case UI::ZERO:
      mn[0] = region.left ();
      mx[0] = region.right ();
      mn[1] = region.bottom ();
      mx[1] = region.top ();
      break;
    case UI::RIGHT90:
      mn[0] = region.right ();
      mx[0] = region.left ();
      mn[1] = region.bottom ();
      mx[1] = region.top ();
      break;
    case UI::LEFT90:
      mn[0] = region.left ();
      mx[0] = region.right ();
      mn[1] = region.bottom ();
      mx[1] = region.top ();
      break;
    case UI::INVERT:
      mn[0] = region.left ();
      mx[0] = region.right ();
      mn[1] = region.top ();
      mx[1] = region.bottom ();
      break;
    default:
      mn[0] = 0.0f;
      mn[1] = 0.0f;
      mx[0] = 1.0f;
      mx[1] = 1.0f;
      break;
    }

    // set the uniforms
    glUniform2f (2, mn[0], mn[1]);    // min
    glUniform2f (3, mx[0], mx[1]);    // max
    glUniform1i (4, TSY);        // count of entries in the hgram
    glUniform1i (5, static_cast<GLint>(attributeID));    // data attribute
    glUniform4fv (6, 1, col);

    // fuck
  //  glPolygonMode (GL_FRONT, GL_LINE);
  //  glPolygonMode (GL_BACK, GL_LINE);

    // push BIN_COUNT number of points onto the gpu and let the shaders do their stuff
    glDrawArrays (GL_LINE_STRIP, 0, TSY);

  //  GLUtils::unbindBuffer (GL_ARRAY_BUFFER);
#endif

    GLUtils::detachProgram ();
    glActiveTexture (GL_TEXTURE0);

    glDisable (GL_BLEND);
    glColor4fv (col);
    glLineWidth (1.0f);
#endif
  }
#endif
#ifdef __GL_H__
  static void drawAttribute (const Mutables::Property& min,
                             const Mutables::Property& max,
                             const GLUtils::ShaderProfile& plotHistogramShader,
                             const GLUtils::TextureDescription& minValues,
                             const GLUtils::TextureDescription& maxValues,
                             const GLUtils::TextureDescription& hdata,
                             const GLUtils::TextureDescription& hmaxs,
                             const Param& attributeName,
                             const IntegerSetType attributeID,
                             const UI::Bounds& drawRegion,
                             const UI::DrawStyle& drawStyle)
  {
    UI::Bounds    datRegion;    // region between the data bounds

    if ((drawRegion.width () == 0) || (drawRegion.height () == 0))      {return;}

    // get the user range and the data range bounds
#if 0
    UI::Bounds    selRegion;    // region between the user bounds

    Mutables::DRW_Attribute::attributeBound (attributeName, min, max, drawRegion, datRegion, selRegion);
#else
    datRegion = drawRegion;
#endif
    // draw the histogram inside the data area
    glColor4fv (drawStyle.frame ());
    drawSparkline (plotHistogramShader, minValues, maxValues, hdata, hmaxs, datRegion, attributeID);

    // draw the regions
    glLineWidth (2.0f);
    glColor4f (1.0f, 1.0f, 1.0f, 1.0f);
    UI::Bounds::drawShortEdges (GL_LINES, datRegion);
#if 0
    glColor4f (1.0f, 0.0f, 0.0f, 0.6f);      UI::Bounds::drawShortEdges (GL_LINES, selRegion);
#endif
    glLineWidth (1.0f);

    // label the ranges
    glColor4fv (drawStyle.text ());
    Mutables::DRW_Range::drawRange (datRegion, min.str (), max.str ());
  }
#endif
public:
  DRW_Attribute ()
  {}
  void operator() (void)
  {
#ifdef __GL_H__
    switch (drawRegion ().orientation ())
    {
      case UI::ZERO:
        glLineWidth (2);  UI::Bounds::drawBorder (GL_LINES, drawRegion (), UI::BOTTOM);       glLineWidth (1);
        glLineWidth (2);  UI::Bounds::drawNotches (GL_LINES, drawRegion (), UI::BOTTOM, 10);  glLineWidth (1);
        UI::Display::drawText (drawRegion (), value ().c_str (), GLUtils::Text::CENTER, GLUtils::Text::MIDDLE, 30.0f);
        break;
      case UI::LEFT90:
        glLineWidth (2);  UI::Bounds::drawBorder (GL_LINES, drawRegion (), UI::LEFT);         glLineWidth (1);
        glLineWidth (2);  UI::Bounds::drawNotches (GL_LINES, drawRegion (), UI::LEFT, 10);    glLineWidth (1);
        UI::Display::drawText (drawRegion (), value ().c_str (), GLUtils::Text::CENTER, GLUtils::Text::MIDDLE, 30.0f);
        break;
      case UI::RIGHT90:
        glLineWidth (2);  UI::Bounds::drawBorder (GL_LINES, drawRegion (), UI::RIGHT);        glLineWidth (1);
        glLineWidth (2);  UI::Bounds::drawNotches (GL_LINES, drawRegion (), UI::RIGHT, 10);   glLineWidth (1);
        UI::Display::drawText (drawRegion (), value ().c_str (), GLUtils::Text::CENTER, GLUtils::Text::MIDDLE, 30.0f);
        break;
      case UI::INVERT:
        glLineWidth (2);  UI::Bounds::drawBorder (GL_LINES, drawRegion (), UI::TOP);          glLineWidth (1);
        glLineWidth (2);  UI::Bounds::drawNotches (GL_LINES, drawRegion (), UI::TOP, 10);     glLineWidth (1);
        UI::Display::drawText (drawRegion (), value ().c_str (), GLUtils::Text::CENTER, GLUtils::Text::MIDDLE, 30.0f);
        break;
    }
#endif
  }
};
class DRW_ColourMap : public Mutables::Drawer
{
public:
  DRW_ColourMap ()
  {}
  void operator() (void)
  {
#ifdef GLUTILS_H
    GLUtils::TextureDescription    txd;
    float      otl[4] = {0.0f};
    float      itl[2] = {0.0f};
    float      col[4] = {0.0f};

  //  if (Property::getImage (txd, value ()) == false)
#if 0
    idx = static_cast<GLuint>(descriptionSet[Types::colourMap ()].values ()[value ()]);

    txd = GLUtils::TextureDescription (GL_TEXTURE_1D, idx);
#else
    if ((repositorySet && value ()) == false)
    {
      setReturnValue (FALSE);
      return;
    }

    Mutables::PropertySet&  clrm = repositorySet[value ()];

    if ((clrm && "Image\0") == false)
    {
      setReturnValue (FALSE);
      return;
    }

    if (clrm["Image\0"].getImage (txd) == false)
    {
      setReturnValue (FALSE);
      return;
    }
#endif

    glGetFloatv (GL_CURRENT_COLOR, col);

    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glColor4f (1.0f, 1.0f, 1.0f, 0.7f);

    // draw the colour map
    glEnable (GL_TEXTURE_1D);
    glTexEnvi (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  //  glBindTexture (GL_TEXTURE_1D, clrs[value ()]["Image\0"].getImage ());

    txd.bind ();

    glBegin (GL_QUADS);
    // FIXME: This colourmap size and location is a ratio of the draw region, make it fixed
    glTexCoord1f (0.0f); glVertex2f (drawRegion ().left (), drawRegion ().bottom ());
    glTexCoord1f (0.0f); glVertex2f (drawRegion ().right (), drawRegion ().bottom ());
    glTexCoord1f (1.0f); glVertex2f (drawRegion ().right (), drawRegion ().top ());
    glTexCoord1f (1.0f); glVertex2f (drawRegion ().left (), drawRegion ().top ());
    glEnd ();
#if 0
    // add info around the object if selected
    if (0)//(selected == true)
    {
      UI::Bounds    c;
      GLUtils::TextureDescription  txd;

      // draw other colourmaps around this one

      // draw the after ones
      Mutables::Description::ValueList&  vals = Mutables::Property::descriptionSet[Mutables::Types::colourMap ()].values ();
      Mutables::Description::ValueList::ConstParamList  cl, cl_beg, cl_end;

      cl_beg = vals.begin ();
      cl_end = vals.end ();

    //  if ((cl = std::find (cl_beg, cl_end, value ())) != cl_end)
      if ((cl = vals.find (value ())) != cl_end)
      {
        c = drawRegion ();
        c.offset (drawRegion ().width () + UI::Display::spacing (), 0.0f);
        c.scale (0.75f, 0.75f);

        while (++cl != cl_end)
        {
        //  glBindTexture (GL_TEXTURE_1D, clrs[*cl]["Image\0"].getImage ());
          clrs[cl->first]["Image\0"].getImage ().bind ();
          glBegin (GL_QUADS);
          // FIXME: This colourmap size and location is a ratio of the draw region, make it fixed
          glTexCoord1f (0.0f); glVertex2f (c.left (), c.bottom ());
          glTexCoord1f (0.0f); glVertex2f (c.right (), c.bottom ());
          glTexCoord1f (1.0f); glVertex2f (c.right (), c.top ());
          glTexCoord1f (1.0f); glVertex2f (c.left (), c.top ());
          glEnd ();

          c.offset (c.width () + UI::Display::spacing (), 0.0f);
        //  ++cl;
        }
      }

    //  if (((cl = std::find (cl_beg, cl_end, v)) != cl_end) &&
      if (((cl = vals.find (v)) != cl_end) &&
        (cl-- != cl_beg))
      {
        c = drawRegion ();
        c.offset (-(drawRegion ().width () + UI::Display::spacing ()), 0.0f);
        c.scale (0.75f, 0.75f);

        while (cl != cl_end)
        {
#if 0
          glBindTexture (GL_TEXTURE_1D, clrs[*cl]["Image\0"].getImage ());
#else
          clrs[cl->first]["Image\0"].getImage (txd);
          txd.bind ();
#endif
          glBegin (GL_QUADS);
          // FIXME: This colourmap size and location is a ratio of the draw region, make it fixed
          glTexCoord1f (0.0f); glVertex2f (c.left (), c.bottom ());
          glTexCoord1f (0.0f); glVertex2f (c.right (), c.bottom ());
          glTexCoord1f (1.0f); glVertex2f (c.right (), c.top ());
          glTexCoord1f (1.0f); glVertex2f (c.left (), c.top ());
          glEnd ();

          c.offset (-(c.width () + UI::Display::spacing ()), 0.0f);

          if (cl == cl_beg)      {cl = cl_end;}
          else            {--cl;}
        }
      }

    //  glDisable (GL_TEXTURE_1D);
    }
#endif
    glTexEnvi (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glDisable (GL_TEXTURE_1D);
    glDisable (GL_BLEND);

    glColor4fv (col);
#endif
  }
};
class DRW_DataSource : public Mutables::Drawer
{
public:
  DRW_DataSource ()
  {}
  void operator() (void)
  {
#ifdef GLUTILS_H
    UI::Bounds::draw (GL_LINE_LOOP, drawRegion ());
    UI::Display::drawText (drawRegion (), value ().c_str (), GLUtils::Text::RIGHT, GLUtils::Text::MIDDLE);
#endif
  }
};
class DRW_Bound : public Drawer
{
protected:
public:
  DRW_Bound ()
  {}
  void operator() (void)
  {
#ifdef GLUTILS_H
    UI::Bounds::draw (GL_LINE_LOOP, drawRegion ());
#endif
    setReturnValue (TRUE);
  }
};

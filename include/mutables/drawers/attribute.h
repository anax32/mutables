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
                             drawRegion.bottom () - (drawRegion.height () * (brng[1] - mn[0])/(mx[1]-mn[0])),
                             drawRegion.bottom () - (drawRegion.height () * (brng[0] - mn[0])/(mx[1]-mn[0])));
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

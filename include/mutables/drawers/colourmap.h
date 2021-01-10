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

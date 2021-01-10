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

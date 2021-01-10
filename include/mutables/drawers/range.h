class DRW_Range : public Drawer
{
public:
  static void drawRange(const UI::Bounds& region, const Param& min, const Param& max)
  {
    /**
     * draw a range (min -> max) inside the bounding region
     */
#ifdef __GL_H__
    const float label_width=0.4f;  // label width
    UI::Bounds  bound;
    const UI::Rotation rotation = region.orientation ();

    if ((region.width() == 0) || (region.height() == 0))
    {
      return;
    }

    // MIN
    switch(rotation)
    {
    case UI::ZERO:
    case UI::INVERT:
      bound.setRegion(
          region.left(),
          region.top(),
          region.left()+region.width()*label_width,
          region.bottom());
      break;
    case UI::LEFT90:
      bound.setRegion(
          region.left(),
          region.top(),
          region.right(),
          region.top()+region.height()*label_width);
      break;
    case UI::RIGHT90:
      bound.setRegion(
          region.left(),
          region.top()+region.height()*label_width,
          region.right(),
          region.top());
      break;
    }

    UI::Display::drawText(
        bound,
        min.c_str(),
        GLUtils::Text::LEFT,
        GLUtils::Text::MIDDLE);

    // MAX
    switch (rotation)
    {
    case UI::ZERO:
    case UI::INVERT:
      bound.setRegion(
          region.right()-region.width()*label_width,
          region.top(),
          region.right(),
          region.bottom());
      break;
    case UI::LEFT90:
      bound.setRegion(
          region.left(),
          region.bottom()-region.height()*label_width,
          region.right(),
          region.bottom());
      break;
    case UI::RIGHT90:
      bound.setRegion(
          region.left(),
          region.bottom(),
          region.right(),
          region.bottom()-region.height()*label_width);
      break;
    }

    UI::Display::drawText(
        bound,
        max.c_str(),
        GLUtils::Text::RIGHT,
        GLUtils::Text::MIDDLE);
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

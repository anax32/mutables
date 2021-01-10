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

    if (b.width() == 0 || b.height() == 0)
    {
      return;
    }

    switch (b.orientation ())
    {
#ifdef __GL_H__
      case UI::ZERO:
        UI::Display::drawText(
            drawRegion(),
            value().c_str(),
            GLUtils::Text::LEFT,
            GLUtils::Text::MIDDLE,
            30.0f);
        break;
      case UI::LEFT90:
        UI::Display::drawText(
            drawRegion(),
            value ().c_str(),
            GLUtils::Text::LEFT,
            GLUtils::Text::MIDDLE,
            30.0f);
        break;
      case UI::RIGHT90:
        UI::Display::drawText(
            drawRegion(),
            value().c_str(),
            GLUtils::Text::LEFT,
            GLUtils::Text::MIDDLE,
            30.0f);
        break;
#endif
      default:
        break;
    }
  }
};

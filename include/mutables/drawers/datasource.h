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

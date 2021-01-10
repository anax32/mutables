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

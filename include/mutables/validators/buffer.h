class VLD_Buffer : public Validator
{
public:
  virtual void operator() (void)
  {
#ifdef __GL_H__
    GLUtils::BufferDescription  buf;

    setResult (Mutables::Property::getBuffer (buf, value ()));
    setResult (glIsBuffer (buf.index ()) == GL_TRUE);
#endif
  }
};

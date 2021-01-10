class VLD_Image : public Validator
{
public:
  virtual void operator() (void)
  {
#ifdef __GL_H__
    GLUtils::TextureDescription  txd;

    setResult (Mutables::Property::getImage (txd, value ()));
    setResult (glIsTexture (txd.index ()) == GL_TRUE);
#endif
  }
};

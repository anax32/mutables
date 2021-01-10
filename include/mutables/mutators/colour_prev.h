class MUT_Colour_Prev : public Mutator
{
protected:
  const Param type;
public:
  MUT_Colour_Prev (const Param typeTarget)
    : type (typeTarget)
  {}
  void operator() (void)
  {
    float  rgba[4];
    float  rgb[3], hsv[3];

    Property::getColour (value (), rgba);
    rgb[0]=rgba[0];rgb[1]=rgba[1];rgb[2]=rgba[2];
    ColourUtils::rgb2hsv (rgb, hsv);

    if (alt () == true)    {hsv[1]-=0.01f;}
    else          {hsv[0]-=2.00f;}

    while (hsv[0]<0.0f)    {hsv[0]+=360.0f;}
    while (hsv[1]<0.0f)    {hsv[1]+=1.0f;}

    ColourUtils::hsv2rgb (hsv, rgb);
    rgba[0]=rgb[0];rgba[1]=rgb[1];rgba[2]=rgb[2];
    ColourUtils::float2hex (rgba, val);
  }
};

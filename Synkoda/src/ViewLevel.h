#ifndef view_level_h_
#define view_level_h_

#include <WS2812Serial.h>

class ViewLevel
{
  public:
    ViewLevel(
      WS2812Serial* pixels,
      uint8_t firstIndex,
      uint8_t rotation,
      uint8_t red = 50,
      uint8_t green = 50,
      uint8_t blue = 50
      );

  void setLevel(int level);
  void setColour(uint8_t red, uint8_t green, uint8_t blue);

   private:
       WS2812Serial* _pixels;
       uint8_t _firstIndex;
       uint8_t _rotation;
       uint8_t _red;
       uint8_t _green;
       uint8_t _blue;
       unsigned long _lastFlush;

};

#endif

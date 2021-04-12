#ifndef display_h_
#define display_h_

#include <WS2812Serial.h>
#define USE_WS2812SERIAL
#include <FastLED.h>

class Display
{
  public:
    Display();

  void setLevel(int ring, int level);
  void setColour(int ring, CHSV colour);

  private:
    CRGB _leds[80];
    int _indexes[5]= {48, 32, 0, 16, 64};
    int _rotations[5]= {9, 11, 12, 13, 5};
    int _seeds[80];
    CHSV _colours[5]= {
      CHSV(100, 255, 255),
      CHSV(100, 255, 255),
      CHSV(100, 255, 255),
      CHSV(100, 255, 255),
      CHSV(100, 0, 255)
    };
    unsigned long _lastAnimate;
    unsigned long _lastFlush;

};

#endif

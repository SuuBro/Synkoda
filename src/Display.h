#ifndef display_h_
#define display_h_

#include <WS2812Serial.h>
#define USE_WS2812SERIAL
#include <FastLED.h>

enum DisplayMode
{
  Range,
  Option,
  Balance
};

class DisplaySettings
{
public:
  DisplaySettings(int max)
  {
    Max = max;
  }

  int Min;
  int Max;
};

class Display
{
public:
  Display();

  void setMode(DisplayMode mode, DisplaySettings settings);
  void setLevel(int ring, int level);
  void setColour(int ring, CHSV colour);

private:
  CRGB _leds[80];
  int _indexes[5] = {48, 32, 16, 0, 64};
  int _rotations[5] = {9, 11, 12, 13, 5};
  int _seeds[80];
  CHSV _colours[5] = {
      CHSV(100, 255, 100),
      CHSV(100, 255, 100),
      CHSV(100, 255, 100),
      CHSV(100, 255, 100),
      CHSV(100, 0, 150)};
  DisplayMode _modes[5] = {
      DisplayMode::Range,
      DisplayMode::Range,
      DisplayMode::Range,
      DisplayMode::Range,
      DisplayMode::Option};
  DisplaySettings _settings[5] = {
      DisplaySettings(127),
      DisplaySettings(127),
      DisplaySettings(127),
      DisplaySettings(127),
      DisplaySettings(7),};
  unsigned long _lastAnimate;
  unsigned long _lastFlush;
};

#endif

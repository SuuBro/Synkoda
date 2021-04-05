#include "Display.h"

const int ledPin = 24;
const int numRings = 5;
const int numLedsPerRing = 16;

const float maxLevel = 16.0;
const float maxMidiValue = 127.0;
const float subDivisions = maxMidiValue/maxLevel;

Display::Display()
{
  LEDS.addLeds<WS2812SERIAL,ledPin,RGB>(_leds, numRings*numLedsPerRing);
  _lastFlush = millis();
  _lastAnimate = millis();
  for (size_t i = 0; i < 80; i++)
  {
    _seeds[i] = random(3, 25);
  }
}

void Display::setLevel(int ring, int level)
{
  int index = _indexes[ring];
  int rotation = _rotations[ring];
  int hue = hues[ring];

  int numLights = ((level / maxMidiValue) * 16.0);
  int mod = (level - (numLights * subDivisions));
  int extra = mod == 0 ? 0 : map(mod, 0.0, subDivisions, 4.1, subDivisions);
  for (int l = index + (numLedsPerRing-1); l >= index; --l)
  {
    int rotated = l - rotation;
    rotated = rotated < 0 ? rotated + numLedsPerRing : rotated;
    if (numLedsPerRing - rotated % numLedsPerRing == numLights)
    {
      _leds[l] = CHSV(hue, 255, 255);
    }
    else if (numLedsPerRing - rotated % numLedsPerRing < numLights)
    {
      _leds[l] = CHSV(hue+_seeds[l]-12, 255, _seeds[l]*10);
    }
    else if (numLedsPerRing - rotated % numLedsPerRing == numLights+1)
    {
      _leds[l] = CHSV(hue, 255, 255/8.0 * extra);
    }
    else
    {
      _leds[l] = CRGB::Black;
    }
  }

  if (millis() - _lastFlush > 20)
  {
    _lastFlush = millis();
    FastLED.show();
  }

  if (millis() - _lastAnimate > 20)
  {
    _lastAnimate = millis();
    for (size_t i = 0; i < 80; i++)
    {
      _seeds[i] = min(max((_seeds[i] + random(0, 6)-3), 8), 25);
    }
  }
}

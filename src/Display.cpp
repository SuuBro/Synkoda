#include "Display.h"

const int ledPin = 24;
const int numRings = 5;
const int numLedsPerRing = 16;

const float maxLevel = 16.0;

Display::Display()
{
  LEDS.addLeds<WS2812SERIAL, ledPin, RGB>(_leds, numRings * numLedsPerRing);
  _lastFlush = millis();
  _lastAnimate = millis();
  for (size_t i = 0; i < 80; i++)
  {
    _seeds[i] = random(3, 25);
  }
}

void Display::setColour(int ring, CHSV colour)
{
  _colours[ring] = colour;
}

void Display::setLevel(int ring, int level)
{
  const int index = _indexes[ring];
  const int rotation = _rotations[ring];
  const CHSV colour = _colours[ring];
  const DisplaySettings settings = _settings[ring];

  if (_modes[ring] == DisplayMode::Range)
  {
    const float subDivisions = settings.Max / maxLevel;
    const int numLights = ((level / (float)settings.Max) * maxLevel);
    const int mod = (level - (numLights * subDivisions));
    const int extra = mod == 0 ? 0 : map(mod, 0.0, subDivisions, 4.1, subDivisions);
    for (int l = index + (numLedsPerRing - 1); l >= index; --l)
    {
      int rotated = l - rotation;
      rotated = rotated < 0 ? rotated + numLedsPerRing : rotated;
      if (numLedsPerRing - rotated % numLedsPerRing == numLights)
      {
        _leds[l] = colour;
      }
      else if (numLedsPerRing - rotated % numLedsPerRing < numLights)
      {
        _leds[l] = CHSV(colour.h + _seeds[l] - 12, colour.s, _seeds[l] * 10);
      }
      else if (numLedsPerRing - rotated % numLedsPerRing == numLights + 1)
      {
        _leds[l] = CHSV(colour.h, colour.s, 255 / 8.0 * extra);
      }
      else
      {
        _leds[l] = CRGB::Black;
      }
    }
  }
  else if(_modes[ring] == DisplayMode::Option)
  {
    const int lightsPerOption = numLedsPerRing/(settings.Max+1);
    const int firstIndex = lightsPerOption * level;
    const int lastIndex = firstIndex + lightsPerOption;

    for (int l = index + (numLedsPerRing - 1); l >= index; --l)
    {
      int rotated = l - rotation - 1;
      rotated = rotated < 0 ? rotated + numLedsPerRing : rotated;
      if (numLedsPerRing - rotated % numLedsPerRing >= firstIndex+1
       && numLedsPerRing - rotated % numLedsPerRing < lastIndex+1)
      {
        _leds[l] = colour;
      }
      else {
        _leds[l] = CRGB::Black;
      }
    }
  }

  if (millis() - _lastFlush > 20)
  {
    _lastFlush = millis();
    FastLED.show();
  }

  if (millis() - _lastAnimate > 100)
  {
    _lastAnimate = millis();
    for (size_t i = 0; i < 80; i++)
    {
      _seeds[i] = min(max((_seeds[i] + random(0, 6) - 3), 8), 25);
    }
  }
}

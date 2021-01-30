#include "ViewLevel.h"

ViewLevel::ViewLevel(
    WS2812Serial *pixels,
    uint8_t firstIndex,
    uint8_t rotation,
    uint8_t red,
    uint8_t green,
    uint8_t blue)
{
  _pixels = pixels;
  _firstIndex = firstIndex;
  _rotation = rotation;
  _red = red;
  _green = green;
  _blue = blue;
  _lastFlush = millis();
}

int rgb(int r, int g, int b){
  int rgb = r;
  rgb = (rgb << 8) + g;
  rgb = (rgb << 8) + b;
  return rgb;
}

void ViewLevel::setLevel(int level)
{
  int numLights = ((level / 127.0) * 16.0);
  int mod = (level - (numLights * 8.0));
  int extra = mod == 0 ? 0 : map(mod, 0.0, 8.0, 2.1, 8.0);
  for (int l = _firstIndex + 15; l >= _firstIndex; --l)
  {
    int rotated = l - _rotation;
    rotated = rotated < 0 ? rotated + 16 : rotated;
    if (16 - rotated % 16 <= numLights)
    {
      _pixels->setPixelColor(l, rgb(_red, _green, _blue));

    }
    else if (16 - rotated % 16 == numLights+1)
    {
      _pixels->setPixelColor(l, rgb(_red / 8.0 * extra, _green / 8.0 * extra, _blue / 8.0 * extra));
    }
    else
    {
      _pixels->setPixelColor(l, rgb(0, 0, 0));
    }
  }
  if (millis() - _lastFlush > 20)
  {
    _lastFlush = millis();
    _pixels->show();
  }
}

void ViewLevel::setColour(uint8_t red, uint8_t green, uint8_t blue)
{
  _red = red;
  _green = green;
  _blue = blue;
}

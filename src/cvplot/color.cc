#include "cvplot/color.h"

#include <cmath>

namespace cvplot {

Color Color::alpha(uint8_t alpha) const { return Color(r, g, b, alpha); }

Color Color::gray(uint8_t v) { return Color(v, v, v); }

Color Color::index(uint32_t index, uint32_t density, float avoid,
                   float range) {  // avoid greens by default
  if (avoid > 0) {
    auto step = density / (6 - range);
    auto offset = (avoid + range / 2) * step;
    index = offset + index % density;
    density += step * range;
  }
  auto hue = index % density * 6.f / density;
  return Color::cos(hue);
}

Color Color::hash(const std::string &seed) {
  return Color::index(std::hash<std::string>{}(seed));
}

Color Color::hue(float hue) {
  Color color;
  auto i = (int)hue;
  auto f = (hue - i) * (255 - paleness * 2) + paleness;
  switch (i) {
    case 0:
      color.r = 255 - paleness;
      color.g = f;
      color.b = paleness;
      break;
    case 1:
      color.r = 255 - f;
      color.g = 255 - paleness;
      color.b = paleness;
      break;
    case 2:
      color.r = paleness;
      color.g = 255 - paleness;
      color.b = f;
      break;
    case 3:
      color.r = paleness;
      color.g = 255 - f;
      color.b = 255 - paleness;
      break;
    case 4:
      color.r = f;
      color.g = paleness;
      color.b = 255 - paleness;
      break;
    case 5:
    default:
      color.r = 255 - paleness;
      color.g = paleness;
      color.b = 255 - f;
      break;
  }
  return color;
}

Color Color::cos(float hue) {
  return Color((std::cos(hue * 1.047f) + 1) * 127.9f,
               (std::cos((hue - 2) * 1.047f) + 1) * 127.9f,
               (std::cos((hue - 4) * 1.047f) + 1) * 127.9f);
}

float Color::hue() const {
  auto min = std::min({r, g, b});
  auto max = std::max({r, g, b});
  if (min == max) {
    return 0;
  }
  auto hue = 0.f;
  if (r == max) {
    hue = (g - b) / (float)(max - min);
  } else if (g == max) {
    hue = 2.f + (b - r) / (float)(max - min);
  } else {
    hue = 4.f + (r - g) / (float)(max - min);
  }
  if (hue < 0) {
    hue += 6;
  }
  return hue;
}

}  // namespace cvplot

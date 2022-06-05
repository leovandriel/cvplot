#include "cvplot/color.h"

#include <cmath>
#include <map>

namespace cvplot {

namespace {
std::map<std::string, int> color_counter;
}

auto Color::alpha(uint8_t alpha) const -> Color { return {r, g, b, alpha}; }

auto Color::gamma(float gamma) const -> Color {
  return {static_cast<uint8_t>(pow(r / 255.F, 1 / gamma) * 255),
          static_cast<uint8_t>(pow(g / 255.F, 1 / gamma) * 255),
          static_cast<uint8_t>(pow(b / 255.F, 1 / gamma) * 255), a};
}

auto Color::gray(uint8_t v) -> Color { return {v, v, v}; }

auto Color::index(uint8_t index, uint8_t density, float avoid,
                  float range) -> Color {  // avoid greens by default
  if (avoid > 0) {
    auto step = density / (6 - range);
    auto offset = (avoid + range / 2) * step;
    index = offset + index % density;
    density += step * range;
  }
  auto hue = index % density * 6.F / density;
  return Color::cos(hue);
}

auto Color::hash(const std::string &seed) -> Color {
  unsigned hash = 37;
  for (char c : seed) {
    hash = (hash * 54059) ^ (c * 76963);
  }
  return Color::index(hash);
}

auto Color::uniq(const std::string &name) -> Color {
  if (color_counter.count(name) == 0) {
    color_counter[name] = color_counter.size();
  }
  return Color::index(color_counter[name]);
}

auto Color::hue(float hue) -> Color {
  Color color;
  auto i = static_cast<int>(hue);
  auto f = static_cast<uint8_t>((hue - i) * 255);
  switch (i % 6) {
    case 0:
      return {255, f, 0};
    case 1:
      return {static_cast<uint8_t>(255 - f), 255, 0};
    case 2:
      return {0, 255, f};
    case 3:
      return {0, static_cast<uint8_t>(255 - f), 255};
    case 4:
      return {f, 0, 255};
    case 5:
      return {255, 0, static_cast<uint8_t>(255 - f)};
  }
  return {};
}

auto Color::cos(float hue) -> Color {
  return {static_cast<uint8_t>((std::cos(hue * 1.047F) + 1) * 127.9F),
          static_cast<uint8_t>((std::cos((hue - 2) * 1.047F) + 1) * 127.9F),
          static_cast<uint8_t>((std::cos((hue - 4) * 1.047F) + 1) * 127.9F)};
}

auto Color::hue() const -> float {
  auto min = std::min(std::min(r, g), b);
  auto max = std::max(std::max(r, g), b);
  if (min == max) {
    return 0;
  }
  auto hue = 0.F;
  if (r == max) {
    hue = (g - b) / static_cast<float>(max - min);
  } else if (g == max) {
    hue = 2.F + (b - r) / static_cast<float>(max - min);
  } else {
    hue = 4.F + (r - g) / static_cast<float>(max - min);
  }
  if (hue < 0) {
    hue += 6;
  }
  return hue;
}

}  // namespace cvplot

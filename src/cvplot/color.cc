#include "cvplot/color.h"

#include <cmath>
#include <map>

namespace cvplot {

namespace {
std::map<std::string, int> color_counter;
}

auto Color::alpha(uint8_t alpha) const -> Color { return {r, g, b, alpha}; }

auto Color::gamma(double gamma) const -> Color {
  return {static_cast<uint8_t>(pow(r / 255., 1 / gamma) * 255),
          static_cast<uint8_t>(pow(g / 255., 1 / gamma) * 255),
          static_cast<uint8_t>(pow(b / 255., 1 / gamma) * 255), a};
}

auto Color::gray(uint8_t v) -> Color { return {v, v, v}; }

auto Color::index(uint8_t index, uint8_t density, double avoid,
                  double range) -> Color {  // avoid greens by default
  if (avoid > 0) {
    auto step = density / (6 - range);
    auto offset = (avoid + range / 2) * step;
    index = offset + index % density;
    density += step * range;
  }
  auto hue = index % density * 6. / density;
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

auto Color::hue(double hue) -> Color {
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

auto Color::cos(double hue) -> Color {
  return {static_cast<uint8_t>((std::cos(hue * 1.047) + 1) * 127.9),
          static_cast<uint8_t>((std::cos((hue - 2) * 1.047) + 1) * 127.9),
          static_cast<uint8_t>((std::cos((hue - 4) * 1.047) + 1) * 127.9)};
}

auto Color::hue() const -> double {
  auto min = std::min(std::min(r, g), b);
  auto max = std::max(std::max(r, g), b);
  if (min == max) {
    return 0;
  }
  auto hue = 0.;
  if (r == max) {
    hue = (g - b) / static_cast<double>(max - min);
  } else if (g == max) {
    hue = 2. + (b - r) / static_cast<double>(max - min);
  } else {
    hue = 4. + (r - g) / static_cast<double>(max - min);
  }
  if (hue < 0) {
    hue += 6;
  }
  return hue;
}

}  // namespace cvplot

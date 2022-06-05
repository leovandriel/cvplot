#ifndef CVPLOT_COLOR_H
#define CVPLOT_COLOR_H

#include <string>

namespace cvplot {

struct Color {
  uint8_t r, g, b, a;
  Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255)
      : r(r), g(g), b(b), a(a) {}
  Color(const uint8_t *rgb, uint8_t a = 255)
      : Color(rgb[0], rgb[1], rgb[2], a) {}
  Color() : Color(0, 0, 0) {}

  auto alpha(uint8_t alpha) const -> Color;
  auto gamma(double gamma) const -> Color;
  auto hue() const -> double;

  static auto gray(uint8_t v) -> Color;
  static auto hue(double hue) -> Color;
  static auto cos(double hue) -> Color;
  static auto index(uint8_t index, uint8_t density = 16, double avoid = 2.,
                    double range = 2.) -> Color;
  static auto hash(const std::string &seed) -> Color;
};

static const Color Red = Color::hue(0.);
static const Color Orange = Color::hue(.5);
static const Color Yellow = Color::hue(1.);
static const Color Lawn = Color::hue(1.5);
static const Color Green = Color::hue(2.);
static const Color Aqua = Color::hue(2.5);
static const Color Cyan = Color::hue(3.);
static const Color Sky = Color::hue(3.5);
static const Color Blue = Color::hue(4.);
static const Color Purple = Color::hue(4.5);
static const Color Magenta = Color::hue(5.);
static const Color Pink = Color::hue(5.5);
static const Color Black = Color::gray(0);
static const Color Dark = Color::gray(32);
static const Color Gray = Color::gray(128);
static const Color Light = Color::gray(223);
static const Color White = Color::gray(255);

}  // namespace cvplot

#endif  // CVPLOT_COLOR_H

#ifndef CVPLOT_FIGURE_H
#define CVPLOT_FIGURE_H

#include <map>
#include <string>
#include <utility>
#include <vector>

#include "color.h"
#include "window.h"

namespace cvplot {

struct Point2 {
  double x, y;
  Point2() : Point2(0, 0) {}
  Point2(double x, double y) : x(x), y(y) {}
};

struct Point3 {
  double x, y, z;
  Point3() : Point3(0, 0, 0) {}
  Point3(double x, double y, double z) : x(x), y(y), z(z) {}
};

enum Type {
  Line,
  DotLine,
  Dots,
  FillLine,
  RangeLine,
  Histogram,
  Vistogram,
  Horizontal,
  Vertical,
  Range,
  Circle,
};

class Series {
 public:
  Series(std::string label, enum Type type, Color color)
      : label_(std::move(label)),
        type_(type),
        color_(color),
        dims_(0),
        depth_(0),
        legend_(true),
        dynamic_color_(false) {}

  auto type(enum Type type) -> Series &;
  auto color(Color color) -> Series &;
  auto dynamicColor(bool dynamic_color) -> Series &;
  auto legend(bool legend) -> Series &;
  auto add(const std::vector<std::pair<double, double>> &data) -> Series &;
  auto add(const std::vector<std::pair<double, Point2>> &data) -> Series &;
  auto add(const std::vector<std::pair<double, Point3>> &data) -> Series &;
  auto addValue(const std::vector<double> &values) -> Series &;
  auto addValue(const std::vector<Point2> &values) -> Series &;
  auto addValue(const std::vector<Point3> &values) -> Series &;
  auto add(double key, double value) -> Series &;
  auto add(double key, Point2 value) -> Series &;
  auto add(double key, Point3 value) -> Series &;
  auto addValue(double value) -> Series &;
  auto addValue(double value_a, double value_b) -> Series &;
  auto addValue(double value_a, double value_b, double value_c) -> Series &;
  auto set(const std::vector<std::pair<double, double>> &data) -> Series &;
  auto set(const std::vector<std::pair<double, Point2>> &data) -> Series &;
  auto set(const std::vector<std::pair<double, Point3>> &data) -> Series &;
  auto setValue(const std::vector<double> &values) -> Series &;
  auto setValue(const std::vector<Point2> &values) -> Series &;
  auto setValue(const std::vector<Point3> &values) -> Series &;
  auto set(double key, double value) -> Series &;
  auto set(double key, double value_a, double value_b) -> Series &;
  auto set(double key, double value_a, double value_b, double value_c)
      -> Series &;
  auto setValue(double value) -> Series &;
  auto setValue(double value_a, double value_b) -> Series &;
  auto setValue(double value_a, double value_b, double value_c) -> Series &;
  auto clear() -> Series &;

  auto label() const -> const std::string &;
  auto legend() const -> bool;
  auto color() const -> Color;
  void draw(void *buffer, double x_min, double x_max, double y_min,
            double y_max, double x_axis, double xs, double xd, double ys,
            double yd, double y_axis, int unit, double offset) const;
  auto collides() const -> bool;
  void dot(void *b, int x, int y, int r) const;
  void bounds(double &x_min, double &x_max, double &y_min, double &y_max,
              int &n_max, int &p_max) const;
  void verifyParams() const;

 protected:
  void ensureDimsDepth(int dims, int depth);
  auto flipAxis() const -> bool;

 protected:
  std::vector<int> entries_;
  std::vector<double> data_;
  enum Type type_;
  Color color_;
  std::string label_;
  int dims_;
  int depth_;
  bool legend_;
  bool dynamic_color_;
};

class Figure {
 public:
  Figure(View &view)
      : view_(view),
        border_size_(50),
        background_color_(White),
        axis_color_(Black),
        sub_axis_color_(Light),
        text_color_(Black),
        include_zero_x_(true),
        include_zero_y_(true),
        aspect_square_(false),
        grid_size_(60),
        grid_padding_(20) {}

  auto clear() -> Figure &;
  auto origin(bool x, bool y) -> Figure &;
  auto square(bool square) -> Figure &;
  auto border(int size) -> Figure &;
  auto alpha(int alpha) -> Figure &;
  auto gridSize(int size) -> Figure &;
  auto backgroundColor(Color color) -> Figure &;
  auto axisColor(Color color) -> Figure &;
  auto subaxisColor(Color color) -> Figure &;
  auto textColor(Color color) -> Figure &;
  auto backgroundColor() -> Color;
  auto axisColor() -> Color;
  auto subaxisColor() -> Color;
  auto textColor() -> Color;

  void draw(void *buffer, double x_min, double x_max, double y_min,
            double y_max, int n_max, int p_max) const;
  auto drawFit(void *buffer) const -> int;
  auto drawFile(const std::string &filename, Size size) const -> bool;
  void show(bool flush = true) const;
  auto series(const std::string &label) -> Series &;

 protected:
  View &view_;
  std::vector<Series> series_;
  int border_size_;
  Color background_color_;
  Color axis_color_;
  Color sub_axis_color_;
  Color text_color_;
  bool include_zero_x_;
  bool include_zero_y_;
  bool aspect_square_;
  int grid_size_;
  int grid_padding_;
};

auto figure(const std::string &view) -> Figure &;

}  // namespace cvplot

#endif  // CVPLOT_FIGURE_H

#include "cvplot/figure.h"

#include <cmath>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "cvplot/window.h"
#include "internal.h"

namespace cvplot {

namespace {
std::map<std::string, Figure> shared_figures_;
}

void Series::verifyParams() const {
  auto dims = 1;
  auto depth = 0;
  switch (type_) {
    case Line:
    case DotLine:
    case Dots:
    case FillLine:
    case Vistogram:
    case Histogram:
    case Horizontal:
    case Vertical: {
      depth = 1;
      break;
    }
    case RangeLine: {
      depth = 3;
      break;
    }
    case Range:
    case Circle: {
      depth = 2;
      break;
    }
  }
  if (dynamic_color_) {
    depth += 1;
  }
  if (!entries_.empty()) {
    EXPECT_EQ(dims_, dims);
    EXPECT_EQ(depth_, depth);
  }
}

void Series::ensureDimsDepth(int dims, int depth) {
  if (dims_ != dims) {
    if (dims_ != 0) {
      std::cerr << "incorrect dims (input dimensions), was " << dims_ << " now "
                << dims << std::endl;
    }
    dims_ = dims;
  }
  if (depth_ != depth) {
    if (depth_ != 0) {
      std::cerr << "incorrect depth (output dimensions), was " << depth_
                << " now " << depth << std::endl;
    }
    depth_ = depth;
  }
}

auto Series::clear() -> Series & {
  entries_.clear();
  data_.clear();
  dims_ = 0;
  depth_ = 0;
  return *this;
}

auto Series::type(enum Type type) -> Series & {
  type_ = type;
  return *this;
}

auto Series::color(Color color) -> Series & {
  color_ = color;
  return *this;
}

auto Series::dynamicColor(bool dynamic_color) -> Series & {
  dynamic_color_ = dynamic_color;
  return *this;
}

auto Series::legend(bool legend) -> Series & {
  legend_ = legend;
  return *this;
}

auto Series::add(const std::vector<std::pair<float, float>> &data) -> Series & {
  ensureDimsDepth(1, 1);
  for (const auto &d : data) {
    entries_.push_back(data_.size());
    data_.push_back(d.first);
    data_.push_back(d.second);
  }
  return *this;
}

auto Series::add(const std::vector<std::pair<float, Point2>> &data)
    -> Series & {
  ensureDimsDepth(1, 2);
  for (const auto &d : data) {
    entries_.push_back(data_.size());
    data_.push_back(d.first);
    data_.push_back(d.second.x);
    data_.push_back(d.second.y);
  }
  return *this;
}

auto Series::add(const std::vector<std::pair<float, Point3>> &data)
    -> Series & {
  ensureDimsDepth(1, 3);
  for (const auto &d : data) {
    entries_.push_back(data_.size());
    data_.push_back(d.first);
    data_.push_back(d.second.x);
    data_.push_back(d.second.y);
    data_.push_back(d.second.z);
  }
  return *this;
}

auto Series::addValue(const std::vector<float> &values) -> Series & {
  std::vector<std::pair<float, float>> data(values.size());
  auto i = 0;
  for (auto &d : data) {
    d.first = i + entries_.size();
    d.second = values[i++];
  }
  return add(data);
}

auto Series::addValue(const std::vector<Point2> &values) -> Series & {
  std::vector<std::pair<float, Point2>> data(values.size());
  auto i = 0;
  for (auto &d : data) {
    d.first = i + entries_.size();
    d.second = values[i++];
  }
  return add(data);
}

auto Series::addValue(const std::vector<Point3> &values) -> Series & {
  std::vector<std::pair<float, Point3>> data(values.size());
  auto i = 0;
  for (auto &d : data) {
    d.first = i + entries_.size();
    d.second = values[i++];
  }
  return add(data);
}

auto Series::add(float key, float value) -> Series & {
  return add(std::vector<std::pair<float, float>>({{key, value}}));
}

auto Series::add(float key, Point2 value) -> Series & {
  return add(std::vector<std::pair<float, Point2>>({{key, value}}));
}

auto Series::add(float key, Point3 value) -> Series & {
  return add(std::vector<std::pair<float, Point3>>({{key, value}}));
}

auto Series::addValue(float value) -> Series & {
  return addValue(std::vector<float>({value}));
}

auto Series::addValue(float value_a, float value_b) -> Series & {
  return addValue(std::vector<Point2>({{value_a, value_b}}));
}

auto Series::addValue(float value_a, float value_b, float value_c) -> Series & {
  return addValue(std::vector<Point3>({{value_a, value_b, value_c}}));
}

auto Series::set(const std::vector<std::pair<float, float>> &data) -> Series & {
  clear();
  return add(data);
}

auto Series::set(const std::vector<std::pair<float, Point2>> &data)
    -> Series & {
  clear();
  return add(data);
}

auto Series::set(const std::vector<std::pair<float, Point3>> &data)
    -> Series & {
  clear();
  return add(data);
}

auto Series::setValue(const std::vector<float> &values) -> Series & {
  std::vector<std::pair<float, float>> data(values.size());
  auto i = 0;
  for (auto &d : data) {
    d.first = i;
    d.second = values[i++];
  }
  return set(data);
}

auto Series::setValue(const std::vector<Point2> &values) -> Series & {
  std::vector<std::pair<float, Point2>> data(values.size());
  auto i = 0;
  for (auto &d : data) {
    d.first = i;
    d.second = values[i++];
  }
  return set(data);
}

auto Series::setValue(const std::vector<Point3> &values) -> Series & {
  std::vector<std::pair<float, Point3>> data(values.size());
  auto i = 0;
  for (auto &d : data) {
    d.first = i;
    d.second = values[i++];
  }
  return set(data);
}

auto Series::set(float key, float value) -> Series & {
  return set(std::vector<std::pair<float, float>>({{key, value}}));
}

auto Series::set(float key, float value_a, float value_b) -> Series & {
  return set(
      std::vector<std::pair<float, Point2>>({{key, {value_a, value_b}}}));
}

auto Series::set(float key, float value_a, float value_b, float value_c)
    -> Series & {
  return set(std::vector<std::pair<float, Point3>>(
      {{key, {value_a, value_b, value_c}}}));
}

auto Series::setValue(float value) -> Series & {
  return setValue(std::vector<float>({value}));
}

auto Series::setValue(float value_a, float value_b) -> Series & {
  return setValue(std::vector<Point2>({{value_a, value_b}}));
}

auto Series::setValue(float value_a, float value_b, float value_c) -> Series & {
  return setValue(std::vector<Point3>({{value_a, value_b, value_c}}));
}

auto Series::label() const -> const std::string & { return label_; }

auto Series::legend() const -> bool { return legend_; }

auto Series::color() const -> Color { return color_; }

auto Series::collides() const -> bool {
  return type_ == Histogram || type_ == Vistogram;
}

auto Series::flipAxis() const -> bool {
  return type_ == Vertical || type_ == Vistogram;
}

void Series::bounds(float &x_min, float &x_max, float &y_min, float &y_max,
                    int &n_max, int &p_max) const {
  for (const auto &e : entries_) {
    auto xe = e;
    auto xd = dims_;
    auto ye = e + dims_;
    auto yd = depth_ - (dynamic_color_ ? 1 : 0);
    if (type_ == Circle) {
      yd = 1;
    }
    if (flipAxis()) {
      auto s = xe;
      xe = ye;
      ye = s;
      s = xd;
      xd = yd;
      yd = s;
    }
    if (type_ != Horizontal) {  // TODO(leo): check Horizontal/Vertical logic
      EXPECT_EQ(xd, 1);
      const auto &x = data_[xe];
      if (x_min > x) {
        x_min = x;
      }
      if (x_max < x) {
        x_max = x;
      }
    }
    if (type_ != Vertical) {
      for (auto yi = ye, _y = yi + yd; yi != _y; yi++) {
        const auto &y = data_[yi];
        if (y_min > y) {
          y_min = y;
        }
        if (y_max < y) {
          y_max = y;
        }
      }
    }
  }
  if (n_max < entries_.size()) {
    n_max = entries_.size();
  }
  if (type_ == Histogram || type_ == Vistogram) {
    p_max = std::max(30, p_max);
  }
}

void Series::dot(void *b, int x, int y, int r) const {
  Trans trans(b);
  cv::circle(trans.with(color_), {x, y}, r, color2scalar(color_), -1,
             cv::LINE_AA);
}

void Series::draw(void *b, float x_min, float x_max, float y_min, float y_max,
                  float xs, float xd, float ys, float yd, float x_axis,
                  float y_axis, int unit, float offset) const {
  if (dims_ == 0 || depth_ == 0) {
    return;
  }
  Trans trans(*static_cast<cv::Mat *>(b));
  auto color = color2scalar(color_);
  switch (type_) {
    case Line:
    case DotLine:
    case Dots:
    case FillLine:
    case RangeLine: {
      if (type_ == FillLine) {
        bool has_last = false;
        float last_x = NAN;
        float last_y = NAN;
        for (const auto &e : entries_) {
          auto x = data_[e];
          auto y = data_[e + dims_];
          if (dynamic_color_) {
            color = color2scalar(Color::cos(data_[e + dims_ + 1]));
          }
          cv::Point point(static_cast<int>(x * xs + xd),
                          static_cast<int>(y * ys + yd));
          if (has_last) {
            // NOLINTNEXTLINE(modernize-avoid-c-arrays)
            cv::Point points[4] = {
                point,
                {point.x, static_cast<int>(y_axis * ys + yd)},
                {static_cast<int>(last_x * xs + xd),
                 static_cast<int>(y_axis * ys + yd)},
                {static_cast<int>(last_x * xs + xd),
                 static_cast<int>(last_y * ys + yd)},
            };
            cv::fillConvexPoly(trans.with(color_.a / 2), points, 4, color,
                               cv::LINE_AA);
          } else {
            has_last = true;
          }
          last_x = x, last_y = y;
        }
      } else if (type_ == RangeLine) {
        bool has_last = false;
        float last_x = NAN;
        float last_y1 = NAN;
        float last_y2 = NAN;
        for (const auto &e : entries_) {
          auto x = data_[e];
          auto y1 = data_[e + dims_ + 1];
          auto y2 = data_[e + dims_ + 2];
          if (dynamic_color_) {
            color = color2scalar(Color::cos(data_[e + dims_ + 1]));
          }
          if (has_last) {
            // NOLINTNEXTLINE(modernize-avoid-c-arrays)
            cv::Point points[4] = {
                {static_cast<int>(x * xs + xd), static_cast<int>(y1 * ys + yd)},
                {static_cast<int>(x * xs + xd), static_cast<int>(y2 * ys + yd)},
                {static_cast<int>(last_x * xs + xd),
                 static_cast<int>(last_y2 * ys + yd)},
                {static_cast<int>(last_x * xs + xd),
                 static_cast<int>(last_y1 * ys + yd)},
            };
            cv::fillConvexPoly(trans.with(color_.a / 2), points, 4, color,
                               cv::LINE_AA);
          } else {
            has_last = true;
          }
          last_x = x, last_y1 = y1, last_y2 = y2;
        }
      }
      bool has_last = false;
      float last_x = NAN;
      float last_y = NAN;
      for (const auto &e : entries_) {
        auto x = data_[e];
        auto y = data_[e + dims_];
        if (dynamic_color_) {
          color = color2scalar(Color::cos(data_[e + dims_ + 1]));
        }
        cv::Point point(static_cast<int>(x * xs + xd),
                        static_cast<int>(y * ys + yd));
        if (has_last) {
          if (type_ == DotLine || type_ == Line || type_ == FillLine ||
              type_ == RangeLine) {
            cv::line(trans.with(color_),
                     {static_cast<int>(last_x * xs + xd),
                      static_cast<int>(last_y * ys + yd)},
                     point, color, 1, cv::LINE_AA);
          }
        } else {
          has_last = true;
        }
        if (type_ == DotLine || type_ == Dots) {
          cv::circle(trans.with(color_), point, 2, color, 1, cv::LINE_AA);
        }
        last_x = x, last_y = y;
      }
    } break;
    case Vistogram:
    case Histogram: {
      auto u = 2 * unit;
      auto o = static_cast<int>(2 * u * offset);
      for (const auto &e : entries_) {
        auto x = data_[e];
        auto y = data_[e + dims_];
        if (dynamic_color_) {
          color = color2scalar(Color::cos(data_[e + dims_ + 1]));
        }
        if (type_ == Histogram) {
          cv::rectangle(trans.with(color_),
                        {static_cast<int>(x * xs + xd) - u + o,
                         static_cast<int>(y_axis * ys + yd)},
                        {static_cast<int>(x * xs + xd) + u + o,
                         static_cast<int>(y * ys + yd)},
                        color, -1, cv::LINE_AA);
        } else if (type_ == Vistogram) {
          cv::rectangle(trans.with(color_),
                        {static_cast<int>(x_axis * xs + xd),
                         static_cast<int>(x * ys + yd) - u + o},
                        {static_cast<int>(y * xs + xd),
                         static_cast<int>(x * ys + yd) + u + o},
                        color, -1, cv::LINE_AA);
        }
      }

    } break;
    case Horizontal:
    case Vertical: {
      for (const auto &e : entries_) {
        auto y = data_[e + dims_];
        if (dynamic_color_) {
          color = color2scalar(Color::cos(data_[e + dims_ + 1]));
        }
        if (type_ == Horizontal) {
          cv::line(trans.with(color_),
                   {static_cast<int>(x_min * xs + xd),
                    static_cast<int>(y * ys + yd)},
                   {static_cast<int>(x_max * xs + xd),
                    static_cast<int>(y * ys + yd)},
                   color, 1, cv::LINE_AA);
        } else if (type_ == Vertical) {
          cv::line(trans.with(color_),
                   {static_cast<int>(y * xs + xd),
                    static_cast<int>(y_min * ys + yd)},
                   {static_cast<int>(y * xs + xd),
                    static_cast<int>(y_max * ys + yd)},
                   color, 1, cv::LINE_AA);
        }
      }
    } break;
    case Range: {
      bool has_last = false;
      cv::Point last_a;
      cv::Point last_b;
      for (const auto &e : entries_) {
        auto x = data_[e];
        auto y_a = data_[e + dims_];
        auto y_b = data_[e + dims_ + 1];
        if (dynamic_color_) {
          color = color2scalar(Color::cos(data_[e + dims_ + 2]));
        }
        cv::Point point_a(static_cast<int>(x * xs + xd),
                          static_cast<int>(y_a * ys + yd));
        cv::Point point_b(static_cast<int>(x * xs + xd),
                          static_cast<int>(y_b * ys + yd));
        if (has_last) {
          // NOLINTNEXTLINE(modernize-avoid-c-arrays)
          cv::Point points[4] = {point_a, point_b, last_b, last_a};
          cv::fillConvexPoly(trans.with(color_), points, 4, color, cv::LINE_AA);
        } else {
          has_last = true;
        }
        last_a = point_a, last_b = point_b;
      }
    } break;
    case Circle: {
      for (const auto &e : entries_) {
        auto x = data_[e];
        auto y = data_[e + dims_];
        auto r = data_[e + dims_ + 1];
        if (dynamic_color_) {
          color = color2scalar(Color::cos(data_[e + dims_ + 2]));
        }
        cv::Point point(static_cast<int>(x * xs + xd),
                        static_cast<int>(y * ys + yd));
        cv::circle(trans.with(color_), point, r, color, -1, cv::LINE_AA);
      }
    } break;
  }
}

auto Figure::clear() -> Figure & {
  series_.clear();
  return *this;
}

auto Figure::origin(bool x, bool y) -> Figure & {
  include_zero_x_ = x, include_zero_y_ = y;
  return *this;
}

auto Figure::square(bool square) -> Figure & {
  aspect_square_ = square;
  return *this;
}

auto Figure::border(int size) -> Figure & {
  border_size_ = size;
  return *this;
}

auto Figure::alpha(int alpha) -> Figure & {
  background_color_ = background_color_.alpha(alpha);
  axis_color_ = axis_color_.alpha(alpha);
  sub_axis_color_ = sub_axis_color_.alpha(alpha);
  text_color_ = text_color_.alpha(alpha);
  return *this;
}

auto Figure::gridSize(int size) -> Figure & {
  grid_size_ = size;
  return *this;
}

auto Figure::backgroundColor(Color color) -> Figure & {
  background_color_ = color;
  return *this;
}

auto Figure::axisColor(Color color) -> Figure & {
  axis_color_ = color;
  return *this;
}

auto Figure::subaxisColor(Color color) -> Figure & {
  sub_axis_color_ = color;
  return *this;
}

auto Figure::textColor(Color color) -> Figure & {
  text_color_ = color;
  return *this;
}

auto Figure::backgroundColor() -> Color { return background_color_; }

auto Figure::axisColor() -> Color { return axis_color_; }

auto Figure::subaxisColor() -> Color { return sub_axis_color_; }

auto Figure::textColor() -> Color { return text_color_; }

auto Figure::series(const std::string &label) -> Series & {
  for (auto &s : series_) {
    if (s.label() == label) {
      return s;
    }
  }
  Series s(label, Line, Color::hash(label));
  series_.push_back(s);
  return series_.back();
}

void Figure::draw(void *b, float x_min, float x_max, float y_min, float y_max,
                  int n_max, int p_max) const {
  auto &buffer = *static_cast<cv::Mat *>(b);
  Trans trans(b);

  // draw background and sub axis square
  cv::rectangle(trans.with(background_color_), {0, 0},
                {buffer.cols, buffer.rows}, color2scalar(background_color_), -1,
                cv::LINE_AA);
  cv::rectangle(trans.with(sub_axis_color_), {border_size_, border_size_},
                {buffer.cols - border_size_, buffer.rows - border_size_},
                color2scalar(sub_axis_color_), 1, cv::LINE_AA);

  // size of the plotting area
  auto w_plot = buffer.cols - 2 * border_size_;
  auto h_plot = buffer.rows - 2 * border_size_;

  // add padding inside graph (histograms get extra)
  if (p_max != 0) {
    auto dx = p_max * (x_max - x_min) / w_plot;
    auto dy = p_max * (y_max - y_min) / h_plot;
    x_min -= dx;
    x_max += dx;
    y_min -= dy;
    y_max += dy;
  }

  // adjust value range if aspect ratio square
  if (aspect_square_) {
    if (h_plot * (x_max - x_min) < w_plot * (y_max - y_min)) {
      auto dx = w_plot * (y_max - y_min) / h_plot - (x_max - x_min);
      x_min -= dx / 2;
      x_max += dx / 2;
    } else if (w_plot * (y_max - y_min) < h_plot * (x_max - x_min)) {
      auto dy = h_plot * (x_max - x_min) / w_plot - (y_max - y_min);
      y_min -= dy / 2;
      y_max += dy / 2;
    }
  }

  // calc where to draw axis
  auto x_axis = std::max(x_min, std::min(x_max, 0.F));
  auto y_axis = std::max(y_min, std::min(y_max, 0.F));

  // calc sub axis grid size
  auto x_grid =
      (x_max != x_min ? value2snap((x_max - x_min) / floor(w_plot / grid_size_))
                      : 1);
  auto y_grid =
      (y_max != x_min ? value2snap((y_max - y_min) / floor(h_plot / grid_size_))
                      : 1);

  // calc affine transform value space to plot space
  auto xs = (x_max != x_min ? (buffer.cols - 2 * border_size_) / (x_max - x_min)
                            : 1.F);
  auto xd = border_size_ - x_min * xs;
  auto ys = (y_max != y_min ? (buffer.rows - 2 * border_size_) / (y_min - y_max)
                            : 1.F);
  auto yd = buffer.rows - y_min * ys - border_size_;

  // safe unit for showing points
  auto unit =
      std::max(1, (static_cast<int>(std::min(buffer.cols, buffer.rows)) -
                   2 * border_size_) /
                      n_max / 10);

  // draw sub axis
  for (int i = ceil(x_min / x_grid), e = floor(x_max / x_grid); i <= e; i++) {
    auto x = i * x_grid;
    cv::line(trans.with(sub_axis_color_),
             {static_cast<int>(x * xs + xd), border_size_},
             {static_cast<int>(x * xs + xd), buffer.rows - border_size_},
             color2scalar(sub_axis_color_), 1, cv::LINE_AA);
  }
  for (int i = ceil(y_min / y_grid), e = floor(y_max / y_grid); i <= e; i++) {
    auto y = i * y_grid;
    cv::line(trans.with(sub_axis_color_),
             {border_size_, static_cast<int>(y * ys + yd)},
             {buffer.cols - border_size_, static_cast<int>(y * ys + yd)},
             color2scalar(sub_axis_color_), 1, cv::LINE_AA);
  }
  if (std::abs(x_grid * xs) < 30) {
    x_grid *= std::ceil(30.F / std::abs(x_grid * xs));
  }
  for (int i = std::ceil(x_min / x_grid), e = floor(x_max / x_grid); i <= e;
       i++) {
    auto x = i * x_grid;
    std::ostringstream out;
    out << std::setprecision(4) << (x == 0 ? 0 : x);
    int baseline = 0;
    cv::Size size =
        getTextSize(out.str(), cv::FONT_HERSHEY_SIMPLEX, 0.3F, 1.F, &baseline);
    cv::Point org(x * xs + xd - size.width / 2,
                  buffer.rows - border_size_ + 5 + size.height);
    cv::putText(trans.with(text_color_), out.str(), org,
                cv::FONT_HERSHEY_SIMPLEX, 0.3F, color2scalar(text_color_), 1.F);
  }
  if (std::abs(y_grid * ys) < 20) {
    y_grid *= std::ceil(20.F / std::abs(y_grid * ys));
  }
  for (int i = std::ceil(y_min / y_grid), e = floor(y_max / y_grid); i <= e;
       i++) {
    auto y = i * y_grid;
    std::ostringstream out;
    out << std::setprecision(4) << (y == 0 ? 0 : y);
    int baseline = 0;
    cv::Size size =
        getTextSize(out.str(), cv::FONT_HERSHEY_SIMPLEX, 0.3F, 1.F, &baseline);
    cv::Point org(border_size_ - 5 - size.width, y * ys + yd + size.height / 2);
    cv::putText(trans.with(text_color_), out.str(), org,
                cv::FONT_HERSHEY_SIMPLEX, 0.3F, color2scalar(text_color_), 1.F);
  }

  // draw axis
  cv::line(trans.with(axis_color_),
           {border_size_, static_cast<int>(y_axis * ys + yd)},
           {buffer.cols - border_size_, static_cast<int>(y_axis * ys + yd)},
           color2scalar(axis_color_), 1, cv::LINE_AA);
  cv::line(trans.with(axis_color_),
           {static_cast<int>(x_axis * xs + xd), border_size_},
           {static_cast<int>(x_axis * xs + xd), buffer.rows - border_size_},
           color2scalar(axis_color_), 1, cv::LINE_AA);

  // draw plot
  auto index = 0;
  for (const auto &s : series_) {
    if (s.collides()) {
      index++;
    }
  }
  std::max(static_cast<int>(series_.size()) - 1, 1);
  for (auto s = series_.rbegin(); s != series_.rend(); ++s) {
    if (s->collides()) {
      index--;
    }
    s->draw(&trans.with(s->color()), x_min, x_max, y_min, y_max, xs, xd, ys, yd,
            x_axis, y_axis, unit, static_cast<float>(index) / series_.size());
  }

  // draw label names
  index = 0;
  for (const auto &s : series_) {
    if (!s.legend()) {
      continue;
    }
    auto name = s.label();
    int baseline = 0;
    cv::Size size =
        getTextSize(name, cv::FONT_HERSHEY_SIMPLEX, 0.4F, 1.F, &baseline);
    cv::Point org(buffer.cols - border_size_ - size.width - 17,
                  border_size_ + 15 * index + 15);
    auto shadow = true;
    cv::putText(trans.with(background_color_), name,
                {org.x + (shadow ? 1 : 0), org.y + (shadow ? 1 : 0)},
                cv::FONT_HERSHEY_SIMPLEX, 0.4F, color2scalar(background_color_),
                (shadow ? 1.F : 2.F));
    cv::circle(trans.with(background_color_),
               {buffer.cols - border_size_ - 10 + 1, org.y - 3 + 1}, 3,
               color2scalar(background_color_), -1, cv::LINE_AA);
    cv::putText(trans.with(text_color_), name, org, cv::FONT_HERSHEY_SIMPLEX,
                0.4F, color2scalar(text_color_), 1.F);
    s.dot(&trans.with(s.color()), buffer.cols - border_size_ - 10, org.y - 3,
          3);
    index++;
  }
}

auto Figure::drawFit(void *buffer) const -> int {
  auto x_min = (include_zero_x_ ? 0.F : FLT_MAX);
  auto x_max = (include_zero_x_ ? 0.F : FLT_MIN);
  auto y_min = (include_zero_y_ ? 0.F : FLT_MAX);
  auto y_max = (include_zero_y_ ? 0.F : FLT_MIN);
  auto n_max = 0;
  auto p_max = grid_padding_;

  // find value bounds
  for (const auto &s : series_) {
    s.verifyParams();
    s.bounds(x_min, x_max, y_min, y_max, n_max, p_max);
  }

  if (n_max != 0) {
    draw(buffer, x_min, x_max, y_min, y_max, n_max, p_max);
  }

  return n_max;
}

auto Figure::drawFile(const std::string &filename, Size size) const -> bool {
  cv::Mat mat(cv::Size(size.width, size.height), CV_8UC3);
  int n_max = drawFit(&mat);
  if (n_max != 0) {
    std::vector<int> compression_params;
    compression_params.push_back(cv::IMWRITE_PNG_COMPRESSION);
    compression_params.push_back(9);
    return imwrite(filename, mat, compression_params);
  }
  return false;
}

void Figure::show(bool flush) const {
  Rect rect(0, 0, 0, 0);
  auto &buffer = *static_cast<cv::Mat *>(view_.buffer(rect));
  auto sub = buffer({rect.x, rect.y, rect.width, rect.height});
  int n_max = drawFit(&sub);
  if (n_max != 0) {
    view_.finish();
    if (flush) {
      view_.flush();
    }
  }
}

auto figure(const std::string &name) -> Figure & {
  if (shared_figures_.count(name) == 0) {
    auto &view = Window::current().view(name);
    shared_figures_.insert(
        std::map<std::string, Figure>::value_type(name, Figure(view)));
  }
  return shared_figures_.at(name);
}

}  // namespace cvplot

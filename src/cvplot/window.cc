#include "cvplot/window.h"

#include <ctime>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <utility>

#include "internal.h"

namespace cvplot {

namespace {
// NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
Window *shared_window = nullptr;
}  // namespace

void mouse_callback(int event, int x, int y, int flags, void *window) {
  (static_cast<Window *>(window))->onmouse(event, x, y, flags);
}

// View

auto View::resize(Rect rect) -> View & {
  rect_ = rect;
  window_.dirty();
  return *this;
}

auto View::size(Size size) -> View & {
  rect_.width = size.width;
  rect_.height = size.height;
  window_.dirty();
  return *this;
}

auto View::offset(Offset offset) -> View & {
  rect_.x = offset.x;
  rect_.y = offset.y;
  window_.dirty();
  return *this;
}

auto View::autosize() -> View & {
  size({0, 0});
  return *this;
}

auto View::title(const std::string &title) -> View & {
  title_ = title;
  window_.dirty();
  return *this;
}

auto View::alpha(int alpha) -> View & {
  background_color_ = background_color_.alpha(alpha);
  frame_color_ = frame_color_.alpha(alpha);
  text_color_ = text_color_.alpha(alpha);
  window_.dirty();
  return *this;
}

auto View::backgroundColor(Color color) -> View & {
  background_color_ = color;
  window_.dirty();
  return *this;
}

auto View::frameColor(Color color) -> View & {
  frame_color_ = color;
  window_.dirty();
  return *this;
}

auto View::textColor(Color color) -> View & {
  text_color_ = color;
  window_.dirty();
  return *this;
}

auto View::mouse(MouseCallback callback, void *param) -> View & {
  mouse_callback_ = callback;
  mouse_param_ = (param == nullptr ? this : param);
  return *this;
}

auto View::backgroundColor() -> Color { return background_color_; }

auto View::frameColor() -> Color { return frame_color_; }

auto View::textColor() -> Color { return text_color_; }
auto View::title() -> std::string & { return title_; }

void View::drawRect(Rect rect, Color color) {
  Trans trans(window_.buffer());
  cv::rectangle(trans.with(color), {rect_.x + rect.x, rect_.y + rect.y},
                {rect_.x + rect.x + rect.width, rect_.y + rect.y + rect.height},
                color2scalar(color), -1);
  window_.dirty();
}

void View::drawText(const std::string &text, Offset offset, Color color,
                    double height) const {
  auto face = cv::FONT_HERSHEY_SIMPLEX;
  auto scale = height / 30.;
  auto thickness = height / 12.;
  int baseline = 0;
  cv::Size size =
      getTextSize(text, face, scale, static_cast<int>(thickness), &baseline);
  cv::Point org(rect_.x + offset.x, rect_.y + size.height + offset.y);
  Trans trans(window_.buffer());
  cv::putText(trans.with(color), text, org, face, scale, color2scalar(color),
              static_cast<int>(thickness));
  window_.dirty();
}

void View::drawTextShadow(const std::string &text, Offset offset, Color color,
                          double height) const {
  int off = static_cast<int>(height / 20);
  drawText(text, {offset.x + off, offset.y + off}, cvplot::Black.alpha(100),
           height);
  drawText(text, offset, color, height);
}

void View::drawFrame(const std::string &title) const {
  Trans trans(window_.buffer());
  cv::rectangle(trans.with(background_color_), {rect_.x, rect_.y},
                {rect_.x + rect_.width - 1, rect_.y + rect_.height - 1},
                color2scalar(background_color_), 1);
  cv::rectangle(trans.with(frame_color_), {rect_.x + 1, rect_.y + 1},
                {rect_.x + rect_.width - 2, rect_.y + rect_.height - 2},
                color2scalar(frame_color_), 1);
  cv::rectangle(trans.with(frame_color_), {rect_.x + 2, rect_.y + 2},
                {rect_.x + rect_.width - 3, rect_.y + 16},
                color2scalar(frame_color_), -1);
  int baseline = 0;
  cv::Size size = getTextSize(title, cv::FONT_HERSHEY_PLAIN, 1., 1., &baseline);
  cv::putText(trans.with(text_color_), title,
              {rect_.x + 2 + (rect_.width - size.width) / 2, rect_.y + 14},
              cv::FONT_HERSHEY_PLAIN, 1., color2scalar(text_color_), 1.);
  window_.dirty();
}

void View::drawImage(const void *image, int alpha) {
  const auto &img = *static_cast<const cv::Mat *>(image);
  if (rect_.width == 0 && rect_.height == 0) {
    rect_.width = img.cols;
    rect_.height = img.rows;
  }
  window_.ensure(rect_);
  Trans trans(window_.buffer());
  if (img.cols != rect_.width || img.rows != rect_.height) {
    cv::Mat resized;
    cv::resize(img, resized, {rect_.width, rect_.height});
    resized.copyTo(
        trans.with(alpha)({rect_.x, rect_.y, rect_.width, rect_.height}));
  } else {
    img.copyTo(
        trans.with(alpha)({rect_.x, rect_.y, rect_.width, rect_.height}));
  }
  window_.dirty();
}

void View::drawFill(Color background) {
  Trans trans(window_.buffer());
  cv::rectangle(trans.with(background), {rect_.x, rect_.y},
                {rect_.x + rect_.width - 1, rect_.y + rect_.height - 1},
                color2scalar(background), -1);
  window_.dirty();
}

auto View::buffer(Rect &rect) -> void * {
  window_.ensure(rect_);
  rect = rect_;
  return window_.buffer();
}

void View::finish() {
  if (!frameless_) {
    drawFrame(title_);
  }
  window_.dirty();
}

void View::flush() { window_.flush(); }

auto View::has(Offset offset) const -> bool {
  return offset.x >= rect_.x && offset.y >= rect_.y &&
         offset.x < rect_.x + rect_.width && offset.y < rect_.y + rect_.height;
}

void View::onmouse(int event, int x, int y, int flags) {
  if (mouse_callback_ != nullptr) {
    mouse_callback_(event, x, y, flags, mouse_param_);
  }
}

void View::hide(bool hidden) {
  if (hidden_ != hidden) {
    hidden_ = hidden;
    drawFill();
  }
}

// Window

Window::Window(std::string title)
    : offset_(0, 0),
      title_(std::move(title)),
      cursor_(-10, -10),
      name_("cvplot_" + std::to_string(clock())) {}

auto Window::buffer() -> void * { return buffer_; }

auto Window::resize(Rect rect) -> Window & {
  offset({rect.x, rect.y});
  size({rect.width, rect.height});
  return *this;
}

auto Window::size(Size size) -> Window & {
  auto &buffer = *(new cv::Mat(cv::Size(size.width, size.height), CV_8UC3,
                               color2scalar(Gray)));
  if (buffer_ != nullptr) {
    auto &current = *static_cast<cv::Mat *>(buffer_);
    if (current.cols > 0 && current.rows > 0 && size.width > 0 &&
        size.height > 0) {
      cv::Rect inter(0, 0, std::min(current.cols, size.width),
                     std::min(current.rows, size.height));
      current(inter).copyTo(buffer(inter));
    }
    // NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
    delete &current;
  }
  buffer_ = &buffer;
  dirty();
  return *this;
}

auto Window::offset(Offset offset) -> Window & {
  offset_ = offset;
  cv::namedWindow(name_, cv::WINDOW_AUTOSIZE);
  cv::moveWindow(name_, offset.x, offset.y);
  return *this;
}

auto Window::title(const std::string &title) -> Window & {
  title_ = title;
  return *this;
}

auto Window::fps(double fps) -> Window & {
  fps_ = fps;
  return *this;
}

auto Window::cursor(bool cursor) -> Window & {
  show_cursor_ = cursor;
  return *this;
}

auto Window::ensure(Rect rect) -> Window & {
  if (buffer_ == nullptr) {
    size({rect.x + rect.width, rect.y + rect.height});
  } else {
    auto &b = *static_cast<cv::Mat *>(buffer_);
    if (rect.x + rect.width > b.cols || rect.y + rect.height > b.rows) {
      size({std::max(b.cols, rect.x + rect.width),
            std::max(b.rows, rect.y + rect.height)});
    }
  }
  return *this;
}

void Window::onmouse(int event, int x, int y, int flags) {
  for (auto iter = views_.rbegin(); iter != views_.rend(); ++iter) {
    auto &view = iter->second;
    if (view.has({x, y})) {
      view.onmouse(event, x, y, flags);
      break;
    }
  }
  cursor_ = {x, y};
  if (show_cursor_) {
    dirty();
    flush();
  }
}

void Window::flush() {
  if (dirty_ && buffer_ != nullptr) {
    auto *b = static_cast<cv::Mat *>(buffer_);
    if (b->cols > 0 && b->rows > 0) {
      cv::Mat mat;
      if (show_cursor_) {
        b->copyTo(mat);
        cv::line(mat, {cursor_.x - 4, cursor_.y + 1},
                 {cursor_.x + 6, cursor_.y + 1}, color2scalar(White), 1);
        cv::line(mat, {cursor_.x + 1, cursor_.y - 4},
                 {cursor_.x + 1, cursor_.y + 6}, color2scalar(White), 1);
        cv::line(mat, {cursor_.x - 5, cursor_.y}, {cursor_.x + 5, cursor_.y},
                 color2scalar(Black), 1);
        cv::line(mat, {cursor_.x, cursor_.y - 5}, {cursor_.x, cursor_.y + 5},
                 color2scalar(Black), 1);
        b = &mat;
      }
      cv::namedWindow(name_, cv::WINDOW_AUTOSIZE);
#if CV_MAJOR_VERSION > 2
      cv::setWindowTitle(name_, title_);
#endif
      cv::imshow(name_, *b);
      cv::setMouseCallback(name_, mouse_callback, this);
      Util::sleep();
    }
  }
  dirty_ = false;
}

auto Window::view(const std::string &name, Size size) -> View & {
  if (views_.count(name) == 0) {
    views_.insert(
        std::map<std::string, View>::value_type(name, View(*this, name, size)));
  }
  return views_.at(name);
}

void Window::dirty() { dirty_ = true; }

void Window::hide(bool hidden) {
  if (hidden_ != hidden) {
    hidden_ = hidden;
    if (hidden) {
      cv::destroyWindow(name_);
    } else {
      dirty();
      flush();
    }
  }
}

auto Window::current() -> Window & {
  if (shared_window == nullptr) {
    // NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
    shared_window = new Window("");
  }
  return *shared_window;
}

auto Window::current(const std::string &title) -> Window & {
  // NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
  shared_window = new Window(title);
  return *shared_window;
}

void Window::current(Window &window) { shared_window = &window; }

// Util

void Util::sleep(double seconds) {
  cv::waitKey(std::max(1, static_cast<int>(seconds * 1000)));
}

auto Util::key(double timeout) -> int {
  return cv::waitKey(std::max(0, static_cast<int>(timeout * 1000)));
}

auto Util::line(double timeout) -> std::string {
  std::stringstream stream;
  auto ms = (timeout > 0 ? std::max(1, static_cast<int>(timeout * 1000)) : -1);
  while (ms != 0) {
    auto key = cv::waitKey(1);
    if (key == -1) {
      ms--;
      continue;
    }
    if (key == '\r' || key <= '\n') {
      break;
    }
    if (key == '\b' || key == 127) {
      auto s = stream.str();
      stream = std::stringstream();
      if (s.length() > 0) {
        stream << s.substr(0, s.length() - 1);
      }
    } else {
      stream << static_cast<char>(key);
    }
  }
  return stream.str();
}

}  // namespace cvplot

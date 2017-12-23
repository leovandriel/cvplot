#include "cvplot/window.h"
#include "internal.h"

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

namespace cvplot {

namespace {
Window shared_window;
}

Window::View &Window::View::resize(Rect rect) {
  rect_ = rect;
  return *this;
}

Window::View &Window::View::size(Size size) {
  rect_.width = size.width;
  rect_.height = size.height;
  return *this;
}

Window::View &Window::View::offset(Offset offset) {
  rect_.x = offset.x;
  rect_.y = offset.y;
  return *this;
}

Window::View &Window::View::autosize() {
  size({0, 0});
  return *this;
}

Window::View &Window::View::title(const std::string &title) {
  title_ = title;
  return *this;
}

Window::View &Window::View::alpha(int alpha) {
  background_color_ = background_color_.alpha(alpha);
  frame_color_ = frame_color_.alpha(alpha);
  text_color_ = text_color_.alpha(alpha);
  return *this;
}

Window::View &Window::View::backgroundColor(Color color) {
  background_color_ = color;
  return *this;
}

Window::View &Window::View::frameColor(Color color) {
  frame_color_ = color;
  return *this;
}

Window::View &Window::View::textColor(Color color) {
  text_color_ = color;
  return *this;
}

Color Window::View::backgroundColor() { return background_color_; }

Color Window::View::frameColor() { return frame_color_; }

Color Window::View::textColor() { return text_color_; }

void Window::View::drawText(const std::string &text, Offset offset,
                            Color color) const {
  auto face = cv::FONT_HERSHEY_SIMPLEX;
  auto scale = 0.4f;
  auto thickness = 1.f;
  int baseline;
  cv::Size size = getTextSize(text, face, scale, thickness, &baseline);
  cv::Point org(rect_.x + offset.x, rect_.y + size.height + offset.y);
  Trans trans(window_.buffer_);
  cv::putText(trans.with(color), text.c_str(), org, face, scale,
              color2scalar(color), thickness);
}

void Window::View::drawFrame(const std::string &title) const {
  Trans trans(window_.buffer_);
  cv::rectangle(trans.with(background_color_), {rect_.x, rect_.y},
                {rect_.x + rect_.width - 1, rect_.y + rect_.height - 1},
                color2scalar(background_color_), 1);
  cv::rectangle(trans.with(frame_color_), {rect_.x + 1, rect_.y + 1},
                {rect_.x + rect_.width - 2, rect_.y + rect_.height - 2},
                color2scalar(frame_color_), 1);
  cv::rectangle(trans.with(frame_color_), {rect_.x + 2, rect_.y + 2},
                {rect_.x + rect_.width - 3, rect_.y + 16},
                color2scalar(frame_color_), -1);
  int baseline;
  cv::Size size =
      getTextSize(title.c_str(), cv::FONT_HERSHEY_PLAIN, 1.f, 1.f, &baseline);
  cv::putText(trans.with(text_color_), title.c_str(),
              {rect_.x + 2 + (rect_.width - size.width) / 2, rect_.y + 14},
              cv::FONT_HERSHEY_PLAIN, 1.f, color2scalar(text_color_), 1.f);
}

void Window::View::drawImage(const void *image, int alpha) {
  auto &img = *(cv::Mat *)image;
  if (rect_.width == 0 && rect_.height == 0) {
    rect_.width = img.cols;
    rect_.height = img.rows;
  }
  window_.ensure(rect_, false);
  Trans trans(window_.buffer_);
  if (img.cols != rect_.width || img.rows != rect_.height) {
    cv::Mat resized;
    cv::resize(img, resized, {rect_.width, rect_.height});
    resized.copyTo(
        trans.with(alpha)({rect_.x, rect_.y, rect_.width, rect_.height}));
  } else {
    img.copyTo(
        trans.with(alpha)({rect_.x, rect_.y, rect_.width, rect_.height}));
  }
}

void Window::View::drawFill(Color color) {
  Trans trans(window_.buffer_);
  cv::rectangle(trans.with(color), {rect_.x, rect_.y},
                {rect_.x + rect_.width - 1, rect_.y + rect_.height - 1},
                color2scalar(color), -1);
}

void *Window::View::buffer(Rect &rect) {
  window_.ensure(rect_, false);
  rect = rect_;
  return window_.buffer_;
}

void Window::View::show(bool flush) const {
  if (!frameless_) {
    drawFrame(title_);
  }
  window_.show(flush);
}

Window &Window::resize(Rect rect, bool flush) {
  offset({rect.x, rect.y});
  size({rect.width, rect.height}, flush);
  return *this;
}

Window &Window::size(Size size, bool flush) {
  auto &buffer = *(new cv::Mat(cv::Size(size.width, size.height), CV_8UC3,
                               cv::Scalar::all(128.f)));
  if (buffer_ != NULL) {
    auto &current = *(cv::Mat *)buffer_;
    if (current.cols > 0 && current.rows > 0 && size.width > 0 &&
        size.height > 0) {
      cv::Rect inter(0, 0, std::min(current.cols, size.width),
                     std::min(current.rows, size.height));
      current(inter).copyTo(buffer(inter));
    }
    delete &current;
  }
  buffer_ = &buffer;
  show(flush);
  return *this;
}

Window &Window::offset(Offset offset) {
  offset_ = offset;
  cv::namedWindow(title_, cv::WINDOW_AUTOSIZE);
  cv::moveWindow(title_, offset.x, offset.y);
  return *this;
}

Window &Window::title(const std::string &title) {
  title_ = title;
  return *this;
}

Window &Window::ensure(Rect rect, bool flush) {
  if (buffer_ == NULL) {
    size({rect.x + rect.width, rect.y + rect.height}, flush);
  } else {
    auto &b = *(cv::Mat *)buffer_;
    if (rect.x + rect.width > b.cols || rect.y + rect.height > b.rows) {
      size({std::max(b.cols, rect.x + rect.width),
            std::max(b.rows, rect.y + rect.height)},
           flush);
    }
  }
  return *this;
}

void Window::show(bool flush) const {
  if (buffer_ != NULL) {
    auto &b = *(cv::Mat *)buffer_;
    if (flush && b.cols > 0 && b.rows > 0) {
      cv::namedWindow(title_, cv::WINDOW_AUTOSIZE);
      cv::imshow(title_.c_str(), b);
      cvWaitKey(1);
    }
  }
}

Window::View &Window::view(const std::string &name, Size size) {
  if (views_.count(name) == 0) {
    views_.insert(
        std::map<std::string, View>::value_type(name, View(*this, name, size)));
  }
  return views_.at(name);
}

void window(const char *title, int width, int height, bool flush) {
  shared_window.title(title);
  shared_window.size({width, height}, flush);
}

void move(int x, int y) { shared_window.offset({x, y}); }

void move(const char *name, int x, int y) {
  shared_window.view(name).offset({x, y});
}

void resize(const char *name, int width, int height) {
  shared_window.view(name).size({width, height});
}

void autosize(const char *name) { shared_window.view(name).autosize(); }

void title(const char *name, const char *title) {
  shared_window.view(name).title(title);
}

void imshow(const char *name, const void *image, bool flush) {
  shared_window.view(name).drawImage(image);
  shared_window.view(name).show(flush);
}

void *buffer(const char *name, int &x, int &y, int &width, int &height) {
  Rect rect(0, 0, 0, 0);
  auto buffer = shared_window.view(name).buffer(rect);
  x = rect.x;
  y = rect.y;
  width = rect.width;
  height = rect.height;
  return buffer;
}

void show(const char *name, bool flush) {
  shared_window.view(name).show(flush);
}

void clear(const char *name, bool flush) {
  shared_window.view(name).drawFill();
  shared_window.view(name).show(flush);
}

void show(bool flush) { shared_window.show(true); }

Window &window() { return shared_window; }

}  // namespace cvplot

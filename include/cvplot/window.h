#ifndef CVPLOT_WINDOW_H
#define CVPLOT_WINDOW_H

#include <map>
#include <string>
#include <utility>

#include "color.h"

namespace cvplot {

struct Rect {
  int x, y, width, height;
  Rect(int x, int y, int width, int height)
      : x(x), y(y), width(width), height(height) {}
};

struct Size {
  int width, height;
  Size(int width, int height) : width(width), height(height) {}
};

struct Offset {
  int x, y;
  Offset(int x, int y) : x(x), y(y) {}
};

using MouseCallback = void (*)(int, int, int, int, void *);
using TrackbarCallback = void (*)(int, void *);

class Window;

class View {
 public:
  View(Window &window, std::string title = "", Size size = {300, 300})
      : window_(window),
        title_(std::move(title)),
        rect_(0, 0, size.width, size.height),
        frameless_(false),
        background_color_(Black),
        frame_color_(Green),
        text_color_(Black),
        mouse_callback_(nullptr),
        mouse_param_(nullptr) {}
  auto resize(Rect rect) -> View &;
  auto size(Size size) -> View &;
  auto offset(Offset offset) -> View &;
  auto autosize() -> View &;
  auto title(const std::string &title) -> View &;
  auto alpha(int alpha) -> View &;
  auto backgroundColor(Color color) -> View &;
  auto frameColor(Color color) -> View &;
  auto textColor(Color color) -> View &;
  auto mouse(MouseCallback callback, void *param = nullptr) -> View &;
  void onmouse(int event, int x, int y, int flags);

  auto backgroundColor() -> Color;
  auto frameColor() -> Color;
  auto textColor() -> Color;
  auto title() -> std::string &;
  auto has(Offset offset) const -> bool;

  void drawRect(Rect rect, Color color);
  void drawFill(Color background = White);
  void drawImage(const void *image, int alpha = 255);
  void drawText(const std::string &text, Offset offset, Color color,
                double height = 12.) const;
  void drawTextShadow(const std::string &text, Offset offset, Color color,
                      double height) const;
  void drawFrame(const std::string &title) const;
  auto buffer(Rect &rect) -> void *;
  void finish();
  void flush();
  void hide(bool hidden = true);

  auto operator=(const View &) -> View & = delete;

 protected:
  Rect rect_;
  std::string title_;
  bool frameless_;
  Window &window_;
  Color background_color_;
  Color frame_color_;
  Color text_color_;
  MouseCallback mouse_callback_;
  void *mouse_param_;
  bool hidden_;
};

class Window {
 public:
  Window(std::string title = "");
  ~Window();
  auto resize(Rect rect) -> Window &;
  auto size(Size size) -> Window &;
  auto offset(Offset offset) -> Window &;
  auto title(const std::string &title) -> Window &;
  auto fps(double fps) -> Window &;
  auto ensure(Rect rect) -> Window &;
  auto cursor(bool cursor) -> Window &;
  auto buffer() -> void *;
  void flush();
  auto view(const std::string &name, Size size = {300, 300}) -> View &;
  void dirty();
  void hide(bool hidden = true);
  void onmouse(int event, int x, int y, int flags);
  auto name() const -> const std::string & { return name_; }

  auto operator=(const Window &) -> Window & = delete;

  static auto current() -> Window &;
  static void current(Window &window);
  static auto current(const std::string &title) -> Window &;

 protected:
  Offset offset_;
  void *buffer_{nullptr};
  std::string title_;
  std::string name_;
  std::map<std::string, View> views_;
  bool dirty_{false};
  double fps_{1};
  bool hidden_{false};
  bool show_cursor_{false};
  Offset cursor_;
};

class Util {
 public:
  static void sleep(double seconds = 0);
  static auto key(double timeout = 0) -> int;
  static auto line(double timeout = 0) -> std::string;
};

auto window(const std::string &name) -> Window &;

}  // namespace cvplot

#endif  // CVPLOT_WINDOW_H

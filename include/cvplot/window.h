#ifndef CVPLOT_WINDOW_H
#define CVPLOT_WINDOW_H

#include "color.h"

#include <map>
#include <string>

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

typedef void (*MouseCallback)(int event, int x, int y, int flags, void *param);

class Window;

class View {
 public:
  View(Window &window, const std::string &title = "", Size size = {300, 300})
      : window_(window),
        title_(title),
        rect_(0, 0, size.width, size.height),
        frameless_(false),
        background_color_(Black),
        frame_color_(Green),
        text_color_(Black),
        mouse_callback_(NULL),
        mouse_param_(NULL) {}
  View &resize(Rect rect);
  View &size(Size size);
  View &offset(Offset offset);
  View &autosize();
  View &title(const std::string &title);
  View &alpha(int alpha);
  View &backgroundColor(Color color);
  View &frameColor(Color color);
  View &textColor(Color color);
  View &mouse(MouseCallback callback, void *param = NULL);
  void onmouse(int event, int x, int y, int flags);

  Color backgroundColor();
  Color frameColor();
  Color textColor();
  std::string &title();
  bool has(Offset offset);

  void drawRect(Rect rect, Color color);
  void drawFill(Color background = White);
  void drawImage(const void *image, int alpha = 255);
  void drawText(const std::string &text, Offset offset, Color color) const;
  void drawFrame(const std::string &title) const;
  void *buffer(Rect &rect);
  void finish();
  void flush();

  View &operator=(const View &) = delete;

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
};

class Window {
 public:
  Window(const std::string &title = "");
  Window &resize(Rect rect);
  Window &size(Size size);
  Window &offset(Offset offset);
  Window &title(const std::string &title);
  Window &fps(float fps);
  Window &ensure(Rect rect);
  Window &cursor(bool cursor);
  void *buffer();
  void flush();
  View &view(const std::string &name, Size size = {300, 300});
  void dirty();
  void tick();
  void sleep(float seconds);
  void hide(bool hidden = true);
  void onmouse(int event, int x, int y, int flags);

  Window &operator=(const Window &) = delete;

 protected:
  Offset offset_;
  void *buffer_;
  std::string title_;
  std::string name_;
  std::map<std::string, View> views_;
  bool dirty_;
  float flush_time_;
  float fps_;
  bool hidden_;
  bool show_cursor_;
  Offset cursor_;
};

void window(const char *title, int width = 0, int height = 0);
View &view(const char *name);
void move(int x, int y);
void move(const char *name, int x, int y);
void resize(const char *name, int width, int height);
void clear(const char *name);
void autosize(const char *name);
void title(const char *name, const char *title);
void imshow(const char *name, const void *image);
void *buffer(const char *name, int &x, int &y, int &width, int &height);
void show(const char *name);
void show();
Window &window();

}  // namespace cvplot

#endif  // CVPLOT_WINDOW_H

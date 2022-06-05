#include "cvplot/highgui.h"

#include <opencv2/highgui/highgui.hpp>

namespace cvplot {

auto createTrackbar(const std::string &trackbarname, const std::string &winname,
                    int *value, int count, TrackbarCallback onChange,
                    void *userdata) -> int {
  return cv::createTrackbar(trackbarname, winname, value, count, onChange,
                            userdata);
}

void destroyAllWindows() { cv::destroyAllWindows(); }

void destroyWindow(const std::string &view) {
  Window::current().view(view).hide();
}

auto getMouseWheelDelta(int flags) -> int {
#if CV_MAJOR_VERSION > 2
  return cv::getMouseWheelDelta(flags);
#else
  return -1;
#endif
}

auto getTrackbarPos(const std::string &trackbarname, const std::string &winname)
    -> int {
  return cv::getTrackbarPos(trackbarname, winname);
}

auto getWindowProperty(const std::string &winname, int prop_id) -> double {
  return cv::getWindowProperty(winname, prop_id);
}

void imshow(const std::string &view, void *img) {
  Window::current().view(view).drawImage(img);
  Window::current().view(view).finish();
  Window::current().view(view).flush();
}

void moveWindow(const std::string &view, int x, int y) {
  Window::current().view(view).offset({x, y});
}

void namedWindow(const std::string &view, int /*flags*/) {
  Window::current().view(view);
}

void resizeWindow(const std::string &view, int width, int height) {
  Window::current().view(view).size({width, height});
}

void resizeWindow(const std::string &view, const Size &size) {
  Window::current().view(view).size({size.width, size.height});
}

void setMouseCallback(const std::string &view, MouseCallback onMouse,
                      void *userdata) {
  Window::current().view(view).mouse(onMouse, userdata);
}

void setTrackbarMax(const std::string &trackbarname, const std::string &winname,
                    int maxval) {
#if CV_MAJOR_VERSION > 2
  cv::setTrackbarMax(trackbarname, winname, maxval);
#endif
}

void setTrackbarMin(const std::string &trackbarname, const std::string &winname,
                    int minval) {
#if CV_MAJOR_VERSION > 2
  cv::setTrackbarMin(trackbarname, winname, minval);
#endif
}

void setTrackbarPos(const std::string &trackbarname, const std::string &winname,
                    int pos) {
  cv::setTrackbarPos(trackbarname, winname, pos);
}

void setWindowProperty(const std::string &winname, int prop_id,
                       double prop_value) {
  cv::setWindowProperty(winname, prop_id, prop_value);
}

void setWindowTitle(const std::string &view, const std::string &title) {
  Window::current().view(view).title(title);
}

auto startWindowThread() -> int { return cv::startWindowThread(); }

auto waitKey(int delay) -> int { return Util::key(delay); }

auto waitKeyEx(int delay) -> int { return Util::key(delay); }

}  // namespace cvplot

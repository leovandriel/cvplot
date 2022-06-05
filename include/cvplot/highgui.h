#ifndef CVPLOT_HIGHGUI_H
#define CVPLOT_HIGHGUI_H

#include <string>
#include <vector>

#include "window.h"

namespace cvplot {

auto createTrackbar(const std::string &trackbarname, const std::string &winname,
                    int *value, int count, TrackbarCallback onChange = nullptr,
                    void *userdata = nullptr) -> int;
void destroyAllWindows();
void destroyWindow(const std::string &view);
auto getMouseWheelDelta(int flags) -> int;
auto getTrackbarPos(const std::string &trackbarname, const std::string &winname)
    -> int;
auto getWindowProperty(const std::string &winname, int prop_id) -> double;
void imshow(const std::string &view, void *img);
void moveWindow(const std::string &view, int x, int y);
void namedWindow(const std::string &view, int flags = 0);
void resizeWindow(const std::string &view, int width, int height);
void resizeWindow(const std::string &view, const Size &size);
void setMouseCallback(const std::string &view, MouseCallback onMouse,
                      void *userdata = nullptr);
void setTrackbarMax(const std::string &trackbarname, const std::string &winname,
                    int maxval);
void setTrackbarMin(const std::string &trackbarname, const std::string &winname,
                    int minval);
void setTrackbarPos(const std::string &trackbarname, const std::string &winname,
                    int pos);
void setWindowProperty(const std::string &winname, int prop_id,
                       double prop_value);
void setWindowTitle(const std::string &view, const std::string &title);
auto startWindowThread() -> int;
auto waitKey(int delay = 0) -> int;
auto waitKeyEx(int delay = 0) -> int;

}  // namespace cvplot

#endif  // CVPLOT_HIGHGUI_H

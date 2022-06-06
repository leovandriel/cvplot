#include <opencv2/opencv.hpp>

#include "cvplot/cvplot.h"

namespace demo {

auto uniform() -> double {
  // NOLINTNEXTLINE
  return std::rand() / static_cast<double>(RAND_MAX);
}

void example() {
  cvplot::figure("myplot").series("myline").addValue({1., 3., 2., 5., 4.});
  cvplot::figure("myplot").show();
}

void demo_highgui() {
  std::vector<std::pair<double, double>> data;
  std::vector<double> values;

  cvplot::Window::current("cvplot demo").offset({60, 100});

  {
    const auto *name = "simple";
    cvplot::setWindowTitle(name, "line and histogram");
    cvplot::moveWindow(name, 0, 0);
    cvplot::resizeWindow(name, 300, 300);
    auto &figure = cvplot::figure(name);
    figure.series("line")
        .setValue({1., 2., 3., 4., 5.})
        .type(cvplot::DotLine)
        .color(cvplot::Blue);
    figure.series("histogram")
        .setValue({1., 2., 3., 4., 5.})
        .type(cvplot::Histogram)
        .color(cvplot::Red);
    figure.show(false);
  }

  {
    const auto *name = "math";
    cvplot::setWindowTitle(name, "math curves");
    cvplot::moveWindow(name, 300, 0);
    cvplot::resizeWindow(name, 300, 300);
    auto &figure = cvplot::figure(name);
    values.clear();
    for (auto i = 0; i <= 10; i++) {
      values.push_back((i - 4) * (i - 4) - 6);
    }
    figure.series("parabola")
        .setValue(values)
        .type(cvplot::DotLine)
        .color(cvplot::Green);
    values.clear();
    for (auto i = 0; i <= 10; i++) {
      values.push_back(sin(i / 1.5) * 5);
    }
    figure.series("sine")
        .setValue(values)
        .type(cvplot::DotLine)
        .color(cvplot::Blue);
    values.clear();
    values.push_back(15);
    figure.series("threshold")
        .setValue(values)
        .type(cvplot::Horizontal)
        .color(cvplot::Red);
    figure.show(false);
  }

  {
    const auto *name = "scatter";
    cvplot::setWindowTitle(name, "scatter plots");
    cvplot::moveWindow(name, 600, 0);
    cvplot::resizeWindow(name, 300, 300);
    auto &figure = cvplot::figure(name);
    data.clear();
    for (auto i = 0; i <= 100; i++) {
      data.emplace_back(uniform() * 10., uniform() * 10.);
    }
    figure.series("uniform").set(data).type(cvplot::Dots).color(cvplot::Orange);
    data.clear();
    for (auto i = 0; i <= 100; i++) {
      data.emplace_back(exp(uniform() * 3.33) - 1, exp(uniform() * 3.33) - 1);
    }
    figure.series("exponential")
        .set(data)
        .type(cvplot::Dots)
        .color(cvplot::Magenta);
    figure.show(false);
  }

  {
    const auto *name = "histograms";
    cvplot::setWindowTitle(name, "multiple histograms");
    cvplot::moveWindow(name, 0, 300);
    cvplot::resizeWindow(name, 300, 300);
    auto &figure = cvplot::figure(name);
    figure.series("1")
        .setValue({1., 2., 3., 4., 5.})
        .type(cvplot::Histogram)
        .color(cvplot::Blue.alpha(201));
    figure.series("2")
        .setValue({6., 5., 4., 3., 2., 1.})
        .type(cvplot::Histogram)
        .color(cvplot::Green.alpha(201));
    figure.series("3")
        .setValue({3., 1., -1., 1., 3., 7.})
        .type(cvplot::Histogram)
        .color(cvplot::Red.alpha(201));
    figure.show(false);
  }

  {
    const auto *name = "parametric";
    cvplot::setWindowTitle(name, "parametric plots");
    cvplot::moveWindow(name, 0, 600);
    cvplot::resizeWindow(name, 300, 300);
    auto &figure = cvplot::figure(name);
    figure.square(true);
    data.clear();
    for (auto i = 0; i <= 100; i++) {
      data.emplace_back(cos(i * .0628 + 4) * 2, sin(i * .0628 + 4) * 2);
    }
    figure.series("circle").add(data);
    data.clear();
    for (auto i = 0; i <= 100; i++) {
      data.emplace_back(cos(i * .2513 + 1), sin(i * .0628 + 4));
    }
    figure.series("lissajous").add(data);
    figure.show(false);
  }

  {
    const auto *name = "no-axis";
    cvplot::setWindowTitle(name, "hidden axis");
    cvplot::moveWindow(name, 600, 600);
    cvplot::resizeWindow(name, 300, 300);
    auto &figure = cvplot::figure(name);
    figure.origin(false, false);
    figure.series("histogram")
        .setValue({4., 5., 7., 6.})
        .type(cvplot::Vistogram)
        .color(cvplot::Blue);
    figure.series("min")
        .setValue(4.)
        .type(cvplot::Vertical)
        .color(cvplot::Pink);
    figure.series("max")
        .setValue(7.)
        .type(cvplot::Vertical)
        .color(cvplot::Purple);
    figure.show(false);
  }

  {
    const auto *name = "colors";
    cvplot::setWindowTitle(name, "auto color");
    cvplot::moveWindow(name, 900, 0);
    cvplot::resizeWindow(name, 300, 300);
    auto &figure = cvplot::figure(name);
    figure.series("color")
        .dynamicColor(true)
        .type(cvplot::Vistogram)
        .legend(false);
    for (auto i = 0; i < 16; i++) {
      figure.series("color").addValue(6, cvplot::Color::index(i).hue());
    }
    figure.show(false);
  }

  {
    const auto *name = "fill";
    cvplot::setWindowTitle(name, "filled line");
    cvplot::moveWindow(name, 900, 0);
    cvplot::resizeWindow(name, 300, 300);
    auto &figure = cvplot::figure(name);
    figure.gridSize(20);
    figure.series("fossil").type(cvplot::FillLine).color(cvplot::Orange);
    figure.series("electric")
        .type(cvplot::FillLine)
        .color(cvplot::Green.gamma(.5));
    for (auto i = 0; i < 16; i++) {
      figure.series("fossil").addValue(10 - i + 10. * uniform());
      figure.series("electric").addValue(i - 10 + 10. * uniform());
    }
    figure.show(false);
  }

  {
    const auto *name = "range";
    cvplot::setWindowTitle(name, "range plot");
    cvplot::moveWindow(name, 900, 300);
    cvplot::resizeWindow(name, 300, 300);
    auto &figure = cvplot::figure(name);
    values.clear();
    figure.series("apples").type(cvplot::RangeLine).color(cvplot::Orange);
    figure.series("pears").type(cvplot::RangeLine).color(cvplot::Sky);
    for (auto i = 0; i <= 10; i++) {
      double v = (i - 4) * (i - 4) - 6;
      figure.series("apples").addValue(v + 10. + 5. * uniform(),
                                       v + 5. * uniform(),
                                       v + 20. + 5. * uniform());
      v = -(i - 6) * (i - 6) + 30;
      figure.series("pears").addValue(v + 10. + 5. * uniform(),
                                      v + 5. * uniform(),
                                      v + 20. + 5. * uniform());
    }
    figure.show(false);
  }

  {
    const auto *name = "balls";
    cvplot::setWindowTitle(name, "transparent circles");
    cvplot::moveWindow(name, 300, 600);
    cvplot::resizeWindow(name, 300, 300);
    auto &figure = cvplot::figure(name);
    figure.series("purple")
        .type(cvplot::Circle)
        .color(cvplot::Purple.alpha(192));
    figure.series("aqua").type(cvplot::Circle).color(cvplot::Aqua.alpha(193));
    for (auto i = 0; i <= 20; i++) {
      figure.series("purple").add(uniform() * 10.,
                                  {uniform() * 10., uniform() * 20.});
      figure.series("aqua").add(uniform() * 10.,
                                {uniform() * 10., uniform() * 20.});
    }
    figure.show(false);
  }

  {
    const auto *name = "image";
    cvplot::setWindowTitle(name, "image and text");
    cvplot::moveWindow(name, 900, 600);
    cvplot::resizeWindow(name, 300, 300);
    auto &view = cvplot::Window::current().view(name);
    auto image = cv::imread("res/demo.jpg");
    if (image.data != nullptr) {
      cv::copyMakeBorder(image, image, 100, 100, 0, 0, cv::BORDER_REPLICATE);
      view.drawImage(&image);
    }
    view.drawText("..and text", {13, 273}, cvplot::Black.alpha(127));
    view.finish();
  }

  {
    const auto *name = "dynamic";
    cvplot::setWindowTitle(name, "dynamic plotting");
    cvplot::moveWindow(name, 300, 300);
    cvplot::resizeWindow(name, 600, 300);
    auto &view = cvplot::Window::current().view(name);
    auto &figure = cvplot::figure(name);
    figure.square(true);
    figure.origin(false, false);
    auto x = 0.;
    auto y = 0.;
    auto dx = 1.;
    auto dy = 0.;
    auto f = 0.;
    auto df = 0.;
    figure.series("random").dynamicColor(true).legend(false);
    clock_t time = 0;
    for (int i = 0; i < 1000; i++) {
      auto fps = CLOCKS_PER_SEC / static_cast<double>(clock() - time);
      time = clock();
      auto l = sqrt((dx * dx + dy * dy) * (f * f + 1)) * 10;
      dx = (dx + f * dy) / l;
      dy = (dy - f * dx) / l;
      f = (f + df) * 0.8;
      df = (df + uniform() * .11 - .05) * 0.8;
      figure.series("random").add(x += dx, {y += dy, i / 10.});
      figure.show(false);
      auto string = std::to_string(fps).substr(0, 4) + " fps  " +
                    std::to_string(i / 10.).substr(0, 4) + "%";
      view.drawText(string, {480, 277}, cvplot::Gray);
      view.finish();
      view.flush();
    }
    view.drawTextShadow("Press any key to exit", {70, 130}, cvplot::Red, 40.);
    view.flush();
  }
}

void mouse_callback(int event, int x, int y, int flags, void *param) {
  auto &view = *static_cast<cvplot::View *>(param);
  std::ostringstream stream;
  if (event == cv::EVENT_MOUSEMOVE) {
    stream << "mousemove";
  }
  if (event == cv::EVENT_LBUTTONDOWN) {
    stream << "lbuttondown";
  }
  if (event == cv::EVENT_RBUTTONDOWN) {
    stream << "rbuttondown";
  }
  if (event == cv::EVENT_MBUTTONDOWN) {
    stream << "mbuttondown";
  }
  if (event == cv::EVENT_LBUTTONUP) {
    stream << "lbuttonup";
  }
  if (event == cv::EVENT_RBUTTONUP) {
    stream << "rbuttonup";
  }
  if (event == cv::EVENT_MBUTTONUP) {
    stream << "mbuttonup";
  }
  if (event == cv::EVENT_LBUTTONDBLCLK) {
    stream << "lbuttondblclk";
  }
  if (event == cv::EVENT_RBUTTONDBLCLK) {
    stream << "rbuttondblclk";
  }
  if (event == cv::EVENT_MBUTTONDBLCLK) {
    stream << "mbuttondblclk";
  }
#if CV_MAJOR_VERSION >= 3
  if (event == cv::EVENT_MOUSEWHEEL) {
    stream << "mousewheel";
  }
  if (event == cv::EVENT_MOUSEHWHEEL) {
    stream << "mousehwheel";
  }
#endif
  if ((flags & cv::EVENT_FLAG_LBUTTON) != 0) {
    stream << " lbutton";
  }
  if ((flags & cv::EVENT_FLAG_RBUTTON) != 0) {
    stream << " rbutton";
  }
  if ((flags & cv::EVENT_FLAG_MBUTTON) != 0) {
    stream << " mbutton";
  }
  if ((flags & cv::EVENT_FLAG_CTRLKEY) != 0) {
    stream << " ctrlkey";
  }
  if ((flags & cv::EVENT_FLAG_SHIFTKEY) != 0) {
    stream << " shiftkey";
  }
  if ((flags & cv::EVENT_FLAG_ALTKEY) != 0) {
    stream << " altkey";
  }
  stream << "  " << x << "," << y;
  view.drawRect({10, 24, 280, 12}, cvplot::Light);
  view.drawText(stream.str(), {10, 25}, cvplot::Black);
}

auto transparency() -> std::unique_ptr<cvplot::Window> {
  const std::string title = "cvplot transparency and mouse";
  std::unique_ptr<cvplot::Window> window_ptr(new cvplot::Window(title));

  auto &window = (*window_ptr).offset({30, 70}).cursor(true);

  {
    auto &view = window.view("opaque", {300, 300});
    view.mouse(mouse_callback).frameColor(cvplot::Sky);
    auto figure = cvplot::Figure(view);
    figure.series("histogram")
        .setValue({1., 2., 3., 4., 5.})
        .type(cvplot::Histogram)
        .color(cvplot::Red)
        .legend(false);
    figure.show(false);
  }

  {
    auto &view = window.view("transparent", {300, 300});
    view.mouse(mouse_callback)
        .frameColor(cvplot::Sky)
        .alpha(200)
        .offset({100, 100});
    auto figure = cvplot::Figure(view);
    figure.alpha(200);
    figure.series("histogram")
        .setValue({5., 4., 3., 2., 1.})
        .type(cvplot::Histogram)
        .color(cvplot::Blue.alpha(200))
        .legend(false);
    figure.show(true);
  }

  return window_ptr;
}

}  // namespace demo

auto main(int /*argc*/, char ** /*argv*/) -> int {
  demo::example();
  auto window2 = demo::transparency();
  demo::demo_highgui();
  std::cout << "Demo completed. Ctrl+C to exit." << std::endl;
  cvplot::waitKey();
  window2.release();
  return 0;
}

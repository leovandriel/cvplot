#include "cvplot/cvplot.h"

#include <opencv2/opencv.hpp>

namespace demo {

void example() {
  cvplot::figure("myplot").series("myline").setValue({1.f, 3.f, 2.f, 5.f, 4.f});
  cvplot::figure("myplot").show();
}

void demo() {
  std::vector<std::pair<float, float>> data;
  std::vector<float> values;

  cvplot::window("cvplot demo");
  cvplot::move(60, 100);

  {
    auto name = "simple";
    cvplot::title(name, "line and histogram");
    cvplot::move(name, 0, 0);
    cvplot::resize(name, 300, 300);
    auto &figure = cvplot::figure(name);
    figure.series("line")
        .setValue({1.f, 2.f, 3.f, 4.f, 5.f})
        .type(cvplot::DotLine)
        .color(cvplot::Blue);
    figure.series("histogram")
        .setValue({1.f, 2.f, 3.f, 4.f, 5.f})
        .type(cvplot::Histogram)
        .color(cvplot::Red);
    figure.show(false);
  }

  {
    auto name = "math";
    cvplot::title(name, "math curves");
    cvplot::move(name, 300, 0);
    cvplot::resize(name, 300, 300);
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
      values.push_back(sin(i / 1.5f) * 5);
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
    auto name = "scatter";
    cvplot::title(name, "scatter plots");
    cvplot::move(name, 600, 0);
    cvplot::resize(name, 300, 300);
    auto &figure = cvplot::figure(name);
    data.clear();
    for (auto i = 0; i <= 100; i++) {
      data.push_back({(rand() % 100) / 10.f, (rand() % 100) / 10.f});
    }
    figure.series("uniform").set(data).type(cvplot::Dots).color(cvplot::Orange);
    data.clear();
    for (auto i = 0; i <= 100; i++) {
      data.push_back(
          {exp((rand() % 100) / 30.f) - 1, exp((rand() % 100) / 30.f) - 1});
    }
    figure.series("exponential")
        .set(data)
        .type(cvplot::Dots)
        .color(cvplot::Magenta);
    figure.show(false);
  }

  {
    auto name = "histograms";
    cvplot::title(name, "multiple histograms");
    cvplot::move(name, 0, 300);
    cvplot::resize(name, 300, 300);
    auto &figure = cvplot::figure(name);
    figure.series("1")
        .setValue({1.f, 2.f, 3.f, 4.f, 5.f})
        .type(cvplot::Histogram)
        .color(cvplot::Blue.alpha(201));
    figure.series("2")
        .setValue({6.f, 5.f, 4.f, 3.f, 2.f, 1.f})
        .type(cvplot::Histogram)
        .color(cvplot::Green.alpha(201));
    figure.series("3")
        .setValue({3.f, 1.f, -1.f, 1.f, 3.f, 7.f})
        .type(cvplot::Histogram)
        .color(cvplot::Red.alpha(201));
    figure.show(false);
  }

  {
    auto name = "parametric";
    cvplot::title(name, "parametric plots");
    cvplot::move(name, 0, 600);
    cvplot::resize(name, 300, 300);
    auto &figure = cvplot::figure(name);
    figure.square(true);
    data.clear();
    for (auto i = 0; i <= 100; i++) {
      data.push_back({cos(i * .0628f + 4) * 2, sin(i * .0628f + 4) * 2});
    }
    figure.series("circle").add(data);
    data.clear();
    for (auto i = 0; i <= 100; i++) {
      data.push_back({cos(i * .2513f + 1), sin(i * .0628f + 4)});
    }
    figure.series("lissajous").add(data);
    figure.show(false);
  }

  {
    auto name = "no-axis";
    cvplot::title(name, "hidden axis");
    cvplot::move(name, 600, 600);
    cvplot::resize(name, 300, 300);
    auto &figure = cvplot::figure(name);
    figure.origin(false, false);
    figure.series("histogram")
        .setValue({4.f, 5.f, 7.f, 6.f})
        .type(cvplot::Vistogram)
        .color(cvplot::Blue);
    figure.series("min")
        .setValue(4.f)
        .type(cvplot::Vertical)
        .color(cvplot::Pink);
    figure.series("max")
        .setValue(7.f)
        .type(cvplot::Vertical)
        .color(cvplot::Purple);
    figure.show(false);
  }

  {
    auto name = "colors";
    cvplot::title(name, "auto color");
    cvplot::move(name, 900, 0);
    cvplot::resize(name, 300, 300);
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
    auto name = "fill";
    cvplot::title(name, "filled line");
    cvplot::move(name, 900, 0);
    cvplot::resize(name, 300, 300);
    auto &figure = cvplot::figure(name);
    figure.gridSize(20);
    figure.series("fossil").type(cvplot::FillLine).color(cvplot::Orange);
    figure.series("electric")
        .type(cvplot::FillLine)
        .color(cvplot::Green.gamma(.5f));
    for (auto i = 0; i < 16; i++) {
      figure.series("fossil").addValue(10 - i + 10.f * rand() / RAND_MAX);
      figure.series("electric").addValue(i - 10 + 10.f * rand() / RAND_MAX);
    }
    figure.show(false);
  }

  {
    auto name = "range";
    cvplot::title(name, "range plot");
    cvplot::move(name, 900, 300);
    cvplot::resize(name, 300, 300);
    auto &figure = cvplot::figure(name);
    values.clear();
    figure.series("apples").type(cvplot::RangeLine).color(cvplot::Orange);
    figure.series("pears").type(cvplot::RangeLine).color(cvplot::Sky);
    for (auto i = 0; i <= 10; i++) {
      float v = (i - 4) * (i - 4) - 6;
      figure.series("apples").addValue(v + 10.f + 5.f * rand() / RAND_MAX,
                                       v + 5.f * rand() / RAND_MAX,
                                       v + 20.f + 5.f * rand() / RAND_MAX);
      v = -(i - 6) * (i - 6) + 30;
      figure.series("pears").addValue(v + 10.f + 5.f * rand() / RAND_MAX,
                                      v + 5.f * rand() / RAND_MAX,
                                      v + 20.f + 5.f * rand() / RAND_MAX);
    }
    figure.show(false);
  }

  {
    auto name = "balls";
    cvplot::title(name, "transparent circles");
    cvplot::move(name, 300, 600);
    cvplot::resize(name, 300, 300);
    auto &figure = cvplot::figure(name);
    figure.series("purple")
        .type(cvplot::Circle)
        .color(cvplot::Purple.alpha(192));
    figure.series("aqua").type(cvplot::Circle).color(cvplot::Aqua.alpha(193));
    for (auto i = 0; i <= 20; i++) {
      figure.series("purple").add(
          (rand() % 100) / 10.f, {(rand() % 100) / 10.f, (rand() % 100) / 5.f});
      figure.series("aqua").add((rand() % 100) / 10.f,
                                {(rand() % 100) / 10.f, (rand() % 100) / 5.f});
    }
    figure.show(false);
  }

  {
    auto name = "image";
    cvplot::title(name, "image and text");
    cvplot::move(name, 900, 600);
    cvplot::resize(name, 300, 300);
    auto &view = cvplot::window().view(name);
    auto image = cv::imread("res/demo.jpg");
    cv::copyMakeBorder(image, image, 100, 100, 0, 0, cv::BORDER_REPLICATE);
    view.drawImage(&image);
    view.drawText("..and text", {13, 273}, cvplot::Black.alpha(127));
    view.show(false);
  }

  {
    auto name = "dynamic";
    cvplot::title(name, "dynamic plotting");
    cvplot::move(name, 300, 300);
    cvplot::resize(name, 600, 300);
    auto &view = cvplot::window().view(name);
    auto &figure = cvplot::figure(name);
    figure.square(true);
    figure.origin(false, false);
    srand(clock());
    auto x = 0.f, y = 0.f, dx = 1.f, dy = 0.f, f = 0.f, df = 0.f;
    figure.series("random").dynamicColor(true).legend(false);
    clock_t time = 0;
    for (int i = 0; i < 1000; i++) {
      auto fps = CLOCKS_PER_SEC / (float)(clock() - time);
      time = clock();
      auto l = sqrt((dx * dx + dy * dy) * (f * f + 1)) * 10;
      dx = (dx + f * dy) / l;
      dy = (dy - f * dx) / l;
      f = (f + df) * 0.8f;
      df = (df + rand() % 11 / 100.f - .05f) * 0.8f;
      figure.series("random").add(x += dx, {y += dy, i / 10.f});
      figure.show(false);
      auto string = std::to_string(fps).substr(0, 4) + " fps  " +
                    std::to_string(i / 10.f).substr(0, 4) + "%";
      view.drawText(string, {480, 277}, cvplot::Gray);
      view.show();
    }
  }
}

void transparency() {
  std::vector<std::pair<float, float>> data;
  std::vector<float> values;

  cvplot::window("cvplot transparency");
  cvplot::move(30, 70);

  {
    auto name = "opaque";
    cvplot::title(name, "opaque");
    cvplot::move(name, 0, 0);
    cvplot::resize(name, 300, 300);
    cvplot::window().view(name).frameColor(cvplot::Sky);
    auto &figure = cvplot::figure(name);
    figure.series("histogram")
        .setValue({1.f, 2.f, 3.f, 4.f, 5.f})
        .type(cvplot::Histogram)
        .color(cvplot::Red)
        .legend(false);
    figure.show(false);
  }

  {
    auto name = "transparent";
    auto alpha = 200;
    cvplot::title(name, "transparent");
    cvplot::move(name, 100, 100);
    cvplot::resize(name, 300, 300);
    cvplot::window().view(name).frameColor(cvplot::Sky);
    cvplot::window().view(name).alpha(alpha);
    auto &figure = cvplot::figure(name);
    figure.series("histogram")
        .setValue({5.f, 4.f, 3.f, 2.f, 1.f})
        .type(cvplot::Histogram)
        .color(cvplot::Blue.alpha(alpha))
        .legend(false);
    figure.alpha(alpha).show(true);
  }
}

}  // namespace demo

int main(int argc, char **argv) {
  demo::example();
  demo::transparency();
  demo::demo();
  return 0;
}

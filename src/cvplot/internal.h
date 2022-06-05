#ifndef CVPLOT_INTERNAL_H
#define CVPLOT_INTERNAL_H

#include <iomanip>
#include <iostream>
#include <opencv2/core/core.hpp>

#define EXPECT_EQ(a__, b__)                                                    \
  do {                                                                         \
    if ((a__) != (b__)) {                                                      \
      std::cerr << "Incorrect " << #a__ << " (" << (a__) << "), should equal " \
                << (b__) << std::endl;                                         \
    }                                                                          \
  } while (0)

namespace cvplot {

static auto color2scalar(const Color &color) -> cv::Scalar {
  return {(double)color.b, (double)color.g, (double)color.r};
}

static auto value2snap(double value) -> double {
  return std::max({pow(10, floor(log10(value))),
                   pow(10, floor(log10(value / 2))) * 2,
                   pow(10, floor(log10(value / 5))) * 5});
}

class Trans {
 public:
  Trans(void *buffer) : Trans(*(cv::Mat *)buffer) {}

  Trans(cv::Mat &buffer) : original_(buffer), alpha_(0), interim_(nullptr) {}

  Trans(cv::Mat &buffer, int alpha) : Trans(buffer) { setup(alpha); }

  ~Trans() { flush(); }

  auto get() const -> cv::Mat & {
    return (interim_ != nullptr ? *interim_ : original_);
  }

  void setup(int alpha) {
    bool transparent = (alpha != 255);
    if (transparent) {
      interim_ = new cv::Mat();
      original_.copyTo(*interim_);
    }
    alpha_ = alpha;
  }

  void flush() {
    if (interim_) {
      // std::cerr << "blending " << alpha_ << std::endl;
      auto weight = alpha_ / 255.;
      cv::addWeighted(*interim_, weight, original_, 1 - weight, 0, original_);
      delete interim_;
      interim_ = nullptr;
    }
  }

  auto with(int alpha) -> cv::Mat & {
    if (alpha != alpha_) {
      flush();
      setup(alpha);
    }
    return get();
  }

  auto with(const Color &color) -> cv::Mat & { return with(color.a); }

 protected:
  int alpha_;
  cv::Mat &original_;
  cv::Mat *interim_;
};

}  // namespace cvplot

#endif  // CVPLOT_INTERNAL_H

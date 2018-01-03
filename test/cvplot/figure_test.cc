#include "cvplot/figure.h"

#include <gtest/gtest.h>

namespace cvplot {

TEST(FigureTest, Init) {
  Window w;
  View v(w);
  Figure f(v);
}

}  // namespace cvplot

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

#include "cvplot/figure.h"

#include <gtest/gtest.h>

#include <cstdio>

namespace cvplot {

TEST(FigureTest, Init) {
  Window w;
  View v(w);
  Figure f(v);
}

TEST(FigureTest, File) {
  const auto *filename = "test/figure.png";
  auto f = figure("test-figure");
  f.series("test-series").addValue({1.F, 3.F, 2.F, 5.F, 4.F});
  auto result = f.drawFile(filename, {500, 500});
  EXPECT_EQ(result, true);
  EXPECT_EQ(remove(filename), 0);
}

}  // namespace cvplot

auto main(int argc, char **argv) -> int {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

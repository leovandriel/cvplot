#include "cvplot/figure.h"

#include <gtest/gtest.h>
#include <stdio.h>

namespace cvplot {

TEST(FigureTest, Init) {
  Window w;
  View v(w);
  Figure f(v);
}

TEST(FigureTest, File) {
  auto filename = "test/figure.png";
  auto f = figure("test-figure");
  f.series("test-series").addValue({1.f, 3.f, 2.f, 5.f, 4.f});
  auto result = f.drawFile(filename, {500, 500});
  EXPECT_EQ(result, true);
  EXPECT_EQ(remove(filename), 0);
}

}  // namespace cvplot

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

#include "cvplot/figure.h"

#include <gtest/gtest.h>

namespace cvplot {

TEST(FigureTest, Init) { class Figure f; }

}  // namespace cvplot

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

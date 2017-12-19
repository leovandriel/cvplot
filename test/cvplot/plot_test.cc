#include "cvplot/plot.h"

#include <gtest/gtest.h>

namespace cvplot {

TEST(PlotTest, Init) { class Plot p; }

}  // namespace cvplot

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

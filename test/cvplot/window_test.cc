#include "cvplot/window.h"

#include <gtest/gtest.h>

namespace cvplot {

TEST(WindowTest, Init) { Window w; }

}  // namespace cvplot

auto main(int argc, char **argv) -> int {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

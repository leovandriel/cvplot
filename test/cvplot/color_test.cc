#include "cvplot/color.h"

#include <gtest/gtest.h>

namespace cvplot {

TEST(ColorTest, Init) {
  Color c;
  EXPECT_EQ(c.r, 0);
  EXPECT_EQ(c.g, 0);
  EXPECT_EQ(c.b, 0);
  EXPECT_EQ(c.a, 255);
}

TEST(ColorTest, Init1) {
  uint8_t v[3] = {1, 2, 3};
  Color c(v, 4);
  EXPECT_EQ(c.r, 1);
  EXPECT_EQ(c.g, 2);
  EXPECT_EQ(c.b, 3);
  EXPECT_EQ(c.a, 4);
  Color d(v);
  EXPECT_EQ(d.a, 255);
}

TEST(ColorTest, Init2) {
  Color c(1, 2, 3, 4);
  EXPECT_EQ(c.r, 1);
  EXPECT_EQ(c.g, 2);
  EXPECT_EQ(c.b, 3);
  EXPECT_EQ(c.a, 4);
  Color d(1, 2, 3);
  EXPECT_EQ(d.a, 255);
}

TEST(ColorTest, Alpha) {
  Color c(1, 2, 3, 4);
  EXPECT_EQ(c.alpha(5).a, 5);
}

TEST(ColorTest, Gray) {
  Color c = Color::gray(3);
  EXPECT_EQ(c.r, 3);
  EXPECT_EQ(c.g, 3);
  EXPECT_EQ(c.b, 3);
  EXPECT_EQ(c.a, 255);
}

TEST(ColorTest, Hue) {
  Color c = Color::hue(3);
  EXPECT_EQ(c.r, 0);
  EXPECT_EQ(c.g, 255);
  EXPECT_EQ(c.b, 255);
  EXPECT_EQ(c.a, 255);
}

TEST(ColorTest, Cos) {
  Color c = Color::cos(3);
  EXPECT_EQ(c.r, 0);
  EXPECT_EQ(c.g, 191);
  EXPECT_EQ(c.b, 191);
  EXPECT_EQ(c.a, 255);
}

TEST(ColorTest, Index) {
  Color c = Color::index(3);
  EXPECT_EQ(c.r, 37);
  EXPECT_EQ(c.g, 94);
  EXPECT_EQ(c.b, 251);
  EXPECT_EQ(c.a, 255);
}

TEST(ColorTest, Hash) {
  Color c = Color::hash("test");
  EXPECT_EQ(c.r, 160);
  EXPECT_EQ(c.g, 4);
  EXPECT_EQ(c.b, 218);
  EXPECT_EQ(c.a, 255);
}

TEST(ColorTest, Red) {
  EXPECT_EQ(Red.r, 255);
  EXPECT_EQ(Red.g, 0);
  EXPECT_EQ(Red.b, 0);
  EXPECT_EQ(Red.a, 255);
}

TEST(ColorTest, Green) {
  EXPECT_EQ(Green.r, 0);
  EXPECT_EQ(Green.g, 255);
  EXPECT_EQ(Green.b, 0);
  EXPECT_EQ(Green.a, 255);
}

TEST(ColorTest, Blue) {
  EXPECT_EQ(Blue.r, 0);
  EXPECT_EQ(Blue.g, 0);
  EXPECT_EQ(Blue.b, 255);
  EXPECT_EQ(Blue.a, 255);
}

}  // namespace cvplot

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

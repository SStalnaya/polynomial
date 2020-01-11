#include "polynomial.h"
#include <gtest/gtest.h>

TEST(constructor, stringconstructor) {
  polynomial<int> p1 ("1+x");
  polynomial<int> q1 {1,1};
  EXPECT_EQ(p1, q1);
  polynomial<int> p2 ("x");
  polynomial<int> q2 {0,1};
  EXPECT_EQ(p2, q2);
  polynomial<int> p3 ("x^2");
  polynomial<int> q3 {0,0,1};
  EXPECT_EQ(p3, q3);
  polynomial<int> p4 ("-1");
  polynomial<int> q4 {-1};
  EXPECT_EQ(p4, q4);
  polynomial<int> p5 ("0");
  polynomial<int> q5 {};;
  EXPECT_EQ(p5, q5);
  polynomial<int> p6 ("-x");
  polynomial<int> q6 {0,-1};
  EXPECT_EQ(p6, q6);
  polynomial<int> p7 ("-x^2");
  polynomial<int> q7 {0,0,-1};
  EXPECT_EQ(p7, q7);
  polynomial<int> p8 ("2");
  polynomial<int> q8 {2};
  EXPECT_EQ(p8, q8);
  polynomial<int> p9 ("1");
  polynomial<int> q9 {1};;
  EXPECT_EQ(p9, q9);
  polynomial<int> p10 ("-1");
  polynomial<int> q10 {-1};
  EXPECT_EQ(p10, q10);
  polynomial<int> p11 ("-2");
  polynomial<int> q11 {-2};
  EXPECT_EQ(p11, q11);
  polynomial<int> p12 ("-3x^2");
  polynomial<int> q12 {0,0,-3};
  EXPECT_EQ(p12, q12);
  polynomial<int> p13 ("4x^3");
  polynomial<int> q13 {0,0,0,4};
  EXPECT_EQ(p13, q13);
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

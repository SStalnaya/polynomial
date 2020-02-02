#include <gtest/gtest.h>
#include "polynomial.h"

TEST(constructors, string) {
  polynomial<int> p1("1+x");
  polynomial<int> q1{1, 1};
  EXPECT_EQ(p1, q1);
  polynomial<int> p2("x");
  polynomial<int> q2{0, 1};
  EXPECT_EQ(p2, q2);
  polynomial<int> p3("x^2");
  polynomial<int> q3{0, 0, 1};
  EXPECT_EQ(p3, q3);
  polynomial<int> p4("-1");
  polynomial<int> q4{-1};
  EXPECT_EQ(p4, q4);
  polynomial<int> p5("0");
  polynomial<int> q5{};
  ;
  EXPECT_EQ(p5, q5);
  polynomial<int> p6("-x");
  polynomial<int> q6{0, -1};
  EXPECT_EQ(p6, q6);
  polynomial<int> p7("-x^2");
  polynomial<int> q7{0, 0, -1};
  EXPECT_EQ(p7, q7);
  polynomial<int> p8("2");
  polynomial<int> q8{2};
  EXPECT_EQ(p8, q8);
  polynomial<int> p9("1");
  polynomial<int> q9{1};
  ;
  EXPECT_EQ(p9, q9);
  polynomial<int> p10("-1");
  polynomial<int> q10{-1};
  EXPECT_EQ(p10, q10);
  polynomial<int> p11("-2");
  polynomial<int> q11{-2};
  EXPECT_EQ(p11, q11);
  polynomial<int> p12("-3x^2");
  polynomial<int> q12{0, 0, -3};
  EXPECT_EQ(p12, q12);
  polynomial<int> p13("4x^3");
  polynomial<int> q13{0, 0, 0, 4};
  EXPECT_EQ(p13, q13);
  polynomial<int> p14("x-x");
  polynomial<int> q14{};
  EXPECT_EQ(p14, q14);
  polynomial<int> p15("x^2+x");
  polynomial<int> q15{0, 1, 1};
  EXPECT_EQ(p15, q15);
  polynomial<int> p16("x^3-2");
  polynomial<int> q16{-2, 0, 0, 1};
  EXPECT_EQ(p16, q16);
  polynomial<int> p17("x^2-x^3");
  polynomial<int> q17{0, 0, 1, -1};
  EXPECT_EQ(p17, q17);
  polynomial<int> p18("x^4-3x");
  polynomial<int> q18{0, -3, 0, 0, 1};
  EXPECT_EQ(p18, q18);
  polynomial<int> p19("1+x-4x^2+x^3-4x-5x^3+7+4x^2");
  polynomial<int> q19{8, -3, 0, -4};
  EXPECT_EQ(p19, q19);
  polynomial<int> p20("-3x+2");
  polynomial<int> q20{2, -3};
  EXPECT_EQ(p20, q20);
  polynomial<int> p21("-5x^2+x+1");
  polynomial<int> q21{1, 1, -5};
  EXPECT_EQ(p21, q21);
}

TEST(operators, addition) {
  polynomial<int> p1{1};
  polynomial<int> q1{1};
  polynomial<int> r1{2};
  EXPECT_EQ(p1 + q1, r1);
  polynomial<int> p2{0, 1};
  polynomial<int> q2{1, 0};
  polynomial<int> r2{1, 1};
  EXPECT_EQ(p2 + q2, r2);
  polynomial<int> p3{3};
  polynomial<int> q3{-3};
  polynomial<int> r3{};
  EXPECT_EQ(p3 + q3, r3);
  polynomial<int> p4{1, 2, 2};
  polynomial<int> q4{0, 1, -2};
  polynomial<int> r4{1, 3};
  EXPECT_EQ(p4 + q4, r4);
  polynomial<int> p5{4, -2, 5, 1, 0, -1, 4};
  polynomial<int> q5{-2, 2, 4, 7, 3, 1, -4};
  polynomial<int> r5{2, 0, 9, 8, 3};
  EXPECT_EQ(p5 + q5, r5);
  polynomial<int> p6{1};
  polynomial<int> q6{-1};
  polynomial<int> r6{};
  EXPECT_EQ(p6 + q6, r6);
}

TEST(operators, negation) {
  polynomial<int> p1{1};
  polynomial<int> q1{-1};
  EXPECT_EQ(-p1, q1);
  polynomial<int> p2{-1, -1};
  polynomial<int> q2{1, 1};
  EXPECT_EQ(-p2, q2);
  polynomial<int> p3{0, 1};
  polynomial<int> q3{0, -1};
  EXPECT_EQ(-p3, q3);
  polynomial<int> p4{7, -3, 2, 0, 6, -3, 2, -1};
  polynomial<int> q4{-7, 3, -2, 0, -6, 3, -2, 1};
  EXPECT_EQ(-p4, q4);
}

TEST(operators, subtraction) {
  polynomial<int> p1{1};
  polynomial<int> q1{2};
  polynomial<int> r1{-1};
  EXPECT_EQ(p1 - q1, r1);
  polynomial<int> p2{1};
  polynomial<int> q2{-1};
  polynomial<int> r2{2};
  EXPECT_EQ(p2 - q2, r2);
}

TEST(operators, stream_output) {
  polynomial<int> p1{};
  std::stringstream s1("0");
  std::stringstream o1;
  o1 << p1;
  EXPECT_EQ(o1.str(), s1.str());
  polynomial<int> p2{1};
  std::stringstream s2("1");
  std::stringstream o2;
  o2 << p2;
  EXPECT_EQ(o2.str(), s2.str());
  polynomial<int> p3{-1};
  std::stringstream s3("-1");
  std::stringstream o3;
  o3 << p3;
  EXPECT_EQ(o3.str(), s3.str());
  polynomial<int> p4{0, 1};
  std::stringstream s4("x");
  std::stringstream o4;
  o4 << p4;
  EXPECT_EQ(o4.str(), s4.str());
  polynomial<int> p5{0, -1};
  std::stringstream s5("-x");
  std::stringstream o5;
  o5 << p5;
  EXPECT_EQ(o5.str(), s5.str());
  polynomial<int> p6{1, 1};
  std::stringstream s6("1+x");
  std::stringstream o6;
  o6 << p6;
  EXPECT_EQ(o6.str(), s6.str());
  polynomial<int> p7{0, 0, -2};
  std::stringstream s7("-2x^2");
  std::stringstream o7;
  o7 << p7;
  EXPECT_EQ(o7.str(), s7.str());
  polynomial<int> p8{0, 0, 2};
  std::stringstream s8("2x^2");
  std::stringstream o8;
  o8 << p8;
  EXPECT_EQ(o8.str(), s8.str());
  polynomial<int> p9{2, -1, -1};
  p9.set_var("y");
  std::stringstream s9("2-y-y^2");
  std::stringstream o9;
  o9 << p9;
  EXPECT_EQ(o9.str(), s9.str());
  polynomial<int> p10{-3, 2, 4};
  std::stringstream s10("-3+2x+4x^2");
  std::stringstream o10;
  o10 << p10;
  EXPECT_EQ(o10.str(), s10.str());
}

TEST(operators, multiplication) {
  polynomial<int> p1{1, 1};
  polynomial<int> q1{1, -1};
  polynomial<int> r1{1, 0, -1};
  EXPECT_EQ(p1 * q1, r1);
  polynomial<int> p2{1, 4, 2};
  polynomial<int> q2{};
  polynomial<int> r2{};
  EXPECT_EQ(p2 * q2, r2);
}

TEST(multivar, stream_output) {
  polynomial<polynomial<std::complex<int>>> p1 {{{1,2}, {2,3}}, {{-1,-2}, {-2,-3}}};
  p1.set_var({"z", "y"});
  std::stringstream s1("((1,2)+(2,3)y)+((-1,-2)+(-2,-3)y)z");
  std::stringstream o1;
  o1 << p1;
  EXPECT_EQ(o1.str(), s1.str());
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

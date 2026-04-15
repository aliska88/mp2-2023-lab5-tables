#include <gtest.h>
#include <string>
#include "arithmetic.h"
#include "monom.h"
#include "ordered_array_table.h"
#include "ordered_list_table.h"
#include "polynomial.h"
#include "rb_table.h"
#include "stack.h"
#include "unordered_table.h"


TEST(TStackTest, Can_Create_Stack_With_Positive_Length) {
  EXPECT_NO_THROW(TStack<int> s(5));
}

TEST(TStackTest, Throws_When_Create_Stack_With_Negative_Length) {
  EXPECT_THROW(TStack<int> s(-5), std::invalid_argument);
}

TEST(TStackTest, Can_Push_And_Pop_Element) {
  TStack<int> s(5);
  s.push(10);
  EXPECT_EQ(10, s.pop());
}

TEST(TStackTest, Is_Empty_Works_Correctly) {
  TStack<int> s(5);
  EXPECT_TRUE(s.is_empty());
  s.push(1);
  EXPECT_FALSE(s.is_empty());
}

TEST(TStackTest, Throws_When_Pop_From_Empty_Stack) {
  TStack<int> s(5);
  EXPECT_THROW(s.pop(), std::underflow_error);
}

TEST(TStackTest, Stack_Resizes_Automatically) {
  TStack<int> s(2);
  s.push(1);
  s.push(2);
  EXPECT_NO_THROW(s.push(3));
  EXPECT_EQ(3, s.get_count());
  EXPECT_EQ(3, s.pop());
}

TEST(MonomTest, Can_Create_Monom) {
  Monom m(2.5, 1, 2, 3);
  EXPECT_DOUBLE_EQ(2.5, m.getCoeff());
  EXPECT_EQ(1, m.getDegreeX());
  EXPECT_EQ(2, m.getDegreeY());
  EXPECT_EQ(3, m.getDegreeZ());
}

TEST(MonomTest, Can_Multiply_Monom_By_Scalar) {
  Monom m(2.0, 1, 1, 1);
  Monom res = m * 3.0;
  EXPECT_DOUBLE_EQ(6.0, res.getCoeff());
  EXPECT_EQ(m.getDegree(), res.getDegree());
}

TEST(MonomTest, Can_Multiply_Two_Monoms) {
  Monom m1(2.0, 1, 1, 1);
  Monom m2(3.0, 2, 0, 1);
  Monom res = m1 * m2;
  EXPECT_DOUBLE_EQ(6.0, res.getCoeff());
  EXPECT_EQ(3, res.getDegreeX());
  EXPECT_EQ(1, res.getDegreeY());
  EXPECT_EQ(2, res.getDegreeZ());
}

TEST(MonomTest, IsSimilar_Works_Correctly) {
  Monom m1(2.0, 1, 2, 3);
  Monom m2(5.0, 1, 2, 3);
  Monom m3(2.0, 1, 2, 4);
  EXPECT_TRUE(m1.isSimilar(m2));
  EXPECT_FALSE(m1.isSimilar(m3));
}

TEST(PolynomialTest, Can_Create_Empty_Polynomial) {
  Polynomial p;
  EXPECT_TRUE(p.isEmpty());
  EXPECT_EQ(0, p.size());
}

TEST(PolynomialTest, Can_Insert_Monom) {
  Polynomial p;
  p.insert(2.0, 1, 1, 1);
  EXPECT_FALSE(p.isEmpty());
  EXPECT_EQ(1, p.size());
}

TEST(PolynomialTest, Can_Add_Two_Polynomials) {
  Polynomial p1;
  p1.insert(2.0, 1, 0, 0);
  Polynomial p2;
  p2.insert(3.0, 1, 0, 0);

  Polynomial res = p1 + p2;
  Polynomial expected;
  expected.insert(5.0, 1, 0, 0);

  EXPECT_TRUE(res == expected);
}

TEST(UnorderedTableTest, Can_Insert_And_Find) {
  UnorderedTable table;
  Polynomial p;
  p.insert(1.0, 1, 0, 0);

  int ops = 0;
  table.insert("poly1", p, ops);

  Polynomial found_p;
  bool found = table.find("poly1", found_p, ops);

  EXPECT_TRUE(found);
  EXPECT_TRUE(found_p == p);
  EXPECT_EQ(1, table.size());
}

TEST(UnorderedTableTest, Can_Remove_Element) {
  UnorderedTable table;
  Polynomial p;
  int ops = 0;
  table.insert("poly1", p, ops);

  bool removed = table.remove("poly1", ops);
  EXPECT_TRUE(removed);
  EXPECT_EQ(0, table.size());
}

TEST(RBTableTest, Can_Insert_And_Find_In_RedBlack_Tree) {
  RBTable tree;
  Polynomial p;
  p.insert(5.0, 2, 2, 2);

  int ops = 0;
  tree.insert("test_key", p, ops);

  Polynomial found_p;
  bool found = tree.find("test_key", found_p, ops);

  EXPECT_TRUE(found);
  EXPECT_TRUE(found_p == p);
  EXPECT_EQ(1, tree.size());
}

TEST(PostfixTest, Can_Calculate_Simple_Expression) {
  Postfix postfix;
  postfix.set_infix("2+2*2");
  postfix.parse();
  postfix.to_postfix();
  EXPECT_DOUBLE_EQ(6.0, postfix.calculate());
}

TEST(PostfixTest, Can_Calculate_With_Parentheses) {
  Postfix postfix;
  postfix.set_infix("(2+2)*2");
  postfix.parse();
  postfix.to_postfix();
  EXPECT_DOUBLE_EQ(8.0, postfix.calculate());
}

TEST(PostfixTest, Can_Calculate_With_Variables) {
  Postfix postfix;
  postfix.set_infix("x+y*z");
  postfix.parse();
  postfix.to_postfix();
  EXPECT_DOUBLE_EQ(7.0, postfix.calculate(1.0, 2.0, 3.0));
}

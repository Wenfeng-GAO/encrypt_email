#include "gtest/include/gtest/gtest.h"
#include "../BigUnsigned.h"

class BigUnsignedTest : public ::testing::Test {

    protected:

        virtual void SetUp() {
            t = new BigUnsigned(2);
            x = new BigUnsigned(*t);
        }

        virtual void TearDown() {
            delete t;
            delete x;
        }

        BigUnsigned *t;
        BigUnsigned *x;
};


TEST_F(BigUnsignedTest, Intialiations) {
    /* Test zero constructor */
    BigUnsigned bu0;
    ASSERT_TRUE(bu0.isZero());

    /* Assignment */
    BigUnsigned bu1(1);
    BigUnsigned bu11 = bu1;
    ASSERT_FALSE(bu11.isZero());
    EXPECT_EQ(1, bu11.toInt());

    /* Test constructor that copies from a given array */
#if 0
    BigUnsigned::Blk *array = new BigUnsigned::Blk[1];
    array[0] = 1234567890;
    BigUnsigned *bua = new BigUnsigned(array, 1);
    ASSERT_TRUE(!bua->isZero());
    ASSERT_EQ(1234567890, bua->toLong());
#endif
    BigUnsigned::Blk array[] = {1234567890};
    BigUnsigned bua(array, 1);
    ASSERT_TRUE(!bua.isZero());
    ASSERT_EQ(1234567890, bua.toLong());

    ASSERT_TRUE(!t->isZero()); 
}

TEST_F(BigUnsignedTest, Converters) {
    ASSERT_EQ(2, t->toInt());
}

TEST_F(BigUnsignedTest, Comparisons) {
    ASSERT_EQ(0, x->compareTo(*t)) << x->compareTo(*t);
    BigUnsigned v1(1), v3(3);
    ASSERT_EQ(1, v3.compareTo(v1));
    ASSERT_EQ(1, v3.compareTo(*x));
    ASSERT_EQ(-1, v1.compareTo(v3));
    ASSERT_EQ(-1, v1.compareTo(*x));
    ASSERT_EQ(0, v1.compareTo(v1));
    ASSERT_EQ(0, v3.compareTo(v3));
}

TEST_F(BigUnsignedTest, CopyLessOperations) {
    /* Test addition */
    BigUnsigned v0(0), v1(1), v2(2), v(123);
    v0.add(v1, v2);
    ASSERT_EQ(3, v0.toInt());
    v.add(v1, v);
    ASSERT_EQ(124, v.toInt());
    v.add(v, v);
    ASSERT_EQ(248, v.toInt());

    /* Test subtraction */
    BigUnsigned b0(0), b1(1), b2(2);
    b1.subtract(b2, b0);
    ASSERT_EQ(2, b2.toInt());
}

TEST_F(BigUnsignedTest, Operators) {
    /* Assignment operator */
    BigUnsigned tt(123);
    ASSERT_TRUE(!tt.isZero());
    ASSERT_EQ(123, tt.toInt());
    *t = tt;
    ASSERT_EQ(0, t->compareTo(tt));
    ASSERT_TRUE(!t->isZero());
    ASSERT_EQ(123, t->toInt());

    /* Ordinary comparison operators */
    BigUnsigned v1(1), v2(2), v3(v1);
    EXPECT_TRUE(v1==v3);
    EXPECT_TRUE(v1<=v3);
    EXPECT_TRUE(v1>=v3);
    EXPECT_TRUE(v1<v2);
    EXPECT_TRUE(v1<=v2);
    EXPECT_TRUE(v2>v3);
    EXPECT_TRUE(v2>=v3);
    EXPECT_TRUE(v2!=v1);
}

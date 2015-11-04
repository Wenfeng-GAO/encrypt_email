#include "gtest/include/gtest/gtest.h"
#include "../BigUnsigned.h"

class BigUnsignedTest : public ::testing::Test {

    protected:
        BigUnsignedTest() {
            BigUnsigned t(2);
            BigUnsigned x(t);
        }
        BigUnsigned t;
        BigUnsigned x;
};


TEST_F(BigUnsignedTest, Intialiations) {
    ASSERT_TRUE(!t.isZero()) << "t = 2";
}

TEST_F(BigUnsignedTest, Converters) {
    ASSERT_EQ(2, x.toInt());
}

TEST_F(BigUnsignedTest, Comparisons) {
    ASSERT_EQ(1, x.compareTo(t));
}


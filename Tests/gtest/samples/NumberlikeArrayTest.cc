#include "NumberlikeArray.h"
#include "gtest/gtest.h"

class NumberlikeArrayTest : public ::testing::Test {
    protected:
        NumberlikeArrayTest() {
            NumberlikeArray<int> array0();
            NumberlikeArray<int> array1(1);
            NumberlikeArray<int> array2(2);
            NumberlikeArray<int> array00(0);
        }
};

TEST_F(NumberlikeArrayTest, IsNullInitially) {
    ASSERT_TRUE(array0.blk == NULL);
    ASSERT_TRUE(array00.blk == NULL);
}

TEST_F(NumberlikeArrayTest, BlkInitiallly) {
    ASSERT_TRUE(array1.blk != NULL);
    ASSERT_TRUE(array2.blk != NULL);

    EXPECT_EQ(array1.cap, 1);
    EXPECT_EQ(array1.len, 0);
    EXPECT_EQ(array2.cap, 2);
    EXPECT_EQ(array2.len, 0);
}

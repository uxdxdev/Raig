#include <iostream>
#include "gtest/gtest.h"

#include "base/Vector3.h"

TEST(TestCompare, ReturnOneIfEQ)
{
    base::Vector3 vecA(0, 1, 0);
    base::Vector3 vecB(0, 1, 0);
    int result = vecA.Compare(&vecB);
    EXPECT_EQ(result, 1);
}
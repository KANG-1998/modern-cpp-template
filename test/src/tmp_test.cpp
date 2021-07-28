#include "project/tmp.hpp"

#include <gtest/gtest.h>

TEST(TmpAddTest, CheckValues)
{
  ASSERT_EQ(tmp::add(1, 2), 3);
  EXPECT_TRUE(true);
}

TEST(TmpAddTest, CheckSub)
{
  ASSERT_EQ(tmp::sub(3, 1), 2);
  EXPECT_FALSE(true);
}

int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);

  std::cout << "balala" <<std::endl;
  return RUN_ALL_TESTS();
}

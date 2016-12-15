#include "../src/status.cpp"
#include <gtest/gtest.h>

TEST(StatusTest,TestAllStatusMethods){
  EXPECT_TRUE(new Status());
}

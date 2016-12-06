#include <gtest/gtest.h>
#include "../src/status.cpp"

TEST(StatusTest,TestAllStatusMethods){
  EXPECT_TRUE(new Status());
}

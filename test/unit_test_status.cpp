#include "../src/status.cpp"
#include <gtest/gtest.h>

TEST(StatusTest,TestAllStatusMethods){
  EXPECT_TRUE(new Status());
}

TEST(StatusTest,TestUsage){
  Status* status = new Status();
  status->usage("ml-status");
}
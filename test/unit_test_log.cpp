#include "../src/log.cpp"
#include <gtest/gtest.h>

TEST(LogTest,TestAllLogMethods){
  EXPECT_TRUE(new Log());
}

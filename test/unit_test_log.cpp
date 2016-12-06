#include <gtest/gtest.h>
#include "../src/log.cpp"

TEST(LogTest,TestAllLogMethods){
  EXPECT_TRUE(new Log());
}

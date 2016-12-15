#include "../src/log.cpp"
#include <gtest/gtest.h>

TEST(LogTest,TestAllLogMethods){
  EXPECT_TRUE(new Log());
}

TEST(LogTest,TestUsage){
  Log* log = new Log();
  log->usage("ml-log");
}
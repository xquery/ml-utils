#include "../src/log.cpp"
#include <gtest/gtest.h>
using namespace std;
using namespace mlutil;

TEST(LogTest,TestAllLogMethods){
  EXPECT_TRUE(new Log());
}

TEST(LogTest,TestUsage){
  Log* log = new Log();
  log->usage("ml-log");
}
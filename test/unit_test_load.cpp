#include "../src/load.cpp"
#include <gtest/gtest.h>

TEST(LoadTest,TestAllLoadMethods){
  EXPECT_TRUE(new Load());
}

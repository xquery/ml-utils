#include <gtest/gtest.h>
#include "../src/load.cpp"

TEST(LoadTest,TestAllLoadMethods){
  EXPECT_TRUE(new Load());
}

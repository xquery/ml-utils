#include "../src/load.cpp"
#include <gtest/gtest.h>
using namespace std;
using namespace mlutil;

TEST(LoadTest,TestAllLoadMethods){
  EXPECT_TRUE(new Load());
}

TEST(LoadTest,TestUsage){
  Load* load = new Load();
  load->usage("ml-load");
}
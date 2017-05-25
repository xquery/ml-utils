#include "../src/browse.cpp"

#include <gtest/gtest.h>

using namespace std;
using namespace mlutil;

TEST(BrowseTest,TestAllBrowseMethods){
  EXPECT_TRUE(new Browse());
}

TEST(BrowseTest,TestUsage){
  Browse* bs = new Browse();
  bs->usage("ml-browse");
}

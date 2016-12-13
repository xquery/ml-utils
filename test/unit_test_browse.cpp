#include <gtest/gtest.h>
#include "../src/browse.cpp"

TEST(BrowseTest,TestAllBrowseMethods){
  EXPECT_TRUE(new Browse());
}

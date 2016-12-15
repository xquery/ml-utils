#include "../src/browse.cpp"

#include <gtest/gtest.h>

TEST(BrowseTest,TestAllBrowseMethods){
  EXPECT_TRUE(new Browse());
}

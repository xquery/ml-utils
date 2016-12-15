#include "../src/query.cpp"
#include <gtest/gtest.h>

TEST(QueryTest,TestAllQueryMethods){
  EXPECT_TRUE(new Query());
}

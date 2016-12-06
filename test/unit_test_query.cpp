#include <gtest/gtest.h>
#include "../src/query.cpp"

TEST(QueryTest,TestAllQueryMethods){
  EXPECT_TRUE(new Query());
}

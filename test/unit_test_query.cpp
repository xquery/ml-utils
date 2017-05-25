#include "../src/query.cpp"
#include <gtest/gtest.h>

using namespace std;
using namespace mlutil;

TEST(QueryTest,TestAllQueryMethods){
  EXPECT_TRUE(new Query());
}

TEST(QueryTest,TestUsage){
  Query* query = new Query();
  query->usage("ml-xq");
}
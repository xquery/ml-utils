#include <gtest/gtest.h>
#include "../src/configure.cpp"

TEST(AdminTest,TestAllAdminMethods){
  EXPECT_TRUE(new Admin());
}

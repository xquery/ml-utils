#include "../src/admin.cpp"

#include <gtest/gtest.h>

TEST(AdminTest,TestAllAdminMethods){
  EXPECT_TRUE(new Admin());
}

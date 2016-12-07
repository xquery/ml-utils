#include <gtest/gtest.h>
#include "../src/admin.cpp"

TEST(AdminTest,TestAllAdminMethods){
  EXPECT_TRUE(new Admin());
}

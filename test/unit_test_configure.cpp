#include <gtest/gtest.h>
#include "../src/admin.cpp"


TEST(AdminTest,TestAllAdminMethods){
  EXPECT_TRUE(new Admin());
}

TEST(AdminTest,TestUsage){
  Admin* admin = new Admin();
  admin->usage("ml-config");
}

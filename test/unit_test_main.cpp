#include "unit_test_history.cpp"
#include "unit_test_load.cpp"
#include "unit_test_query.cpp"
#include "unit_test_status.cpp"
#include "unit_test_configure.cpp"
#include "unit_test_log.cpp"

#include <gtest/gtest.h>

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

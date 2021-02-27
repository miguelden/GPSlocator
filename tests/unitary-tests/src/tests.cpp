#include <gtest/gtest.h>
#include "gtest_mem_main.hpp"

/**
 * Function to initialize static memory usage to avoid leak detection
 * after first call of some functions
 */
void init_static_mem() {

}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    testing::UnitTest::GetInstance()->listeners().Append(new testing::MemoryLeakDetector());
    init_static_mem();
    return RUN_ALL_TESTS();
}

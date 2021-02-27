/**
 * @file App_test.cpp
 *
 * @author miguel garcia (miguelden@gmail.com)
 *
 * @brief
 *    Tests for GPSlocator main APP
 */

#include <gtest/gtest.h>
#include "app.h"

using namespace ::std;

/**
 * APP Step
 */
TEST(App, step_001)
{

    app_step();
    GTEST_SUCCEED();
}

/**
 * @file App_test.cpp
 *
 * @author Instituto Tecnologico de Informatica
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

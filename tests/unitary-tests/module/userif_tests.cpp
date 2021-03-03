/**
 * @file userif_test.cpp
 *
 * @author miguel garcia (miguelden@gmail.com)
 *
 * @brief
 *    Tests for User Interface component
 */

#include <gtest/gtest.h>
#include "userif.h"

using namespace ::std;
using namespace ::testing;

/**
 * Update On-Range-Target
 */
TEST(UserIf, test_set_target_reached_001)
{
    uint8_t values[] = {0, 1, 1, 0, 0};

    for(auto v : values) {
        userif_set_target_reached(v);
        ASSERT_EQ(userif_get_target_reached(),v);
    }
}

/**
 * Update GPS-Fix
 */
TEST(UserIf, test_set_gps_status_001)
{
    uint8_t values[] = {0, 1, 1, 0, 0};

    for(auto v : values) {
        userif_set_gps_status(v);
        ASSERT_EQ(userif_get_gps_status(),v);
    }
}

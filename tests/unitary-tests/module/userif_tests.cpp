/**
 * @file userif_test.cpp
 *
 * @author miguel garcia (miguelden@gmail.com)
 *
 * @brief
 *    Tests for User Interface component
 */

#include <gtest/gtest.h>
#include "UserIfMock.hpp"
#include "userif.h"

using namespace ::std;
using namespace ::testing;

bool targetOnRange_ = false;
uint32_t targetOnRangeUpdateCnt_ = 0;

uint8_t gpsFix_ = false;
uint32_t gpsFixUpdateCnt_ = 0;

/** External function to update On-Range-Target indicator */
void UserIfUpdateTargetIndicator(uint8_t on_range)
{
    targetOnRange_ = (on_range>0);
    targetOnRangeUpdateCnt_++;
}

/** External function to update Active-GPS-Fix */
void UserIfUpdateGPSFix(uint8_t active)
{
    gpsFix_ = (active>0);
    gpsFixUpdateCnt_++;
}

/**
 * Update On-Range-Target
 */
TEST(UserIf, test_set_target_reached_001)
{
    UserIfMock::InitCallbacks(UserIfUpdateTargetIndicator, UserIfUpdateGPSFix);
    auto cnt = targetOnRangeUpdateCnt_;
    uint8_t values[] = {0, 1, 1, 0, 0};

    for(auto v : values) {
        userif_set_target_reached(v);
        ASSERT_TRUE((v>0)? targetOnRange_ : !targetOnRange_);
        ASSERT_EQ(targetOnRangeUpdateCnt_, ++cnt);
    }
}

/**
 * Update GPS-Fix
 */
TEST(UserIf, test_set_gps_status_001)
{
    UserIfMock::InitCallbacks(UserIfUpdateTargetIndicator, UserIfUpdateGPSFix);
    auto cnt = gpsFix_;
    uint8_t values[] = {0, 1, 1, 0, 0};

    for(auto v : values) {
        userif_set_gps_status(v);
        ASSERT_TRUE((v>0)? gpsFix_ : !gpsFix_);
        ASSERT_EQ(gpsFixUpdateCnt_, ++cnt);
    }
}

/**
 * @file target_tests.cpp
 *
 * @author miguel garcia (miguelden@gmail.com)
 *
 * @brief
 *    Tests for Target component
 */

#include <gtest/gtest.h>
#include "target.h"

using namespace ::std;

/**
 * Get target position
 */
TEST(Target, get_postion_001)
{
    // The constant target position must be (39.4731325, -0.3677324)*10^7
    auto targetPos = target_get_position();

    ASSERT_EQ(targetPos.latitude, 394731325L);
    ASSERT_EQ(targetPos.longitude, -3677324L);
}

/**
 * Get target range
 */
TEST(Target, get_range_001)
{
    // The constant target position must be (39.4731325, -0.3677324)*10^7
    auto targetRange = target_get_range();

    ASSERT_EQ(targetRange, 100UL);
}

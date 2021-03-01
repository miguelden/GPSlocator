/**
 * @file UserIfMock.cpp
 *
 * Mock for emulating User Interface (LEDs)
 *
 * @author miguelgarcia <miguelden@gmail.es>
 *
 * @license
 * All rights reserved. Read LICENSE.txt file for the license terms.
 *
 * Changelog:
 *
 * [01/03/2021]     [miguelgarcia]
 * Initial version
 *
 */

#include "UserIfMock.hpp"

using namespace ::testing;
using namespace ::std;

/** Callback function to update On-Range-Target indicator */
void (*UserIfMock::TargetIndicatorCallback)(uint8_t) = nullptr;

/** Callback function to update Active-GPS-Fix */
void (*UserIfMock::GpsFixCallback)(uint8_t) = nullptr;

extern "C" {

/** External function to update On-Range-Target indicator */
void update_target_indicator(uint8_t on_range)
{
    if (UserIfMock::TargetIndicatorCallback) {
        UserIfMock::TargetIndicatorCallback(on_range);
    }
}

/** External function to update Active-GPS-Fix */
void update_gps_fix(uint8_t active)
{
    if (UserIfMock::GpsFixCallback) {
        UserIfMock::GpsFixCallback(active);
    }
}

}

void UserIfMock::InitCallbacks (
        void (*targetCb)(uint8_t),
        void (*gpsCb)(uint8_t)
)
{
    TargetIndicatorCallback = targetCb;
    GpsFixCallback = gpsCb;
}


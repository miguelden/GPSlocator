/**
 * @file App_test.cpp
 *
 * @author miguel garcia (miguelden@gmail.com)
 *
 * @brief
 *    Tests for GPSlocator main APP
 */

#include <gtest/gtest.h>
#include "NmeaUtils.hpp"
#include "UserIfMock.hpp"
#include "position.h"
#include "navigation.h"
#include "app.h"

using namespace ::std;
using namespace ::testing;

bool app_targetOnRange_ = false;
uint32_t app_targetOnRangeUpdateCnt_ = 0;

uint8_t app_gpsFix_ = false;
uint32_t app_gpsFixUpdateCnt_ = 0;

/** External function to update On-Range-Target indicator */
void AppUpdateTargetIndicator(uint8_t on_range)
{
    app_targetOnRange_ = (on_range>0);
    app_targetOnRangeUpdateCnt_++;
}

/** External function to update Active-GPS-Fix */
void AppUpdateGPSFix(uint8_t active)
{
    app_gpsFix_ = (active>0);
    app_gpsFixUpdateCnt_++;
}

void UpdateApp (
        GgaType gga1
)
{
    string nmea1 = NmeaUtils::GenNMEA_GGAsentence(gga1);
    for (uint16_t i=0; i<nmea1.length(); i++) {
        app_step(nmea1.c_str()[i]);
    }
}

void CheckNavigation (
        GgaType gga1
)
{
    auto p = navigation_get_llh();
    if(gga1.fix) {
        ASSERT_LE(NmeaUtils::GetSqError(p.latitude,gga1.latitude), 0.1f);
        ASSERT_LE(NmeaUtils::GetSqError(p.longitude,gga1.longitude), 0.1f);
        ASSERT_LE(NmeaUtils::GetSqError(p.altitude,gga1.altitude), 0.1f);
        ASSERT_TRUE(p.is_valid != pos_invalid);
    } else {
        ASSERT_TRUE(p.is_valid == pos_invalid);
    }
}



/**
 * APP Step
 */
TEST(App, step_001)
{
    UserIfMock::InitCallbacks(AppUpdateTargetIndicator, AppUpdateGPSFix);
    app_init();

    GgaType gga1 = {.hours=1, .minutes=2, .seconds=3, .milliseconds=4,
            .latitude=39.47314319954006f, .longitude=-0.36773293176583255f, .nsIndicator='N', .ewIndicator='E',
            .fix=1, .satellites=12, .hdop=1.0, .altitude=13.0, .geoidal=0.0};

    GgaType gga2 = {.hours=1, .minutes=2, .seconds=3, .milliseconds=4,
            .latitude=39.47314319954006f, .longitude=-0.36773293176583255f, .nsIndicator='N', .ewIndicator='E',
            .fix=0, .satellites=12, .hdop=1.0, .altitude=13.0, .geoidal=0.0};

    bool success = false;

    UpdateApp(gga1);
    CheckNavigation(gga1);
    ASSERT_TRUE(app_gpsFix_);

    UpdateApp(gga2);
    CheckNavigation(gga2);
    ASSERT_FALSE(app_gpsFix_);
}

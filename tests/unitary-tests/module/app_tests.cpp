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


GgaType BuildGGA(float lat, float lon, float alt, int fix, int nsat) {

    GgaType gga = {.hours=1, .minutes=2, .seconds=3, .milliseconds=4,
            .latitude=lat, .longitude=lon, .nsIndicator='N', .ewIndicator='E',
            .fix=fix, .satellites=nsat, .hdop=1.0, .altitude=13.0, .geoidal=0.0};

    return gga;
}



auto p0 = BuildGGA(39.4731325f, -0.3677324f, 8.0f, 1, 10);

GgaType on_range_pos[] = {BuildGGA(39.47267848962713f, -0.36772682880576657f, 8.0f, 1, 10),
                          BuildGGA(39.4722752640897f, -0.3677248171490452f, 8.0f, 1, 10),
                          BuildGGA(39.47358055856425f, -0.3677326706376487f, 8.0f, 1, 10),
                          BuildGGA(39.47398227229816f, -0.36771610581500536f, 4.0f, 1, 10),
                          BuildGGA(39.47312442596326f, -0.36715412433585626f, 8.0f, 1, 10),
                          BuildGGA(39.47311564175071f, -0.3666363503051331f, 10.0f, 1, 10),
                          BuildGGA(39.4731343810911f, -0.36831925541892374f, 8.0f, 1, 10),
                          BuildGGA(39.473132640050245f, -0.36884252212343827f, 9.0f, 1, 10)};

GgaType out_of_range_pos[] = {BuildGGA(39.47218209226129f, -0.3677228054922441f, 10.0f, 1, 10),
                              BuildGGA(39.47178145196614f, -0.3677160999688299f, 12.0f, 1, 10),
                              BuildGGA(39.474072844045814f, -0.367716105815197f, 4.0f, 1, 10),
                              BuildGGA(39.474477120466744f, -0.36771077181243966f, 4.0f, 1, 10),
                              BuildGGA(39.473114909732615f, -0.36650453603622735f, 4.0f, 1, 10),
                              BuildGGA(39.47311200677346f, -0.3659854642201853f, 4.0f, 1, 10),
                              BuildGGA(39.473125675886905f, -0.3689643169524081f, 7.0f, 1, 10),
                              BuildGGA(39.47314482733609f, -0.36949209458329585f, 7.0f, 1, 10)};

GgaType no_fix_pos[] = {BuildGGA(39.4731325f, -0.3677324f, 8.0f, 0, 1),
                        BuildGGA(39.47267848962713f, -0.36772682880576657f, 8.0f, 0, 3),
                        BuildGGA(39.47358055856425f, -0.3677326706376487f, 8.0f, 0, 2),
                        BuildGGA(39.473114909732615f, -0.36650453603622735f, 4.0f, 0, 0),
                        BuildGGA(39.473125675886905f, -0.3689643169524081f, 7.0f, 0, 1)};

/**
 * APP Step
 */
TEST(App, step_002)
{
    UserIfMock::InitCallbacks(AppUpdateTargetIndicator, AppUpdateGPSFix);
    app_init();

    UpdateApp(p0);
    CheckNavigation(p0);
    ASSERT_TRUE(app_gpsFix_);
}

/**
 * APP Step. On range positions
 */
TEST(App, step_003)
{
    UserIfMock::InitCallbacks(AppUpdateTargetIndicator, AppUpdateGPSFix);
    app_init();

    uint32_t onrange_updated_cnt = app_targetOnRangeUpdateCnt_;
    uint32_t fix_updated_cnt = app_gpsFixUpdateCnt_;

    for(auto p : on_range_pos) {
        UpdateApp(p);
        CheckNavigation(p);
        ASSERT_TRUE(app_gpsFix_);
        ASSERT_TRUE(app_targetOnRange_);
        ASSERT_EQ(app_targetOnRangeUpdateCnt_, ++onrange_updated_cnt);
        ASSERT_EQ(app_gpsFixUpdateCnt_, ++fix_updated_cnt);
    }
}

/**
 * APP Step. Out of range positions
 */
TEST(App, step_004)
{
    UserIfMock::InitCallbacks(AppUpdateTargetIndicator, AppUpdateGPSFix);
    app_init();

    uint32_t onrange_updated_cnt = app_targetOnRangeUpdateCnt_;
    uint32_t fix_updated_cnt = app_gpsFixUpdateCnt_;

    for(auto p : out_of_range_pos) {
        UpdateApp(p);
        CheckNavigation(p);
        ASSERT_TRUE(app_gpsFix_);
        ASSERT_FALSE(app_targetOnRange_);
        ASSERT_EQ(app_targetOnRangeUpdateCnt_, ++onrange_updated_cnt);
        ASSERT_EQ(app_gpsFixUpdateCnt_, ++fix_updated_cnt);
    }
}

/**
 * APP Step. No GPS fix
 */
TEST(App, step_005)
{
    UserIfMock::InitCallbacks(AppUpdateTargetIndicator, AppUpdateGPSFix);
    app_init();

    uint32_t onrange_updated_cnt = app_targetOnRangeUpdateCnt_;
    uint32_t fix_updated_cnt = app_gpsFixUpdateCnt_;

    for(auto p : no_fix_pos) {
        UpdateApp(p);
        CheckNavigation(p);
        ASSERT_FALSE(app_gpsFix_);
        ASSERT_FALSE(app_targetOnRange_);
        ASSERT_EQ(app_targetOnRangeUpdateCnt_, ++onrange_updated_cnt);
        ASSERT_EQ(app_gpsFixUpdateCnt_, ++fix_updated_cnt);
    }
}


/**
 * APP Step. Alternate positions types
 */
TEST(App, step_006)
{
    UserIfMock::InitCallbacks(AppUpdateTargetIndicator, AppUpdateGPSFix);
    app_init();

    uint32_t onrange_updated_cnt = app_targetOnRangeUpdateCnt_;
    uint32_t fix_updated_cnt = app_gpsFixUpdateCnt_;

    // Target position
    UpdateApp(p0);
    CheckNavigation(p0);
    ASSERT_TRUE(app_gpsFix_);
    ASSERT_TRUE(app_targetOnRange_);
    ASSERT_EQ(app_targetOnRangeUpdateCnt_, ++onrange_updated_cnt);
    ASSERT_EQ(app_gpsFixUpdateCnt_, ++fix_updated_cnt);

    // No fix position
    UpdateApp(no_fix_pos[0]);
    CheckNavigation(no_fix_pos[0]);
    ASSERT_FALSE(app_gpsFix_);
    ASSERT_FALSE(app_targetOnRange_);
    ASSERT_EQ(app_targetOnRangeUpdateCnt_, ++onrange_updated_cnt);
    ASSERT_EQ(app_gpsFixUpdateCnt_, ++fix_updated_cnt);

    // Out of range position
    UpdateApp(out_of_range_pos[2]);
    CheckNavigation(out_of_range_pos[2]);
    ASSERT_TRUE(app_gpsFix_);
    ASSERT_FALSE(app_targetOnRange_);
    ASSERT_EQ(app_targetOnRangeUpdateCnt_, ++onrange_updated_cnt);
    ASSERT_EQ(app_gpsFixUpdateCnt_, ++fix_updated_cnt);

    // On range position
    UpdateApp(on_range_pos[3]);
    CheckNavigation(on_range_pos[3]);
    ASSERT_TRUE(app_gpsFix_);
    ASSERT_TRUE(app_targetOnRange_);
    ASSERT_EQ(app_targetOnRangeUpdateCnt_, ++onrange_updated_cnt);
    ASSERT_EQ(app_gpsFixUpdateCnt_, ++fix_updated_cnt);

    // No fix position
    UpdateApp(no_fix_pos[2]);
    CheckNavigation(no_fix_pos[2]);
    ASSERT_FALSE(app_gpsFix_);
    ASSERT_FALSE(app_targetOnRange_);
    ASSERT_EQ(app_targetOnRangeUpdateCnt_, ++onrange_updated_cnt);
    ASSERT_EQ(app_gpsFixUpdateCnt_, ++fix_updated_cnt);

    // On range position
    UpdateApp(on_range_pos[0]);
    CheckNavigation(on_range_pos[0]);
    ASSERT_TRUE(app_gpsFix_);
    ASSERT_TRUE(app_targetOnRange_);
    ASSERT_EQ(app_targetOnRangeUpdateCnt_, ++onrange_updated_cnt);
    ASSERT_EQ(app_gpsFixUpdateCnt_, ++fix_updated_cnt);
}

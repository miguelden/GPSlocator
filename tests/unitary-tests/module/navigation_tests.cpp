/**
 * @file navigation_tests.cpp
 *
 * @author miguel garcia (miguelden@gmail.com)
 *
 * @brief
 *    Tests for Navigation component
 */

#include <vector>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include <gtest/gtest.h>
#include "NmeaUtils.hpp"
#include "position.h"
#include "navigation.h"

using namespace ::std;
using namespace ::testing;

const std::string DATADIR = DATADIR666; // CMakeLists.txt:add_definitions( -DDATADIR666="${DATADIR}" )



/**
 * This reads a NMEA file, pushes each character to the navigation module and gets the resultant positions, that are
 * compared with the positions of a CSV files.
 * Generate a GTEST error in case of differences in one of the positions or in the number of positions.
 * @param fileName   The file names must be equals, but with different extension
 */
void CompareNMEAandCSVfiles (
        string fileName
)
{
    auto pos0 = NmeaUtils::ReadCSVfile(DATADIR + "/nmea/" + fileName +".csv");
    auto nmea = NmeaUtils::ReadNMEAfile(DATADIR + "/nmea/" + fileName + ".nmea");

    uint16_t pos_idx = 0;

    for (auto sentence : nmea) {
        for (uint16_t i=0; i<sentence.length(); i++) {
            auto res = navigation_add_nmea_char(sentence.c_str()[i]);
            if (res!=0) {
                if (pos_idx<pos0.size()) {
                    auto p = navigation_get_llh();
                    ASSERT_LE(NmeaUtils::GetSqError(p.latitude,pos0[pos_idx].latitude), 0.1f);
                    ASSERT_LE(NmeaUtils::GetSqError(p.longitude,pos0[pos_idx].longitude), 0.1f);
                    ASSERT_LE(NmeaUtils::GetSqError(p.altitude,pos0[pos_idx].altitude), 0.1f);
                    pos_idx++;
                } else {
                    ASSERT_LT(pos_idx, pos0.size());
                }
            }
        }
    }

    ASSERT_EQ(pos_idx, pos0.size());
}

/**
 * This generates a GGA sentence, pushes each character to the navigation module and gets the resultant position, that
 * is compared with the position of the input GGA struct.
 * Generate a GTEST error in case of differences in one of the parameters of the position.
 * @param gga1  GGA struct with the parameters to generate the GGA sentence
 */
void CompareNMEAwithGGA (
        GgaType gga1
)
{
    bool success = false;

    string nmea1 = NmeaUtils::GenNMEA_GGAsentence(gga1);

    for (uint16_t i=0; i<nmea1.length(); i++) {
        auto res = navigation_add_nmea_char(nmea1.c_str()[i]);
        if (res!=0) {
            auto p = navigation_get_llh();
            if(gga1.fix) {
                ASSERT_LE(NmeaUtils::GetSqError(p.latitude,gga1.latitude), 0.1f);
                ASSERT_LE(NmeaUtils::GetSqError(p.longitude,gga1.longitude), 0.1f);
                ASSERT_LE(NmeaUtils::GetSqError(p.altitude,gga1.altitude), 0.1f);
                ASSERT_TRUE(p.is_valid != pos_invalid);
            } else {
                ASSERT_TRUE(p.is_valid == pos_invalid);
            }

            success = true;
        }
    }

    ASSERT_TRUE(success);
}


/**
 * Read NMEA file and compare the results with the positions of a CSV file
 */
TEST(Navigation, test_nmea_file_001)
{
    navigation_reset();
    CompareNMEAandCSVfiles("input_001");
}

/**
 * Read NMEA file
 * Invalid CRC in $GPGGA sentences
 */
TEST(Navigation, test_nmea_file_002)
{
    navigation_reset();
    auto nmea = NmeaUtils::ReadNMEAfile(DATADIR + "/nmea/input_err_crc.nmea");

    uint16_t pos_idx = 0;

    for (auto sentence : nmea) {
        for (uint16_t i=0; i<sentence.length(); i++) {
            auto res = navigation_add_nmea_char(sentence.c_str()[i]);
            if (res!=0) {
                GTEST_FAIL();
            }
        }
    }

    GTEST_SUCCEED();
}


/**
 * Read NMEA GGA sentence
 */
TEST(Navigation, test_nmea_gga_001)
{
    navigation_reset();
    GgaType gga1 = {.hours=1, .minutes=2, .seconds=3, .milliseconds=4,
                    .latitude=39.47314319954006f, .longitude=-0.36773293176583255f, .nsIndicator='N', .ewIndicator='E',
                    .fix=1, .satellites=12, .hdop=1.0, .altitude=13.0, .geoidal=0.0};
    CompareNMEAwithGGA(gga1);
}

/**
 * Read NMEA GGA sentence without fix
 */
TEST(Navigation, test_nmea_gga_002)
{
    navigation_reset();
    GgaType gga1 = {.hours=1, .minutes=2, .seconds=3, .milliseconds=4,
            .latitude=39.47314319954006f, .longitude=-0.36773293176583255f, .nsIndicator='N', .ewIndicator='E',
            .fix=0, .satellites=2, .hdop=1.0, .altitude=13.0, .geoidal=0.0};
    CompareNMEAwithGGA(gga1);
}

/**
 * Read NMEA GGA sentence without nsIndicator and ewIndicator
 */
TEST(Navigation, test_nmea_gga_003)
{
    bool success = false;

    navigation_reset();
    GgaType gga1 = {.hours=1, .minutes=2, .seconds=3, .milliseconds=4,
            .latitude=39.47314319954006f, .longitude=-0.36773293176583255f, .nsIndicator=' ', .ewIndicator=' ',
            .fix=1, .satellites=12, .hdop=1.0, .altitude=13.0, .geoidal=0.0};
    string nmea1 = NmeaUtils::GenNMEA_GGAsentence(gga1);

    for (uint16_t i=0; i<nmea1.length(); i++) {
        auto res = navigation_add_nmea_char(nmea1.c_str()[i]);
        if (res!=0) {
            auto p = navigation_get_llh();
            ASSERT_TRUE(p.is_valid == pos_invalid);
            success = true;
        }
    }

    ASSERT_TRUE(success);
}

/**
 * Read NMEA GGA with low number of satellites
 */
TEST(Navigation, test_nmea_gga_004)
{
    navigation_reset();

    GgaType gga1 = {.hours=1, .minutes=2, .seconds=3, .milliseconds=4,
            .latitude=39.47314319954006f, .longitude=-0.36773293176583255f, .nsIndicator='N', .ewIndicator='E',
            .fix=1, .satellites=2, .hdop=1.0, .altitude=13.0, .geoidal=0.0};
    CompareNMEAwithGGA(gga1);

    GgaType gga2 = {.hours=1, .minutes=2, .seconds=3, .milliseconds=4,
            .latitude=39.47314319954006f, .longitude=-0.36773293176583255f, .nsIndicator='N', .ewIndicator='E',
            .fix=1, .satellites=4, .hdop=1.0, .altitude=13.0, .geoidal=0.0};
    CompareNMEAwithGGA(gga2);
}

/**
 * Read NMEA GGA sentence with length shorter than 3
 */
TEST(Navigation, test_nmea_gga_005)
{
    navigation_reset();
    char nmea[255];
    auto n = snprintf(nmea,255,"$\r");

    for (uint16_t i=0; i<n; i++) {
        auto res = navigation_add_nmea_char(nmea[i]);
        if (res!=0) {
            GTEST_FAIL();
        }
    }

    GTEST_SUCCEED();
}

/**
 * Read NMEA GGA sentence with incorrect format -> data[len-3] != '*'
 */
TEST(Navigation, test_nmea_gga_006)
{
    navigation_reset();
    char nmea[255];
    auto n = snprintf(nmea,255,"$GPGGA,0.0\r");

    for (uint16_t i=0; i<n; i++) {
        auto res = navigation_add_nmea_char(nmea[i]);
        if (res!=0) {
            GTEST_FAIL();
        }
    }

    GTEST_SUCCEED();
}


/**
 * Read NMEA GGA sentence with length longer than 255 (MTK3339_BUF_SZ)
 */
TEST(Navigation, test_nmea_gga_007)
{
    navigation_reset();
    char nmea[300];
    memset(nmea, 0, 300);
    nmea[0]='$';
    nmea[299]='\r';

    for (uint16_t i=0; i<300; i++) {
        auto res = navigation_add_nmea_char(nmea[i]);
        if (res!=0) {
            GTEST_FAIL();
        }
    }

    GTEST_SUCCEED();
}


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
#include "position.h"
#include "navigation.h"

using namespace ::std;

const std::string DATADIR = DATADIR666; // CMakeLists.txt:add_definitions( -DDATADIR666="${DATADIR}" )

/**
 * Read CSV file with the list of positions
 * @param path   CSV file path
 * @return  The list of positions
 */
vector<position_st> read_csv_file (
        string path
)
{
    ifstream classFile(path);
    vector<position_st> pos;
    string line;

    while (getline(classFile, line)) {
        std::istringstream s(line);
        std::string field;

        position_st p0;
        getline(s, field,',');
        p0.latitude = strtof(field.c_str(),NULL);
        getline(s, field,',');
        p0.longitude = strtof(field.c_str(),NULL);
        getline(s, field,',');
        p0.altitude = strtof(field.c_str(),NULL);
        pos.push_back(p0);
    }

    return pos;
}

/**
 * Read NMEA file with the list of sentences
 * @param path   NMEA file path
 * @return  The list of NMEA sentences
 */
vector<string> read_nmea_file (
        string path
)
{
    ifstream classFile(path);
    vector<string> nmea;
    string line;

    while (getline(classFile, line)) {

        line += "\r";
        nmea.push_back(line);
    }

    return nmea;
}

/**
 * Get the square difference between expected and obtained values
 * @param x0  Value 0
 * @param x1  Value 1
 * @return Square error
 */
static float sq_error (
        float x0,
        float x1
)
{
    float d = x1 - x0;
    return (d * d);
}


string gen_nmea_gga (
        GgaType gga
)
{
    char hhmmssss[10];
    snprintf(hhmmssss, 10, "%02d%02d%02d.%02d", gga.hours, gga.minutes, gga.seconds, gga.milliseconds);

    float lat_abs = ((gga.latitude<0)?-1.0f:1.0f) * gga.latitude;
    float lat_deg = lat_abs;
    float lat_min = (lat_abs - floorf(lat_deg)) * 60.0f;
    float lat_sec = round((lat_min - floorf(lat_min)) * 1000.0f);
    char lat_pole_prime = (gga.latitude < 0)? ((gga.nsIndicator == 'N')?'S':'N') : (gga.nsIndicator);
    char lat_format[9];
    snprintf(lat_format, 9, "%02d%02d.%03d", (int)lat_deg, (int)lat_min, (int)lat_sec);

    float lng_abs = ((gga.longitude<0)?-1.0f:1.0f) * gga.longitude;
    float lng_deg = lng_abs;
    float lng_min = (lng_abs - floorf(lng_deg)) * 60.0f;
    float lng_sec = round((lng_min - floorf(lng_min)) * 1000.0f);
    char lng_pole_prime = (gga.longitude < 0)? ((gga.ewIndicator == 'E')?'W':'E') : (gga.ewIndicator);
    char lng_format[10];
    snprintf(lng_format, 10, "%03d%02d.%03d", (int)lng_deg, (int)lng_min, (int)lng_sec);

    char str[255];
    auto n = snprintf(str, 255, "GPGGA,%s,%s,%c,%s,%c,%d,%02d,%.1f,%.1f,M,%.1f,M,,",
            hhmmssss, lat_format, lat_pole_prime, lng_format, lng_pole_prime,
            gga.fix, gga.satellites, gga.hdop, gga.altitude, gga.geoidal);

    int crc = 0;
    for (int i=0; i<n; i++) {
        crc = crc ^ str[i];
    }
    crc = crc & 0xFF;

    char res[255];
    snprintf(res, 255, "$%s*%02X\r", str, crc);
    return string(res);
}


/**
 * Read NMEA and CSV files and compare the positions
 * @param file_name  File name without extension
 */
void compare_nmea_and_csv_files(string file_name)
{
    auto pos0 = read_csv_file(DATADIR + "/nmea/" + file_name +".csv");
    auto nmea = read_nmea_file(DATADIR + "/nmea/" + file_name + ".nmea");

    uint16_t pos_idx = 0;

    for (auto sentence : nmea) {
        for (uint16_t i=0; i<sentence.length(); i++) {
            auto res = navigation_add_nmea_char(sentence.c_str()[i]);
            if (res!=0) {
                if (pos_idx<pos0.size()) {
                    auto p = navigation_get_llh();
                    ASSERT_LE(sq_error(p.latitude,pos0[pos_idx].latitude), 0.1f);
                    ASSERT_LE(sq_error(p.longitude,pos0[pos_idx].longitude), 0.1f);
                    ASSERT_LE(sq_error(p.altitude,pos0[pos_idx].altitude), 0.1f);
                    pos_idx++;
                } else {
                    ASSERT_LT(pos_idx, pos0.size());
                }
            }
        }
    }

    ASSERT_EQ(pos_idx, pos0.size());
}

void compare_nmea_with_gga (
        GgaType gga1
)
{
    bool success = false;

    string nmea1 = gen_nmea_gga(gga1);

    for (uint16_t i=0; i<nmea1.length(); i++) {
        auto res = navigation_add_nmea_char(nmea1.c_str()[i]);
        if (res!=0) {
            auto p = navigation_get_llh();
            if(gga1.fix) {
                ASSERT_LE(sq_error(p.latitude,gga1.latitude), 0.1f);
                ASSERT_LE(sq_error(p.longitude,gga1.longitude), 0.1f);
                ASSERT_LE(sq_error(p.altitude,gga1.altitude), 0.1f);
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
 * Read NMEA file
 */
TEST(Navigation, test_nmea_001)
{
    navigation_reset();
    compare_nmea_and_csv_files("input_001");
}

/**
 * Read NMEA GGA sentence
 */
TEST(Navigation, test_nmea_002)
{
    navigation_reset();
    GgaType gga1 = {.hours=1, .minutes=2, .seconds=3, .milliseconds=4,
                    .latitude=39.47314319954006f, .longitude=-0.36773293176583255f, .nsIndicator='N', .ewIndicator='E',
                    .fix=1, .satellites=12, .hdop=1.0, .altitude=13.0, .geoidal=0.0};
    compare_nmea_with_gga(gga1);
}

/**
 * Read NMEA GGA sentence without fix
 */
TEST(Navigation, test_nmea_003)
{
    navigation_reset();
    GgaType gga1 = {.hours=1, .minutes=2, .seconds=3, .milliseconds=4,
            .latitude=39.47314319954006f, .longitude=-0.36773293176583255f, .nsIndicator='N', .ewIndicator='E',
            .fix=0, .satellites=2, .hdop=1.0, .altitude=13.0, .geoidal=0.0};
    compare_nmea_with_gga(gga1);
}

/**
 * Read NMEA GGA sentence without nsIndicator and ewIndicator
 */
TEST(Navigation, test_nmea_004)
{
    bool success = false;

    navigation_reset();
    GgaType gga1 = {.hours=1, .minutes=2, .seconds=3, .milliseconds=4,
            .latitude=39.47314319954006f, .longitude=-0.36773293176583255f, .nsIndicator=' ', .ewIndicator=' ',
            .fix=1, .satellites=12, .hdop=1.0, .altitude=13.0, .geoidal=0.0};
    string nmea1 = gen_nmea_gga(gga1);

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
TEST(Navigation, test_nmea_005)
{
    navigation_reset();

    GgaType gga1 = {.hours=1, .minutes=2, .seconds=3, .milliseconds=4,
            .latitude=39.47314319954006f, .longitude=-0.36773293176583255f, .nsIndicator='N', .ewIndicator='E',
            .fix=1, .satellites=2, .hdop=1.0, .altitude=13.0, .geoidal=0.0};
    compare_nmea_with_gga(gga1);

    GgaType gga2 = {.hours=1, .minutes=2, .seconds=3, .milliseconds=4,
            .latitude=39.47314319954006f, .longitude=-0.36773293176583255f, .nsIndicator='N', .ewIndicator='E',
            .fix=1, .satellites=4, .hdop=1.0, .altitude=13.0, .geoidal=0.0};
    compare_nmea_with_gga(gga2);
}

/**
 * Read NMEA GGA sentence with length shorter than 3
 */
TEST(Navigation, test_nmea_006)
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
TEST(Navigation, test_nmea_007)
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
TEST(Navigation, test_nmea_008)
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

/**
 * Read NMEA file
 * Invalid CRC in $GPGGA sentences
 */
TEST(Navigation, test_err_nmea_crc_001)
{
    navigation_reset();
    auto nmea = read_nmea_file(DATADIR + "/nmea/input_err_crc.nmea");

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

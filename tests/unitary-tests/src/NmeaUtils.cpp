/**
 * @file NmeaUtils.cpp
 *
 * NMEA Utils
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

#include <fstream>
#include <cstdlib>
#include <cmath>
#include <gtest/gtest.h>
#include "NmeaUtils.hpp"

using namespace ::testing;
using namespace ::std;

/**
 * Read CSV file with the list of positions
 * @param path   CSV file path
 * @return  The list of positions
 */
std::vector<position_st> NmeaUtils::ReadCSVfile (
        std::string path
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
vector<string> NmeaUtils::ReadNMEAfile (
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
 * Generate a NMEA sentence of type GGA with the information of the input.
 * The string contains the final '\r', necessary for parsing the message.
 * @param gga  GGA parameters
 * @return  String with the GGA sentence
 */
string NmeaUtils::GenNMEA_GGAsentence (
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
 * Get the square difference between expected and obtained values
 * @param x0  Value 0
 * @param x1  Value 1
 * @return Square error
 */
float NmeaUtils::GetSqError (
        float x0,
        float x1
)
{
    float d = x1 - x0;
    return (d * d);
}


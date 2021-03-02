/**
 * @file navigation.c
 *
 * An interface to the MTK3339 GPS module
 *
 * This file is based on the Embedded Artists implementation:
 *
 * https://www.embeddedartists.com/products/gps-receiver-board/
 * https://os.mbed.com/components/Embedded-Artists-GPS-Receiver-Board/
 *
 * @license
 * All rights reserved. Read LICENSE.txt file for the license terms.
 *
 * Changelog:
 *
 * [28/02/2021]     [miguelgarcia]
 * Initial version
 *
 */

/* -- Includes -- */
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "position.h"
#include "navigation.h"

/* -- Definitions -- */

/** Max size of MTK3339 NMEA sentence */
#define MTK3339_BUF_SZ  255

/* -- Local types -- */

/** State of the data parser
 *     StateStart - Reading the start character '$'
 *     StateData - Reading the NMEA sentence body
 */
typedef enum DataState_ {
    StateStart = 0,
    StateData
} DataState;

/* -- Local variables -- */

/** Time, position and fix related data */
static GgaType gga;

/** Data Buffer */
static char buf_[MTK3339_BUF_SZ];
/** Buffer position */
static int buf_pos_ = 0;
/** Parser state */
static DataState parser_state_ = StateStart;

/** LLH : Latitude, Longitude in Decimal Degrees, Altitude in meters */
static position_st current_llh_;

/* -- Local functions -- */
static uint8_t parseData(char* data, int len);
static void parseGGA(char* data, int dataLen);
static void update_current_llh (void);

/**
 * @brief Reset the last read data from the GPS module.
 */
void navigation_reset (

)
{
    memset(&gga, 0, sizeof(GgaType));
}


/**
 * @brief Get latitude in decimal degrees if GPS fix is valid
 * @return Latitude in decimal degrees or zero if position is not valid
 */
void update_current_llh (

)
{
    /* Latitude conversion */
    if(gga.fix == 0 || gga.nsIndicator == 0) {
        current_llh_.latitude = 0;
    } else {
        float l = gga.latitude;
        char ns = gga.nsIndicator;

        /* convert from ddmm.mmmm to degrees only 60 minutes is 1 degree */
        int deg = (int)(l / 100);
        l = (l - (float)deg*100.0f) / 60.0f;
        l = (float)deg + l;
        if (ns == 'S') {
            l = -l;
        }

        current_llh_.latitude = l;
    }

    /* Longitude conversion */
    if(gga.fix == 0 || gga.ewIndicator == 0) {
        current_llh_.longitude = 0;
    } else {
        float l = gga.longitude;
        char ew = gga.ewIndicator;

        /* convert from ddmm.mmmm to degrees only 60 minutes is 1 degree */
        int deg = (int)(l / 100);
        l = (l - (float)deg*100.0f) / 60.0f;
        l = (float)deg + l;
        if (ew == 'W') {
            l = -l;
        }

        current_llh_.longitude = l;
    }

    /* MSL Altitude conversion */
    if (gga.fix == 0) {
        current_llh_.altitude = 0;
    } else {
        current_llh_.altitude = gga.altitude;
    }

    /* Valid Data */
    if (gga.fix == 0 || gga.nsIndicator == 0 || gga.ewIndicator == 0) {
        current_llh_.is_valid = pos_invalid;
    } else if (gga.satellites <= 4) {
        current_llh_.is_valid = pos_2d;
    } else {
        current_llh_.is_valid = pos_3d;
    }
}

/**
 * @brief Get current LLH value. Latitude and Longitude in decimal degrees and MSL Altitude in meters
 * @return Current LLH position
 */
position_st navigation_get_llh (

)
{
    return current_llh_;
}

/**
 * Parse a NMEA GGA Sentence
 * @param data     Input buffer data
 * @param dataLen  Buffer length
 */
void parseGGA (
        char* data,
        int dataLen
)
{
    /* http://aprs.gids.nl/nmea/#gga */

    float tm = 0;

    memset(&gga, 0, sizeof(GgaType));

    char* p = data;
    int pos = 0;

    p = strchr(p, ',');
    while (p != NULL && *p != 0) {
        p++;

        switch(pos) {
        case 0: /* time: hhmmss.sss */
            tm = strtof(p, NULL);
            gga.hours = (int)(tm / 10000);
            gga.minutes = ((int)tm % 10000) / 100;
            gga.seconds = ((int)tm % 100);
            gga.milliseconds = (int)(tm * 1000) % 1000;
            break;
        case 1: /* latitude: ddmm.mmmm */
            gga.latitude = strtof(p, NULL);
            break;
        case 2: /* N/S indicator (north or south) */
            if (*p == 'N' || *p == 'S') {
                gga.nsIndicator = *p;
            }
            break;
        case 3: /* longitude: dddmm.mmmm */
            gga.longitude = strtof(p, NULL);
            break;
        case 4: /* E/W indicator (east or west) */
            if (*p == 'E' || *p == 'W') {
                gga.ewIndicator = *p;
            }
            break;
        case 5: /* position indicator (1=no fix, 2=GPS fix, 3=Differential) */
            gga.fix = (int)strtol(p, NULL, 10);
            break;
        case 6: /* num satellites */
            gga.satellites = (int)strtol(p, NULL, 10);
            break;
        case 7: /* hdop */
            gga.hdop = strtof(p, NULL);
            break;
        case 8: /* altitude */
            gga.altitude = strtof(p, NULL);
            break;
        case 9: /* units */
            /* ignore units */
            break;
        case 10: /* geoidal separation */
            gga.geoidal = strtof(p, NULL);
            break;
        default:
            /* ignore */
            break;
        }
        pos++;

        p = strchr(p, ',');
    }

}


/**
 * Parse a Generic NMEA Sentence
 * @param [in] data     Input buffer data
 * @param [in] dataLen  Buffer length
 * @return Positive value if Navigation has a new position value, Otherwise Zero.
 */
uint8_t parseData (
        char* data,
        int len
)
{
    uint8_t res = 0;

    // verify checksum
    if (len < 3 || (len > 3 && data[len-3] != '*')) {
        // invalid data
        return 0;
    }
    int sum = strtol(&data[len-2], NULL, 16);
    for(int i = 1; i < len-3; i++) {
        sum ^= data[i];
    }

    if (sum != 0) {
        // invalid checksum
        return 0;
    }

    if (strncmp("$GPGGA", data, 6) == 0) {
        parseGGA(data, len);
        update_current_llh();
        res = 1;
    }

    return res;
}

/**
 * Add new NMEA char to the NMEA parser
 * @param [in] d  Input char
 * @return Positive value if Navigation has a new position value, Otherwise Zero.
 */
uint8_t navigation_add_nmea_char (
        char d
)
{
    uint8_t res = 0;

    if(parser_state_ == StateStart) {
        if (d == '$') {
            buf_[0] = '$';
            buf_pos_ = 1;
            parser_state_ = StateData;
        }

    } else {
        if (buf_pos_ >= MTK3339_BUF_SZ) {
            // error
            parser_state_ = StateStart;

        } else if (d == '\r') {
            buf_[buf_pos_] = 0;
            res = parseData(buf_, buf_pos_);
            parser_state_ = StateStart;

        } else {
            buf_[buf_pos_++] = d;
        }
    }

    return res;
}

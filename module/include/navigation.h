/**
 * @file navigation.h
 *
 * An interface to the MTK3339 GPS module
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

#ifndef INCLUDE_NAVIGATION_H_
#define INCLUDE_NAVIGATION_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "position.h"

/**
 * An interface to the MTK3339 GPS module.
 */

typedef struct {

    /** UTC time - hours */
    int hours;
    /** UTC time - minutes */
    int minutes;
    /** UTC time - seconds */
    int seconds;
    /** UTC time - milliseconds */
    int milliseconds;

    /** The latitude in ddmm.mmmm format (d = degrees, m = minutes) */
    float latitude;
    /** The longitude in dddmm.mmmm format */
    float longitude;
    /** North / South indicator */
    char nsIndicator;
    /** East / West indicator */
    char ewIndicator;

    /**
     * Position indicator:
     * 0 = Fix not available
     * 1 = GPS fix
     * 2 = Differential GPS fix
     */
    int fix;

    /** Number of used satellites */
    int satellites;
    /** Horizontal Dilution of Precision */
    float hdop;
    /** antenna altitude above/below mean sea-level */
    float altitude;
    /** geoidal separation */
    float geoidal;

} GgaType;


/**
 * @brief Reset the last read data from the GPS module.
 */
void navigation_reset(void);


/**
 * @brief Get current LLH position. Latitude and Longitude in decimal degrees and MSL Altitude in meters.
 * @return Current LLH position
 */
position_st navigation_get_llh(void);


/**
 * Add new NMEA char to the NMEA parser
 * @param [in] d  Input char
 * @return Positive value if Navigation has a new position value, Otherwise Zero.
 */
uint8_t navigation_add_nmea_char(char d);

#ifdef __cplusplus
}
#endif

#endif /* INCLUDE_NAVIGATION_H_ */

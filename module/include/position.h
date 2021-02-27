/**
 * @file position.h
 *
 * Position types and structures
 *
 * @author miguelgarcia <miguelden@gmail.es>
 *
 * @license
 * All rights reserved. Read LICENSE.txt file for the license terms.
 *
 * Changelog:
 *
 * [27/02/2021]     [miguelgarcia]
 * Initial version
 *
 */

#ifndef INCLUDE_POSITION_H_
#define INCLUDE_POSITION_H_

#ifdef __cplusplus
extern "C" {
#endif

/** Fixed point position in latitude and longitude value with degrees stored in units of 10^-7 of a degree */
typedef struct {

    int32_t latitude;
    int32_t longitude;

} position_st;



/**
 * @brief Main step of the GPSlocator
 */
void app_step(void);

#ifdef __cplusplus
}
#endif

#endif /* INCLUDE_POSITION_H_ */

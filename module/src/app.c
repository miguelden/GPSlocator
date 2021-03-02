/**
 * @file app.c
 *
 * GPSlocator main application
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

/* -- Includes -- */
#include <stdint.h>
#include "navigation.h"
#include "position.h"
#include "target.h"
#include "userif.h"
#include "app.h"


/**
* @brief Initialize the state of the GPSlocator
*/
void app_init (

)
{
    /* Initialize the Navigation component */
    navigation_reset();
}

/**
 * @brief Main step of the GPSlocator
 * @param [in] d  Input char from GPS device
 */
void app_step (
        char d
)
{
    /* Target */
    position_st target_llh;
    /* Device */
    position_st llh;
    float enu[3];

    /* Update the navigation component */
    if ( navigation_add_nmea_char(d) ) {

        uint8_t on_range = 0;

        /* New GGA data is available */
        llh = navigation_get_llh();
        target_llh = target_get_position();

        /* LLH is valid if GPS fix is active */
        if (llh.is_valid) {
            float dist;

            /* Compute local ENU position with target as origin */
            position_geodetic_to_enu (
                    llh.latitude, llh.longitude, llh.altitude,
                    target_llh.latitude, target_llh.longitude, target_llh.altitude,
                    &enu[0], &enu[1], &enu[2]
            );

            /* Get distance to target */
            dist = position_xyz_distance(0.0f,0.0f,0.0f,enu[0],enu[1],enu[2]);
            on_range = (dist <= target_get_range())? 1 : 0;
        }

        /* Update user interface */
        userif_set_gps_status(llh.is_valid);
        userif_set_target_reached(on_range);

    }
}
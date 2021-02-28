/**
 * @file target.c
 *
 * Target configuration
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
#include "target.h"

/* -- Local variables -- */

/** Target position (Lat=39.4731325, Lon=-0.3677324, Alt=15.0) */
static const position_st target_pos = {.latitude = 39.4731325f, .longitude = -0.3677324f, .altitude = 15.0f,
                                       .is_valid = pos_3d};

/** Range distance to target in meters */
static const float target_range = 100.0f;

/**
 * @brief Return the target position
 * @return Target position
 */
position_st target_get_position (

)
{
    return target_pos;
}

/**
 * @brief Return the target range in meters
 * @return Target range
 */
uint32_t target_get_range (

)
{
    return target_range;
}

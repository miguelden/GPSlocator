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

/** Target position (39.4731325, -0.3677324) * 10^7 [10^-7 * deg] */
static const position_st target_pos = {.latitude = 394731325L, .longitude = -3677324L};

/** Range distance to target in meters */
static const uint32_t target_range = 100UL;

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

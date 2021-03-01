/**
 * @file userif.c
 *
 * User interface monitor
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


/* -- Includes -- */
#include "userif.h"

/* -- External functions -- */

/** External function to update On-Range-Target indicator */
extern void update_target_indicator(uint8_t on_range);

/** External function to update Active-GPS-Fix */
extern void update_gps_fix(uint8_t active);


/**
 * @brief Update the USER INTERFACE with the status of the Target. When the target is reached, the argument must be
 * positive. If the device is out of range of the target, the argument must be zero.
 *
 * @param [in] reached  Value of the reached argument. Zero is out-of-range, positive is on-range.
 */
void userif_set_target_reached (
        uint8_t reached
)
{
    update_target_indicator(reached==0? 0U : 1U);
}

/**
 * @brief Update the USER INTERFACE with the status of the GPS. If the position is not valid (FIX==0), or it is
 * valid (FIX>0)
 *
 * @param [in] fix  GPS FIX status
 */
void userif_set_gps_status (
        uint8_t fix
)
{
    update_gps_fix(fix==0? 0U : 1U);
}

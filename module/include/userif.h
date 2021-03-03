/**
 * @file userif.h
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

#ifndef INCLUDE_USERIF_H_
#define INCLUDE_USERIF_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "position.h"

/**
 * @brief Update the USER INTERFACE with the status of the Target. When the target is reached, the argument must be
 * positive. If the device is out of range of the target, the argument must be zero.
 *
 * @param [in] reached  Value of the reached argument. Zero is out-of-range, positive is in-range.
 */
void userif_set_target_reached(uint8_t reached);

/**
 * @brief Return the USER INTERFACE with the status of the Target. When the target is reached, the argument must be
 * positive. If the device is out of range of the target, the argument must be zero.
 *
 * @return Zero if the target is out-of-range, positive if it is on-range.
 */
uint8_t userif_get_target_reached(void);

/**
 * @brief Update the USER INTERFACE with the status of the GPS. If the position is not valid (FIX==0), or it is
 * valid (FIX>0)
 *
 * @param [in] fix  GPS FIX status
 */
void userif_set_gps_status(uint8_t fix);

/**
 * @brief Return the USER INTERFACE with the status of the GPS. If the position is not valid (FIX==0), or it is
 * valid (FIX>0)
 *
 * @return  GPS FIX status
 */
uint8_t userif_get_gps_status(void);


#ifdef __cplusplus
}
#endif

#endif /* INCLUDE_USERIF_H_ */

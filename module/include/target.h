/**
 * @file target.h
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

#ifndef INCLUDE_TARGET_H_
#define INCLUDE_TARGET_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "position.h"

/**
 * @brief Return the target position
 * @return Target position
 */
position_st target_get_position(void);

/**
 * @brief Return the target range in meters
 * @return Target range
 */
uint32_t target_get_range(void);

#ifdef __cplusplus
}
#endif

#endif /* INCLUDE_TARGET_H_ */

/**
 * @file app.h
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

#ifndef INCLUDE_APP_H_
#define INCLUDE_APP_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
* @brief Initialize the state of the GPSlocator
*/
void app_init(void);

/**
 * @brief Main step of the GPSlocator
 * @param [in] d  Input char from GPS device
 */
void app_step(char d);

#ifdef __cplusplus
}
#endif

#endif /* INCLUDE_DISPATCHER_H_ */

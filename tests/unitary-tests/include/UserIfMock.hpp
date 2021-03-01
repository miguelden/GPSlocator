/**
 * @file UserIfMock.hpp
 *
 * Mock for emulating User Interface (LEDs)
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

#include <vector>
#include <string>
#include "position.h"
#include "navigation.h"

namespace testing {

class UserIfMock {

public:

    static void InitCallbacks( void (*targetCb)(uint8_t), void (*gpsCb)(uint8_t) );

    /** Callback function to update On-Range-Target indicator */
    static void (*TargetIndicatorCallback)(uint8_t);

    /** Callback function to update Active-GPS-Fix */
    static void (*GpsFixCallback)(uint8_t);

    /** Disabled initializers */
    UserIfMock() = delete;
    ~UserIfMock() = delete;
    UserIfMock(const UserIfMock& orig) = delete;
    UserIfMock& operator=(const UserIfMock& orig) = delete;

};

};
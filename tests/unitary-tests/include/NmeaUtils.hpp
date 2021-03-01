/**
 * @file NmeaUtils.hpp
 *
 * NMEA Utils
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

class NmeaUtils {

public:

    /**
     * Read CSV file with the list of positions
     * @param path   CSV file path
     * @return  The list of positions
     */
    static std::vector<position_st> ReadCSVfile(std::string path);

    /**
     * Read NMEA file with the list of sentences
     * @param path   NMEA file path
     * @return  The list of NMEA sentences
     */
    static std::vector<std::string> ReadNMEAfile(std::string path);

    /**
     * Generate a NMEA sentence of type GGA with the information of the input.
     * The string contains the final '\r', necessary for parsing the message.
     * @param gga  GGA parameters
     * @return  String with the GGA sentence
     */
    static std::string GenNMEA_GGAsentence(GgaType gga);

    /**
     * Get the square difference between expected and obtained values
     * @param x0  Value 0
     * @param x1  Value 1
     * @return Square error
     */
    static float GetSqError(float x0, float x1);


    /** Disabled initializers */
    NmeaUtils() = delete;
    ~NmeaUtils() = delete;
    NmeaUtils(const NmeaUtils& orig) = delete;
    NmeaUtils& operator=(const NmeaUtils& orig) = delete;

};

};
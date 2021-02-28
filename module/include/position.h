/**
 * @file position.h
 *
 * Position converters and distance calculator
 *
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

typedef enum {

    pos_invalid = 0,
    pos_2d = 1,
    pos_3d = 2,

} position_status;

/** Geodetic Position. Latitude and Longitude in decimal degrees, MSL Altitude in meters */
typedef struct {

    float latitude;
    float longitude;
    float altitude;

    position_status is_valid;

} position_st;


/**
 * @brief Converts WGS-84 Geodetic point (lat, lon, h) to the Earth-Centered Earth-Fixed (ECEF) coordinates (x, y, z).
 * @param [in]  lat  Latitude in decimal degrees
 * @param [in]  lon  Longitude in decimal degrees
 * @param [in]  h    Geodesic Altitude
 * @param [out] x    ECEF X
 * @param [out] y    ECEF Y
 * @param [out] z    ECEF Z
 */
void position_geodetic_to_ecef(float lat, float lon, float h,
                               float* x, float* y, float* z);

/**
 * @brief Converts the Earth-Centered Earth-Fixed (ECEF) coordinates (x, y, z) to East-North-Up coordinates in a Local
 * Tangent Plane that is centered at the (WGS-84) Geodetic point (lat0, lon0, h0).
 *
 * @param [in]  x       ECEF X
 * @param [in]  y       ECEF Y
 * @param [in]  z       ECEF Z
 * @param [in]  lat0    Geodetic initial latitude
 * @param [in]  lon0    Geodetic initial longitude
 * @param [out] h0      Geodetic initial altitude
 * @param [out] xEast   ENU East coordinate
 * @param [out] yNorth  ENU North coordinate
 * @param [out] zUp     ENU UP coordinate
 */
void position_ecef_to_enu(float x, float y, float z,
                          float lat0, float lon0, float h0,
                          float* xEast, float* yNorth, float* zUp);


/**
 * @brief Converts the geodetic WGS-84 coordinated (lat, lon, h) to East-North-Up coordinates in a Local Tangent Plane
 * that is centered at the (WGS-84) Geodetic point (lat0, lon0, h0).
 *
 * @param [in]  lat     Latitude in decimal degrees
 * @param [in]  lon     Longitude in decimal degrees
 * @param [in]  h       Geodetic Altitude
 * @param [in]  lat0    Geodetic initial latitude
 * @param [in]  lon0    Geodetic initial longitude
 * @param [in]  h0      Geodetic initial altitude
 * @param [out] xEast   ENU East coordinate
 * @param [out] yNorth  ENU North coordinate
 * @param [out] zUp     ENU UP coordinate
 */
void position_geodetic_to_enu(float lat, float lon, float h,
                              float lat0, float lon0, float h0,
                              float* xEast, float* yNorth, float* zUp);

/**
 * @brief  Calculate the distance between two 3D-points
 * @param [in] x0   X0 coordinate in meters
 * @param [in] y0   Y0 coordinate in meters
 * @param [in] z0   Z0 coordinate in meters
 * @param [in] x1   X1 coordinate in meters
 * @param [in] y1   Y1 coordinate in meters
 * @param [in] z1   Z1 coordinate in meters
 * @return  The distance between the two coordinates in meters
 */
float position_xyz_distance(float x0, float y0, float z0,
                            float x1, float y1, float z1);

#ifdef __cplusplus
}
#endif

#endif /* INCLUDE_POSITION_H_ */

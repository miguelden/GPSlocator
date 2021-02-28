/**
 * @file distance.c
 *
 * Position converters and distance calculator
 *
 * Some helpers for converting GPS readings from the WGS84 geodetic system to a local North-East-Up cartesian axis.
 *
 * This implementation is based on the repository of Govert van Drimmelen:
 *   https://gist.github.com/govert/1b373696c9a27ff4c72a
 *
 * The implementation here is according to the paper:
 *   "Conversion of Geodetic coordinates to the Local Tangent Plane" Version 2.01.
 *   "The basic reference for this paper is J.Farrell & M.Barth 'The Global Positioning System & Inertial Navigation'"
 * Also helpful is Wikipedia: http://en.wikipedia.org/wiki/Geodetic_datum
 * Also helpful are the guidance notes here: http://www.epsg.org/Guidancenotes.aspx
 *
 * @license
 * All rights reserved. Read LICENSE.txt file for the license terms.
 *
 * Changelog:
 *
 * [27/02/2021]     [miguelgarcia]
 * Version based on Govert implementation
 *
 */

/* -- Includes -- */
#include <math.h>
#include "position.h"


/* --- WGS-84 geodetic constants --- */

/* WGS-84 Earth semimajor axis (m) */
#define EARTH_SEMIMAJOR_AXIS   6378137.0f
/* Derived Earth semiminor axis (m) */
#define EARTH_SEMIMINOR_AXIS   6356752.314245f
/* Ellipsoid Flatness */
#define ELLIPSOID_FLATNESS     ((EARTH_SEMIMAJOR_AXIS - EARTH_SEMIMINOR_AXIS) / EARTH_SEMIMAJOR_AXIS)
/* Square of Eccentricity */
#define ECCENTRICITY_SQ        (ELLIPSOID_FLATNESS * (2.0f - ELLIPSOID_FLATNESS))


/* -- Local functions -- */
static float degrees_to_radians(float degrees);


/**
 * @brief Converts WGS-84 Geodetic point (lat, lon, h) to the Earth-Centered Earth-Fixed (ECEF) coordinates (x, y, z).
 * @param [in]  lat  Latitude in decimal degrees
 * @param [in]  lon  Longitude in decimal degrees
 * @param [in]  h    Geodesic Altitude
 * @param [out] x    ECEF X
 * @param [out] y    ECEF Y
 * @param [out] z    ECEF Z
 */
void position_geodetic_to_ecef (
        float lat,
        float lon,
        float h,
        float* x,
        float* y,
        float* z
)
{
    /* Convert to radians in notation consistent with the paper */
    float lambda = degrees_to_radians(lat);
    float phi = degrees_to_radians(lon);
    float s = sinf(lambda);
    float N = EARTH_SEMIMAJOR_AXIS / sqrtf(1 - ECCENTRICITY_SQ * s * s);

    float sin_lambda = sinf(lambda);
    float cos_lambda = cosf(lambda);
    float cos_phi = cosf(phi);
    float sin_phi = sinf(phi);

    *x = (h + N) * cos_lambda * cos_phi;
    *y = (h + N) * cos_lambda * sin_phi;
    *z = (h + (1 - ECCENTRICITY_SQ) * N) * sin_lambda;
}

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
void position_ecef_to_enu (
        float x,
        float y,
        float z,
        float lat0,
        float lon0,
        float h0,
        float* xEast,
        float* yNorth,
        float* zUp
)
{
    /* Convert to radians in notation consistent with the paper */
    float lambda = degrees_to_radians(lat0);
    float phi = degrees_to_radians(lon0);
    float s = sinf(lambda);
    float N = EARTH_SEMIMAJOR_AXIS / sqrtf(1 - ECCENTRICITY_SQ * s * s);

    float sin_lambda = sinf(lambda);
    float cos_lambda = cosf(lambda);
    float cos_phi = cosf(phi);
    float sin_phi = sinf(phi);

    float x0 = (h0 + N) * cos_lambda * cos_phi;
    float y0 = (h0 + N) * cos_lambda * sin_phi;
    float z0 = (h0 + (1 - ECCENTRICITY_SQ) * N) * sin_lambda;

    float xd, yd, zd;
    xd = x - x0;
    yd = y - y0;
    zd = z - z0;

    /* This is the matrix multiplication */
    *xEast = -sin_phi * xd + cos_phi * yd;
    *yNorth = -cos_phi * sin_lambda * xd - sin_lambda * sin_phi * yd + cos_lambda * zd;
    *zUp = cos_lambda * cos_phi * xd + cos_lambda * sin_phi * yd + sin_lambda * zd;
}


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
void position_geodetic_to_enu (
        float lat,
        float lon,
        float h,
        float lat0,
        float lon0,
        float h0,
        float* xEast,
        float* yNorth,
        float* zUp
)
{
    float x, y, z;
    position_geodetic_to_ecef(lat, lon, h, &x, &y, &z);
    position_ecef_to_enu(x, y, z, lat0, lon0, h0, xEast, yNorth, zUp);
}


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
float position_xyz_distance (
        float x0,
        float y0,
        float z0,
        float x1,
        float y1,
        float z1
)
{
    float dx = (x1-x0);
    float dy = (y1-y0);
    float dz = (z1-z0);
    return sqrtf( (dx*dx) + (dy*dy) + (dz*dz) );
}


/**
 * @brief  Decimal degress to radians converter
 * @param degrees  Decimar degrees
 * @return  Equivalent in radians
 */
static float degrees_to_radians (
        float degrees
)
{
    static const float PI = 3.14159265358979323846f;
    return (PI / 180.0f) * degrees;
}

/**
 * @file distance_tests.cpp
 *
 * @author miguel garcia (miguelden@gmail.com)
 *
 * @brief
 *    Tests for Position component
 */

#include <iostream>
#include <gtest/gtest.h>
#include "position.h"

using namespace ::std;

/** MAX square error */
static const float max_sq_error = 0.3f;

/**
 * Get the square difference between expected and obtained values
 * @param x0  Value 0
 * @param x1  Value 1
 * @return Square error
 */
static float sq_error (float x0, float x1)
{
    float d = x1 - x0;
    return (d * d);
}

/**
 * Geodetic to ECEF
 */
TEST(Position, test_geodetic_to_ecef_001)
{
    float llh0[3] = {34.00000048f, -117.3335693f, 251.702f};
    float xyz0[3];

    position_geodetic_to_ecef(llh0[0], llh0[1], llh0[2], &xyz0[0], &xyz0[1], &xyz0[2]);

    ASSERT_LE(sq_error(-2430601.8f, xyz0[0]), max_sq_error);
    ASSERT_LE(sq_error(-4702442.7f, xyz0[1]), max_sq_error);
    ASSERT_LE(sq_error(3546587.4f, xyz0[2]), max_sq_error);
}

/**
 * ECEF to ENU
 */
TEST(Position, test_ecef_to_enu_001)
{
    float llh0[3] = {34.00000048f, -117.3335693f, 251.702f};
    float xyz0[3];
    position_geodetic_to_ecef(llh0[0], llh0[1], llh0[2], &xyz0[0], &xyz0[1], &xyz0[2]);

    // Checks to read out the matrix entries, to compare to the paper
    float xyz[3];

    // First column
    xyz[0] = xyz0[0]+1.0f;
    xyz[1] = xyz0[1];
    xyz[2] = xyz0[2];

    float enu[3];
    position_ecef_to_enu(xyz[0], xyz[1], xyz[2], llh0[0], llh0[1], llh0[2], &enu[0], &enu[1], &enu[2]);

    float exp_enu[3] = {0.88834836f, 0.25676467f, -0.38066927f};

    ASSERT_LE(sq_error(exp_enu[0], enu[0]), max_sq_error);
    ASSERT_LE(sq_error(exp_enu[1], enu[1]), max_sq_error);
    ASSERT_LE(sq_error(exp_enu[2], enu[2]), max_sq_error);
}

/**
 * ECEF to ENU
 */
TEST(Position, test_ecef_to_enu_002)
{
    float llh0[3] = {34.00000048f, -117.3335693f, 251.702f};
    float xyz0[3];
    position_geodetic_to_ecef(llh0[0], llh0[1], llh0[2], &xyz0[0], &xyz0[1], &xyz0[2]);

    // Checks to read out the matrix entries, to compare to the paper
    float xyz[3];

    // First column
    xyz[0] = xyz0[0];
    xyz[1] = xyz0[1]+1.0f;
    xyz[2] = xyz0[2];

    float enu[3];
    position_ecef_to_enu(xyz[0], xyz[1], xyz[2], llh0[0], llh0[1], llh0[2], &enu[0], &enu[1], &enu[2]);

    float exp_enu[3] = {-0.45917011f, 0.49675810f, -0.73647416f};

    ASSERT_LE(sq_error(exp_enu[0], enu[0]), max_sq_error);
    ASSERT_LE(sq_error(exp_enu[1], enu[1]), max_sq_error);
    ASSERT_LE(sq_error(exp_enu[2], enu[2]), max_sq_error);
}

/**
 * ECEF to ENU
 */
TEST(Position, test_ecef_to_enu_003)
{
    float llh0[3] = {34.00000048f, -117.3335693f, 251.702f};
    float xyz0[3];
    position_geodetic_to_ecef(llh0[0], llh0[1], llh0[2], &xyz0[0], &xyz0[1], &xyz0[2]);

    // Checks to read out the matrix entries, to compare to the paper
    float xyz[3];

    // First column
    xyz[0] = xyz0[0];
    xyz[1] = xyz0[1];
    xyz[2] = xyz0[2]+1.0f;

    float enu[3];
    position_ecef_to_enu(xyz[0], xyz[1], xyz[2], llh0[0], llh0[1], llh0[2], &enu[0], &enu[1], &enu[2]);

    float exp_enu[3] = {0.00000000f, 0.82903757f, 0.55919291f};

    ASSERT_LE(sq_error(exp_enu[0], enu[0]), max_sq_error);
    ASSERT_LE(sq_error(exp_enu[1], enu[1]), max_sq_error);
    ASSERT_LE(sq_error(exp_enu[2], enu[2]), max_sq_error);
}


/**
 * ECEF to ENU
 */
TEST(Position, test_ecef_to_enu_004)
{
    float llh0[3] = {34.00000048f, -117.3335693f, 251.702f};
    float xyz0[3];
    position_geodetic_to_ecef(llh0[0], llh0[1], llh0[2], &xyz0[0], &xyz0[1], &xyz0[2]);

    // Checks to read out the matrix entries, to compare to the paper
    float xyz[3];

    // First column
    xyz[0] = xyz0[0];
    xyz[1] = xyz0[1];
    xyz[2] = xyz0[2];

    float enu[3];
    position_ecef_to_enu(xyz[0], xyz[1], xyz[2], llh0[0], llh0[1], llh0[2], &enu[0], &enu[1], &enu[2]);

    float exp_enu[3] = {0.0f, 0.0f, 0.0f};

    ASSERT_LE(sq_error(exp_enu[0], enu[0]), max_sq_error);
    ASSERT_LE(sq_error(exp_enu[1], enu[1]), max_sq_error);
    ASSERT_LE(sq_error(exp_enu[2], enu[2]), max_sq_error);
}

/**
 * Position
 */
TEST(Position, test_xyz_distance_001)
{
    float xyz0[3] = {0.0f, 0.0f, 0.0f};
    float xyz1[3] = {0.0f, 0.0f, 0.0f};
    auto dist = position_xyz_distance(xyz0[0], xyz0[1], xyz0[2], xyz1[0], xyz1[1], xyz1[2]);

    ASSERT_LE(sq_error(0.0f, dist), 0.01f);
}

/**
 * Position
 */
TEST(Position, test_xyz_distance_002)
{
    float exp_dist = 32.0f;
    float xyz0[3] = {1234.5f, 789.0f, 32.1f};
    float xyz1[3] = {xyz0[0]+exp_dist, xyz0[1], xyz0[2]};
    auto dist = position_xyz_distance(xyz0[0], xyz0[1], xyz0[2], xyz1[0], xyz1[1], xyz1[2]);

    ASSERT_LE(sq_error(exp_dist, dist), 0.01f);
}

/**
 * Position
 */
TEST(Position, test_xyz_distance_003)
{
    float exp_dist = 132.0f;
    float xyz0[3] = {1234.5f, 789.0f, 32.1f};
    float xyz1[3] = {xyz0[0], xyz0[1]+exp_dist, xyz0[2]};
    auto dist = position_xyz_distance(xyz0[0], xyz0[1], xyz0[2], xyz1[0], xyz1[1], xyz1[2]);

    ASSERT_LE(sq_error(exp_dist, dist), 0.01f);
}

/**
 * Position
 */
TEST(Position, test_xyz_distance_004)
{
    float exp_dist = 13.0f;
    float xyz0[3] = {1234.5f, 789.0f, 32.1f};
    float xyz1[3] = {xyz0[0], xyz0[1], xyz0[2]+exp_dist};
    auto dist = position_xyz_distance(xyz0[0], xyz0[1], xyz0[2], xyz1[0], xyz1[1], xyz1[2]);

    ASSERT_LE(sq_error(exp_dist, dist), 0.01f);
}

/**
 * Position
 */
TEST(Position, test_xyz_distance_005)
{
    float xyz0[3] = {123.4f, 567.8f, 9.0f};
    float xyz1[3] = {234.8f, 987.2f, 5.2f};
    float exp_dist = 433.959399023f;
    auto dist = position_xyz_distance(xyz0[0], xyz0[1], xyz0[2], xyz1[0], xyz1[1], xyz1[2]);

    ASSERT_LE(sq_error(exp_dist, dist), 0.01f);
}

/**
 * Position
 */
TEST(Position, test_xyz_distance_006)
{
    float xyz0[3] = {-123.4f, -567.8f, -9.0f};
    float xyz1[3] = {-234.8f, -987.2f, -5.2f};
    float exp_dist = 433.959399023f;
    auto dist = position_xyz_distance(xyz0[0], xyz0[1], xyz0[2], xyz1[0], xyz1[1], xyz1[2]);

    ASSERT_LE(sq_error(exp_dist, dist), 0.01f);
}

/**
 * Geodetic to ENU and Position
 */
TEST(Position, test_geodetic_to_enu_001)
{
    float llh0[3] = {38.84492440897005f, 0.0938887156236068f, 3.0f};
    float llh1[3] = {38.8441326433408f, 0.09445580144480754f, 3.0f};
    float exp_dist = 100.45f;

    float enu[3];
    position_geodetic_to_enu(llh1[0],llh1[1],llh1[2],llh0[0],llh0[1],llh0[2],&enu[0], &enu[1], &enu[2]);

    float dist = position_xyz_distance(0.0f, 0.0f, 0.0f, enu[0], enu[1], enu[2]);

    ASSERT_LE(sq_error(exp_dist, dist), 1.0f);
}

/**
 * Geodetic to ENU and Position
 */
TEST(Position, test_geodetic_to_enu_002)
{
    float llh0[3] = {39.47314319954006f, -0.36773293176583255f, 13.0f};
    float llh1[3] = {39.47229865871014f, -0.36729115805255386f, 13.0f};
    float exp_dist = 101.15f;

    float enu[3];
    position_geodetic_to_enu(llh1[0],llh1[1],llh1[2],llh0[0],llh0[1],llh0[2],&enu[0], &enu[1], &enu[2]);

    float dist = position_xyz_distance(0.0f, 0.0f, 0.0f, enu[0], enu[1], enu[2]);

    ASSERT_LE(sq_error(exp_dist, dist), 1.0f);
}

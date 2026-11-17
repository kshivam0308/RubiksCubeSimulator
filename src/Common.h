/**
 * @file Common.h
 * @brief Global constants, color definitions, and mathematical utilities
 *        for the 3D Rubik's Cube Computer Graphics Project.
 *
 * Computer Graphics Concepts:
 * - Color Space: RGB Normalized coordinates [0.0f, 1.0f].
 * - Grid Coordinates: 3x3x3 Discrete Cartesian space where x, y, z in {-1, 0, 1}.
 * - Face Normals: Standard outward unit normal vectors along Cartesian axes (+X, -X, +Y, -Y, +Z, -Z).
 */

#ifndef COMMON_H
#define COMMON_H

#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Physical dimension constants
const float CUBIE_SIZE = 0.94f;   // Size of each individual cubelet (leaves a slight realistic gap)
const float CUBIE_HALF = CUBIE_SIZE / 2.0f;
const float SPACING    = 1.00f;   // Distance between adjacent cubie centers

// Face Index Enumeration
enum CubeFace {
    FACE_RIGHT  = 0, // +X face
    FACE_LEFT   = 1, // -X face
    FACE_TOP    = 2, // +Y face
    FACE_BOTTOM = 3, // -Y face
    FACE_FRONT  = 4, // +Z face
    FACE_BACK   = 5  // -Z face
};

// Color Palette Index Enumeration
enum CubeColor {
    COLOR_INNER  = 0, // Dark Charcoal / Black for internal hidden faces
    COLOR_RED    = 1, // Right Face (+X)
    COLOR_ORANGE = 2, // Left Face (-X)
    COLOR_WHITE  = 3, // Top Face (+Y)
    COLOR_YELLOW = 4, // Bottom Face (-Y)
    COLOR_GREEN  = 5, // Front Face (+Z)
    COLOR_BLUE   = 6  // Back Face (-Z)
};

// Standard RGB Color definitions
// Matches official Rubik's Cube color scheme with enhanced visual vibrancy
const float CUBE_PALETTE[7][3] = {
    {0.12f, 0.12f, 0.12f}, // 0: Inner core (Matte Black)
    {0.85f, 0.10f, 0.10f}, // 1: Red
    {1.00f, 0.45f, 0.00f}, // 2: Orange
    {0.95f, 0.95f, 0.95f}, // 3: White
    {0.95f, 0.85f, 0.00f}, // 4: Yellow
    {0.00f, 0.65f, 0.20f}, // 5: Green
    {0.00f, 0.35f, 0.85f}  // 6: Blue
};

/**
 * @brief Loads a 4x4 Identity Matrix in OpenGL column-major order.
 * Identity Matrix leaves coordinates unchanged: M * v = v.
 */
inline void loadIdentity4x4(float* m) {
    for (int i = 0; i < 16; ++i) {
        m[i] = (i % 5 == 0) ? 1.0f : 0.0f;
    }
}

/**
 * @brief Helper to convert degrees to radians.
 */
inline float degToRad(float deg) {
    return deg * (float)(M_PI / 180.0);
}

#endif // COMMON_H

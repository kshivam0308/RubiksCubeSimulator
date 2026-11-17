/**
 * @file Cubie.h
 * @brief Represents an individual small cube (1 of the 27 cubelets in a 3x3x3 Rubik's Cube).
 *
 * Computer Graphics Concepts:
 * - Local vs World Space: Each cubie is modeled at the local origin [-s, s]^3,
 *   then transformed into world coordinates via Translation & Rotation.
 * - Transformation Matrix: Uses a 4x4 matrix in column-major order to accumulate
 *   arbitrary 3D rotations without accumulating floating point error on the position.
 * - Surface Normals: Each face specifies a normal vector (nx, ny, nz) perpendicular
 *   to its plane for Lambertian diffuse and ambient lighting calculation.
 */

#ifndef CUBIE_H
#define CUBIE_H

#ifdef _WIN32
#include <windows.h>
#endif
#include <GL/glut.h>
#include "Common.h"

class Cubie {
public:
    // Logical grid coordinate: x, y, z in {-1, 0, 1}
    int x;
    int y;
    int z;

    // 4x4 Transformation Matrix in OpenGL column-major order
    // Accumulates all finalized rotations applied to this cubie
    float transform[16];

    // Color indices for each of the 6 faces (Right, Left, Top, Bottom, Front, Back)
    int faceColors[6];

    Cubie();

    /**
     * @brief Initializes the cubie at a given logical grid position (lx, ly, lz).
     * Automatically assigns outer face colors and internal black faces.
     */
    void init(int lx, int ly, int lz);

    /**
     * @brief Renders the cubie using OpenGL primitives (GL_QUADS and GL_LINES).
     * Applies translation to logical position and multiplies accumulated rotation.
     */
    void render() const;

    /**
     * @brief Multiplies an incoming 4x4 rotation matrix into the cubie's transform:
     * transform_new = rotMat * transform_old.
     */
    void applyRotationMatrix(const float rotMat[16]);

private:
    void drawFaces() const;
    void drawOutlines() const;
};

#endif // CUBIE_H

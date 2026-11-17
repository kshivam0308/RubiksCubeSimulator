/**
 * @file Camera.h
 * @brief 3D Perspective Camera with Orbit and Zoom controls.
 *
 * Computer Graphics Concepts:
 * - Viewing Transformation (Modelview Matrix): Moves and rotates the entire world
 *   relative to the eye position.
 * - Spherical Orbit Coordinates: Represented via Euler angles (xRot for elevation,
 *   yRot for azimuth) centered on the Rubik's Cube origin (0, 0, 0).
 * - Zoom by Scaling vs Distance: Applies scale factor to coordinate space,
 *   scaling geometry uniformly along all three axes.
 */

#ifndef CAMERA_H
#define CAMERA_H

#ifdef _WIN32
#include <windows.h>
#endif
#include <GL/glut.h>

class Camera {
public:
    float xRot;        // Pitch rotation in degrees
    float yRot;        // Yaw rotation in degrees
    float zoom;        // Zoom scaling factor
    float distance;    // Eye distance from cube center

    // Mouse drag state tracking
    bool isDragging;
    bool isRightDragging;
    int lastMouseX;
    int lastMouseY;

    Camera();

    /**
     * @brief Resets the camera orientation and zoom to the default viewing angle.
     */
    void reset();

    /**
     * @brief Applies the camera view transformations to the OpenGL Modelview matrix.
     */
    void applyView() const;

    /**
     * @brief Handles mouse button press and release events.
     */
    void handleMouse(int button, int state, int x, int y);

    /**
     * @brief Handles mouse motion while buttons are dragged.
     */
    void handleMotion(int x, int y);

    /**
     * @brief Handles zoom in or zoom out operations.
     */
    void adjustZoom(float delta);
};

#endif // CAMERA_H

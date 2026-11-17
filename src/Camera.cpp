/**
 * @file Camera.cpp
 * @brief Implementation of the Camera class.
 */

#include "Camera.h"

Camera::Camera() {
    reset();
}

void Camera::reset() {
    xRot = 28.0f;     // Slight downward tilt for 3D isometric-like perspective
    yRot = -42.0f;    // 45-degree angle showing Front, Right, and Top faces simultaneously
    zoom = 1.0f;
    distance = 9.5f;

    isDragging = false;
    isRightDragging = false;
    lastMouseX = 0;
    lastMouseY = 0;
}

void Camera::applyView() const {
    // 1. Move camera back along the negative Z-axis away from the cube origin
    glTranslatef(0.0f, 0.0f, -distance);

    // 2. Uniform Scaling Transformation: Zoom in/out
    glScalef(zoom, zoom, zoom);

    // 3. Rotation Transformation: Orbit around X-axis (Pitch / Elevation)
    glRotatef(xRot, 1.0f, 0.0f, 0.0f);

    // 4. Rotation Transformation: Orbit around Y-axis (Yaw / Azimuth)
    glRotatef(yRot, 0.0f, 1.0f, 0.0f);
}

void Camera::handleMouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            isDragging = true;
            lastMouseX = x;
            lastMouseY = y;
        } else if (state == GLUT_UP) {
            isDragging = false;
        }
    } else if (button == GLUT_RIGHT_BUTTON) {
        if (state == GLUT_DOWN) {
            isRightDragging = true;
            lastMouseY = y;
        } else if (state == GLUT_UP) {
            isRightDragging = false;
        }
    }

    // FreeGLUT Mouse Wheel Support:
    // Button 3 = Wheel Up (Zoom in)
    // Button 4 = Wheel Down (Zoom out)
    if (button == 3) {
        adjustZoom(0.08f);
    } else if (button == 4) {
        adjustZoom(-0.08f);
    }
}

void Camera::handleMotion(int x, int y) {
    if (isDragging) {
        int dx = x - lastMouseX;
        int dy = y - lastMouseY;

        // Orbit angles: Sensitivity factor of 0.4 degrees per pixel
        yRot += (float)dx * 0.4f;
        xRot += (float)dy * 0.4f;

        // Clamp elevation angle to prevent confusing upside-down camera flips
        if (xRot > 89.0f)  xRot = 89.0f;
        if (xRot < -89.0f) xRot = -89.0f;

        lastMouseX = x;
        lastMouseY = y;
    } else if (isRightDragging) {
        int dy = y - lastMouseY;
        // Right-click vertical drag zooms in and out
        adjustZoom(-(float)dy * 0.01f);
        lastMouseY = y;
    }
}

void Camera::adjustZoom(float delta) {
    zoom += delta;
    if (zoom < 0.25f) zoom = 0.25f; // Prevent inverting or vanishing
    if (zoom > 3.00f) zoom = 3.00f; // Prevent excessive clipping into near plane
}

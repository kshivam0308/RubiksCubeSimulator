/**
 * @file InputHandler.h
 * @brief Handles keyboard and mouse events and delegates actions to RubiksCube, Camera, and Renderer.
 */

#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#ifdef _WIN32
#include <windows.h>
#endif
#include <GL/glut.h>
#include "RubiksCube.h"
#include "Camera.h"
#include "Renderer.h"

class InputHandler {
public:
    static void init(RubiksCube* cubePtr, Camera* cameraPtr, Renderer* rendererPtr);

    static void onKeyboard(unsigned char key, int x, int y);
    static void onSpecialKey(int key, int x, int y);
    static void onMouse(int button, int state, int x, int y);
    static void onMotion(int x, int y);

    static void printControls();

private:
    static RubiksCube* cube;
    static Camera* camera;
    static Renderer* renderer;
};

#endif // INPUTHANDLER_H

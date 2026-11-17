/**
 * @file main.cpp
 * @brief Application entry point for the 3D Rubik's Cube Simulator.
 */

#ifdef _WIN32
#include <windows.h>
#endif
#include <GL/glut.h>
#include <cstdlib>
#include <ctime>
#include <iostream>

#include "Common.h"
#include "RubiksCube.h"
#include "Camera.h"
#include "Renderer.h"
#include "InputHandler.h"

// Global Application Subsystems
static RubiksCube g_cube;
static Camera     g_camera;
static Renderer   g_renderer;

// GLUT Display Callback
void displayCallback() {
    g_renderer.renderScene(g_cube, g_camera);
}

// GLUT Window Reshape Callback
void reshapeCallback(int width, int height) {
    g_renderer.reshape(width, height);
}

// GLUT Keyboard Callback
void keyboardCallback(unsigned char key, int x, int y) {
    InputHandler::onKeyboard(key, x, y);
}

// GLUT Special Keyboard Callback (Arrow keys, Home, etc.)
void specialCallback(int key, int x, int y) {
    InputHandler::onSpecialKey(key, x, y);
}

// GLUT Mouse Click Callback
void mouseCallback(int button, int state, int x, int y) {
    InputHandler::onMouse(button, state, x, y);
}

// GLUT Mouse Drag Callback
void motionCallback(int x, int y) {
    InputHandler::onMotion(x, y);
}

// GLUT Frame-rate Animation Timer Callback (~60 FPS)
void timerCallback(int value) {
    // Progress slice rotation animation if active
    g_cube.updateAnimation();

    // Trigger frame redraw
    glutPostRedisplay();

    // 16 milliseconds ~= 60 FPS
    glutTimerFunc(16, timerCallback, 0);
}

int main(int argc, char** argv) {
    srand((unsigned int)time(NULL));

    // Initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(g_renderer.windowWidth, g_renderer.windowHeight);
    glutInitWindowPosition(100, 60);
    glutCreateWindow("3D Rubik's Cube Simulator");

    // Initialize OpenGL graphics context & lighting
    g_renderer.initGL();

    // Connect Input subsystem with Cube, Camera, and Renderer
    InputHandler::init(&g_cube, &g_camera, &g_renderer);

    // Register GLUT event callbacks
    glutDisplayFunc(displayCallback);
    glutReshapeFunc(reshapeCallback);
    glutKeyboardFunc(keyboardCallback);
    glutSpecialFunc(specialCallback);
    glutMouseFunc(mouseCallback);
    glutMotionFunc(motionCallback);
    glutTimerFunc(16, timerCallback, 0);

    // Print console guide
    InputHandler::printControls();

    // Enter GLUT event loop
    glutMainLoop();

    return 0;
}

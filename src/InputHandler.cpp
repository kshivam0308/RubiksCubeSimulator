/**
 * @file InputHandler.cpp
 * @brief Implementation of the InputHandler class.
 */

#include "InputHandler.h"
#include <iostream>
#include <cstdlib>

RubiksCube* InputHandler::cube = nullptr;
Camera* InputHandler::camera = nullptr;
Renderer* InputHandler::renderer = nullptr;

void InputHandler::init(RubiksCube* cubePtr, Camera* cameraPtr, Renderer* rendererPtr) {
    cube = cubePtr;
    camera = cameraPtr;
    renderer = rendererPtr;
}

void InputHandler::onKeyboard(unsigned char key, int x, int y) {
    if (!cube || !camera) return;

    switch (key) {
        // --- Face Rotations ---
        // Right Face (X = 1)
        case 'r': cube->startMove(0,  1, -1, "R'"); break;
        case 'R': cube->startMove(0,  1,  1, "R");  break;

        // Left Face (X = -1)
        case 'l': cube->startMove(0, -1,  1, "L'"); break;
        case 'L': cube->startMove(0, -1, -1, "L");  break;

        // Up / Top Face (Y = 1)
        case 'u': cube->startMove(1,  1, -1, "U'"); break;
        case 'U': cube->startMove(1,  1,  1, "U");  break;

        // Down / Bottom Face (Y = -1)
        case 'd': cube->startMove(1, -1,  1, "D'"); break;
        case 'D': cube->startMove(1, -1, -1, "D");  break;

        // Front Face (Z = 1)
        case 'f': cube->startMove(2,  1, -1, "F'"); break;
        case 'F': cube->startMove(2,  1,  1, "F");  break;

        // Back Face (Z = -1)
        case 'b': cube->startMove(2, -1,  1, "B'"); break;
        case 'B': cube->startMove(2, -1, -1, "B");  break;

        // --- Middle Slice Rotations ---
        case 'v': cube->startMove(0,  0, -1, "V'"); break;
        case 'V': cube->startMove(0,  0,  1, "V");  break;
        case 'h': cube->startMove(1,  0, -1, "H'"); break;
        case 'H': cube->startMove(1,  0,  1, "H");  break;
        case 'm': cube->startMove(2,  0, -1, "M'"); break;
        case 'M': cube->startMove(2,  0,  1, "M");  break;

        // --- Scramble: SPACEBAR or S ---
        case ' ':
        case 's':
        case 'S':
            cube->scramble(15);
            break;

        // --- Reset: '0' or 'C' ---
        case '0':
        case 'c':
        case 'C':
            std::cout << "[Rubik's Action] Resetting to solved state." << std::endl;
            cube->reset();
            break;

        // --- Wireframe Mode Toggle: 'W' / 'w' ---
        case 'w':
        case 'W':
            if (renderer) {
                renderer->toggleWireframe();
                std::cout << "[Renderer] Wireframe mode toggled: " << (renderer->isWireframe ? "ON" : "OFF") << std::endl;
            }
            break;

        // --- Reset Camera View: Home key or '0' (handled above) ---
        case '9':
            std::cout << "[Camera Action] Resetting camera view." << std::endl;
            camera->reset();
            break;

        // --- Zoom Controls ---
        case '+':
        case '=':
            camera->adjustZoom(0.08f);
            break;

        case '-':
        case '_':
            camera->adjustZoom(-0.08f);
            break;

        // --- ESC: Exit ---
        case 27:
            std::cout << "\nExiting Rubik's Cube Simulator. Goodbye!\n" << std::endl;
            exit(0);
            break;

        default:
            break;
    }

    glutPostRedisplay();
}

void InputHandler::onSpecialKey(int key, int x, int y) {
    if (!camera) return;

    switch (key) {
        case GLUT_KEY_LEFT:
            camera->yRot -= 5.0f;
            break;
        case GLUT_KEY_RIGHT:
            camera->yRot += 5.0f;
            break;
        case GLUT_KEY_UP:
            camera->xRot -= 5.0f;
            if (camera->xRot < -89.0f) camera->xRot = -89.0f;
            break;
        case GLUT_KEY_DOWN:
            camera->xRot += 5.0f;
            if (camera->xRot > 89.0f) camera->xRot = 89.0f;
            break;
        case GLUT_KEY_HOME:
            camera->reset();
            break;
    }

    glutPostRedisplay();
}

void InputHandler::onMouse(int button, int state, int x, int y) {
    if (camera) {
        camera->handleMouse(button, state, x, y);
    }
    glutPostRedisplay();
}

void InputHandler::onMotion(int x, int y) {
    if (camera) {
        camera->handleMotion(x, y);
    }
    glutPostRedisplay();
}

void InputHandler::printControls() {
    std::cout << "\n=======================================================\n";
    std::cout << "        3D RUBIK'S CUBE SIMULATOR (OPENGL / C++)       \n";
    std::cout << "=======================================================\n";
    std::cout << " [ON-SCREEN CONTROLS]\n";
    std::cout << "   - R/L/U/D/F/B : Face turns (lowercase for reverse)\n";
    std::cout << "   - SPACE       : Scramble (15 random moves)\n";
    std::cout << "   - 0 / C       : Reset cube to solved state\n";
    std::cout << "   - W           : Toggle Wireframe mode\n";
    std::cout << "   - + / -       : Zoom in / out\n";
    std::cout << "   - Left Drag   : Rotate camera view\n";
    std::cout << "   - Wheel       : Zoom in / out\n";
    std::cout << "   - ESC         : Exit program\n";
    std::cout << "=======================================================\n\n";
}

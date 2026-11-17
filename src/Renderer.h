/**
 * @file Renderer.h
 * @brief Manages OpenGL initialization, lighting, background gradient, HUD, and scene rendering.
 */

#ifndef RENDERER_H
#define RENDERER_H

#ifdef _WIN32
#include <windows.h>
#endif
#include <GL/glut.h>
#include <string>
#include "RubiksCube.h"
#include "Camera.h"

class Renderer {
public:
    int windowWidth;
    int windowHeight;
    bool isWireframe;

    Renderer();

    /**
     * @brief Initializes OpenGL states, lighting models, depth testing, and color materials.
     */
    void initGL();

    /**
     * @brief Handles window resize / reshape events, adjusting the perspective projection matrix.
     */
    void reshape(int width, int height);

    /**
     * @brief Renders the complete frame: background gradient, 3D cube, and 2D HUD.
     */
    void renderScene(const RubiksCube& cube, const Camera& camera);

    /**
     * @brief Toggles wireframe polygon rendering mode.
     */
    void toggleWireframe();

private:
    void renderBackground();
    void renderHUD(int moveCount, const std::string& lastMove);
    void drawText(float x, float y, const std::string& text, void* font = GLUT_BITMAP_9_BY_15);
};

#endif // RENDERER_H

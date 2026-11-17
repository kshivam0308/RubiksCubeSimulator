/**
 * @file Renderer.cpp
 * @brief Implementation of the Renderer class.
 */

#include "Renderer.h"
#include <sstream>

Renderer::Renderer() {
    windowWidth = 960;
    windowHeight = 720;
    isWireframe = false;
}

void Renderer::initGL() {
    // 1. Clear background color
    glClearColor(0.04f, 0.05f, 0.08f, 1.0f);

    // 2. Depth Buffering (Z-Buffer): Eliminates hidden surface removal artifacts
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    // 3. Smooth Shading
    glShadeModel(GL_SMOOTH);

    // 4. Normal Vector Normalization
    glEnable(GL_NORMALIZE);

    // 5. Material Color Tracking: glColor controls ambient and diffuse material properties
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

    // 6. Lighting Setup: Two-point lighting for realistic 3D appearance
    glEnable(GL_LIGHTING);

    // Key Light (top-right-front)
    glEnable(GL_LIGHT0);
    GLfloat lightPos0[]   = { 7.0f, 9.0f, 8.0f, 0.0f }; // Directional light
    GLfloat lightDiff0[]  = { 0.90f, 0.90f, 0.90f, 1.0f };
    GLfloat lightSpec0[]  = { 0.35f, 0.35f, 0.35f, 1.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiff0);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpec0);

    // Fill Light (bottom-left-back)
    glEnable(GL_LIGHT1);
    GLfloat lightPos1[]   = {-7.0f, -6.0f, -7.0f, 0.0f };
    GLfloat lightDiff1[]  = { 0.45f, 0.45f, 0.50f, 1.0f };
    glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, lightDiff1);

    // Ambient Illumination: Ensures unlit faces retain clear color identification
    GLfloat globalAmbient[] = { 0.35f, 0.35f, 0.35f, 1.0f };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbient);
}

void Renderer::reshape(int width, int height) {
    if (height <= 0) height = 1;
    windowWidth = width;
    windowHeight = height;

    // Viewport Transformation: Map normalized coordinates to screen pixel coordinates
    glViewport(0, 0, width, height);

    // Perspective Projection Transformation
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float aspectRatio = (float)width / (float)height;
    gluPerspective(45.0f, aspectRatio, 0.1f, 100.0f);

    glMatrixMode(GL_MODELVIEW);
}

void Renderer::toggleWireframe() {
    isWireframe = !isWireframe;
}

void Renderer::drawText(float x, float y, const std::string& text, void* font) {
    glRasterPos2f(x, y);
    for (char c : text) {
        glutBitmapCharacter(font, c);
    }
}

void Renderer::renderBackground() {
    // 2D Fullscreen quad pass
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glBegin(GL_QUADS);
    // Top background color: Deep midnight navy / charcoal
    glColor3f(0.06f, 0.08f, 0.14f);
    glVertex2f(-1.0f,  1.0f);
    glVertex2f( 1.0f,  1.0f);

    // Bottom background color: Soft dark obsidian
    glColor3f(0.02f, 0.03f, 0.05f);
    glVertex2f( 1.0f, -1.0f);
    glVertex2f(-1.0f, -1.0f);
    glEnd();

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
}

void Renderer::renderHUD(int moveCount, const std::string& lastMove) {
    // Switch to 2D Orthographic screen-space projection
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, windowWidth, 0, windowHeight);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    // 1. Top HUD: Title and Move Counter
    std::ostringstream topMsg;
    topMsg << "3D RUBIK'S CUBE SIMULATOR    |    Moves: " << moveCount;
    if (!lastMove.empty() && lastMove != "Reset") {
        topMsg << "    (" << lastMove << ")";
    }

    glColor3f(0.95f, 0.95f, 0.98f); // Bright crisp white
    drawText(24.0f, (float)(windowHeight - 32), topMsg.str(), GLUT_BITMAP_9_BY_15);

    // 2. Bottom HUD: Controls Overlay matching reference design
    // Line 1: Keyboard actions
    glColor3f(0.85f, 0.88f, 0.92f);
    drawText(24.0f, 38.0f, "R/L/U/D/F/B: turn    lowercase: reverse    SPACE: scramble    0: reset    W: wireframe    +/-: zoom", GLUT_BITMAP_9_BY_15);

    // Line 2: Mouse and exit controls
    glColor3f(0.70f, 0.74f, 0.80f);
    drawText(24.0f, 18.0f, "Left mouse drag: rotate camera    Wheel: zoom    ESC: exit", GLUT_BITMAP_9_BY_15);

    // Restore 3D projection matrix
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
}

void Renderer::renderScene(const RubiksCube& cube, const Camera& camera) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // 1. Draw atmospheric background gradient
    renderBackground();

    // 2. Setup Modelview Matrix with camera viewing transformation
    glLoadIdentity();
    camera.applyView();

    // 3. Render 3D Rubik's Cube
    if (isWireframe) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    } else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    cube.render();

    // Always reset polygon mode to FILL for 2D UI elements
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // 4. Render 2D on-screen HUD (Header and Bottom Controls)
    renderHUD(cube.moveCount, cube.lastMove);

    // 5. Swap buffers for smooth double-buffered animation
    glutSwapBuffers();
}

/**
 * @file Cubie.cpp
 * @brief Implementation of the Cubie class.
 */

#include "Cubie.h"

Cubie::Cubie() {
    init(0, 0, 0);
}

void Cubie::init(int lx, int ly, int lz) {
    x = lx;
    y = ly;
    z = lz;

    // Reset rotation transform to identity matrix (no initial rotation)
    loadIdentity4x4(transform);

    // Assign face colors based on whether this face is on the exterior of the 3x3x3 cube.
    // Faces facing the inside of the cube are given COLOR_INNER (black).
    // Faces facing the outside receive standard Rubik's cube sticker colors.
    faceColors[FACE_RIGHT]  = (lx == 1)  ? COLOR_RED    : COLOR_INNER;
    faceColors[FACE_LEFT]   = (lx == -1) ? COLOR_ORANGE : COLOR_INNER;
    faceColors[FACE_TOP]    = (ly == 1)  ? COLOR_WHITE  : COLOR_INNER;
    faceColors[FACE_BOTTOM] = (ly == -1) ? COLOR_YELLOW : COLOR_INNER;
    faceColors[FACE_FRONT]  = (lz == 1)  ? COLOR_GREEN  : COLOR_INNER;
    faceColors[FACE_BACK]   = (lz == -1) ? COLOR_BLUE   : COLOR_INNER;
}

void Cubie::render() const {
    glPushMatrix();

    // 1. Translation Transformation: Move cubie from local origin to its grid position
    glTranslatef((float)x * SPACING, (float)y * SPACING, (float)z * SPACING);

    // 2. Rotation Transformation: Apply accumulated orientation matrix
    glMultMatrixf(transform);

    // 3. Draw colored sticker faces with normals for 3D lighting
    drawFaces();

    // 4. Draw black beveled edge outlines for visual separation
    drawOutlines();

    glPopMatrix();
}

void Cubie::drawFaces() const {
    float s = CUBIE_HALF;

    glBegin(GL_QUADS);

    // 1. Right Face (+X) -> Normal: ( 1, 0, 0)
    glColor3fv(CUBE_PALETTE[faceColors[FACE_RIGHT]]);
    glNormal3f(1.0f, 0.0f, 0.0f);
    glVertex3f( s, -s,  s);
    glVertex3f( s, -s, -s);
    glVertex3f( s,  s, -s);
    glVertex3f( s,  s,  s);

    // 2. Left Face (-X) -> Normal: (-1, 0, 0)
    glColor3fv(CUBE_PALETTE[faceColors[FACE_LEFT]]);
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glVertex3f(-s, -s, -s);
    glVertex3f(-s, -s,  s);
    glVertex3f(-s,  s,  s);
    glVertex3f(-s,  s, -s);

    // 3. Top Face (+Y) -> Normal: ( 0, 1, 0)
    glColor3fv(CUBE_PALETTE[faceColors[FACE_TOP]]);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-s,  s,  s);
    glVertex3f( s,  s,  s);
    glVertex3f( s,  s, -s);
    glVertex3f(-s,  s, -s);

    // 4. Bottom Face (-Y) -> Normal: ( 0,-1, 0)
    glColor3fv(CUBE_PALETTE[faceColors[FACE_BOTTOM]]);
    glNormal3f(0.0f, -1.0f, 0.0f);
    glVertex3f(-s, -s, -s);
    glVertex3f( s, -s, -s);
    glVertex3f( s, -s,  s);
    glVertex3f(-s, -s,  s);

    // 5. Front Face (+Z) -> Normal: ( 0, 0, 1)
    glColor3fv(CUBE_PALETTE[faceColors[FACE_FRONT]]);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3f(-s, -s,  s);
    glVertex3f( s, -s,  s);
    glVertex3f( s,  s,  s);
    glVertex3f(-s,  s,  s);

    // 6. Back Face (-Z) -> Normal: ( 0, 0,-1)
    glColor3fv(CUBE_PALETTE[faceColors[FACE_BACK]]);
    glNormal3f(0.0f, 0.0f, -1.0f);
    glVertex3f( s, -s, -s);
    glVertex3f(-s, -s, -s);
    glVertex3f(-s,  s, -s);
    glVertex3f( s,  s, -s);

    glEnd();
}

void Cubie::drawOutlines() const {
    float s = CUBIE_HALF;

    // Temporarily disable lighting on lines so they render as pure crisp black edges
    glDisable(GL_LIGHTING);
    glColor3f(0.05f, 0.05f, 0.05f);
    glLineWidth(2.5f);

    glBegin(GL_LINES);
    // 12 edges of the cubie
    // Bottom 4 edges
    glVertex3f(-s, -s, -s); glVertex3f( s, -s, -s);
    glVertex3f( s, -s, -s); glVertex3f( s, -s,  s);
    glVertex3f( s, -s,  s); glVertex3f(-s, -s,  s);
    glVertex3f(-s, -s,  s); glVertex3f(-s, -s, -s);

    // Top 4 edges
    glVertex3f(-s,  s, -s); glVertex3f( s,  s, -s);
    glVertex3f( s,  s, -s); glVertex3f( s,  s,  s);
    glVertex3f( s,  s,  s); glVertex3f(-s,  s,  s);
    glVertex3f(-s,  s,  s); glVertex3f(-s,  s, -s);

    // Vertical 4 edges
    glVertex3f(-s, -s, -s); glVertex3f(-s,  s, -s);
    glVertex3f( s, -s, -s); glVertex3f( s,  s, -s);
    glVertex3f( s, -s,  s); glVertex3f( s,  s,  s);
    glVertex3f(-s, -s,  s); glVertex3f(-s,  s,  s);
    glEnd();

    glEnable(GL_LIGHTING);
}

void Cubie::applyRotationMatrix(const float rotMat[16]) {
    // Perform matrix multiplication: transform = rotMat * transform
    // Utilizing OpenGL's hardware matrix stack ensures accurate column-major multiplication
    glPushMatrix();
    glLoadIdentity();
    glMultMatrixf(rotMat);
    glMultMatrixf(transform);
    glGetFloatv(GL_MODELVIEW_MATRIX, transform);
    glPopMatrix();
}

/**
 * @file RubiksCube.cpp
 * @brief Implementation of the RubiksCube class.
 */

#include "RubiksCube.h"
#include <cstdlib>
#include <ctime>
#include <iostream>

RubiksCube::RubiksCube() {
    isAnimating = false;
    animAxis = 0;
    animSlice = 0;
    animDir = 1;
    animAngle = 0.0f;
    animSpeed = 6.0f;    // 6.0 degrees/frame -> 15 frames @ 60 FPS = 0.25 sec smooth rotation
    animTarget = 90.0f;  // Each face turn is a 90-degree quarter turn

    moveCount = 0;
    lastMove = "";

    reset();
}

void RubiksCube::reset() {
    // Clear any pending moves in queue
    while (!moveQueue.empty()) {
        moveQueue.pop();
    }
    isAnimating = false;
    animAngle = 0.0f;
    moveCount = 0;
    lastMove = "Reset";

    // Initialize all 27 cubies in the 3x3x3 grid (-1, 0, 1 on each axis)
    int index = 0;
    for (int x = -1; x <= 1; ++x) {
        for (int y = -1; y <= 1; ++y) {
            for (int z = -1; z <= 1; ++z) {
                cubies[index].init(x, y, z);
                index++;
            }
        }
    }
}

void RubiksCube::render() const {
    for (int i = 0; i < TOTAL_CUBIES; ++i) {
        const Cubie& c = cubies[i];

        // Determine if this cubie is part of the currently rotating slice
        bool inSlice = false;
        if (isAnimating) {
            if (animAxis == 0 && c.x == animSlice) inSlice = true;
            if (animAxis == 1 && c.y == animSlice) inSlice = true;
            if (animAxis == 2 && c.z == animSlice) inSlice = true;
        }

        glPushMatrix();

        // 1. Dynamic Layer Rotation (Hierarchical Transformation around origin)
        if (inSlice) {
            float currentAngle = animAngle * (float)animDir;
            if (animAxis == 0) glRotatef(currentAngle, 1.0f, 0.0f, 0.0f);
            if (animAxis == 1) glRotatef(currentAngle, 0.0f, 1.0f, 0.0f);
            if (animAxis == 2) glRotatef(currentAngle, 0.0f, 0.0f, 1.0f);
        }

        // 2. Render individual cubie (handles translation to (x,y,z) and accumulated matrix)
        c.render();

        glPopMatrix();
    }
}

void RubiksCube::startMove(int axis, int slice, int dir, const std::string& notation) {
    if (isAnimating) {
        // If an animation is already running, queue the move for smooth sequential execution
        moveQueue.push(Move(axis, slice, dir, notation));
        return;
    }

    animAxis = axis;
    animSlice = slice;
    animDir = dir;
    animAngle = 0.0f;
    isAnimating = true;

    if (!notation.empty()) {
        lastMove = notation;
        moveCount++;
        std::cout << "[Rubik's Move #" << moveCount << "] " << notation << std::endl;
    }
}

void RubiksCube::updateAnimation() {
    if (isAnimating) {
        animAngle += animSpeed;
        if (animAngle >= animTarget) {
            animAngle = animTarget;
            finalizeRotation();
            isAnimating = false;

            // Check if sequential moves are waiting in the queue
            if (!moveQueue.empty()) {
                Move nextMove = moveQueue.front();
                moveQueue.pop();
                startMove(nextMove.axis, nextMove.slice, nextMove.dir, nextMove.notation);
            }
        }
    }
}

void RubiksCube::finalizeRotation() {
    // 1. Construct the exact 90-degree rotation matrix for this slice turn
    float rotMat[16];
    glPushMatrix();
    glLoadIdentity();
    if (animAxis == 0) glRotatef(90.0f * (float)animDir, 1.0f, 0.0f, 0.0f);
    if (animAxis == 1) glRotatef(90.0f * (float)animDir, 0.0f, 1.0f, 0.0f);
    if (animAxis == 2) glRotatef(90.0f * (float)animDir, 0.0f, 0.0f, 1.0f);
    glGetFloatv(GL_MODELVIEW_MATRIX, rotMat);
    glPopMatrix();

    int cosVal = 0;
    int sinVal = animDir;

    // 2. Update cubies belonging to the active slice
    for (int i = 0; i < TOTAL_CUBIES; ++i) {
        bool inSlice = false;
        if (animAxis == 0 && cubies[i].x == animSlice) inSlice = true;
        if (animAxis == 1 && cubies[i].y == animSlice) inSlice = true;
        if (animAxis == 2 && cubies[i].z == animSlice) inSlice = true;

        if (inSlice) {
            int oldX = cubies[i].x;
            int oldY = cubies[i].y;
            int oldZ = cubies[i].z;
            int newX = oldX;
            int newY = oldY;
            int newZ = oldZ;

            // Mathematical rotation of 3D integer coordinates:
            if (animAxis == 0) {
                newY = oldY * cosVal - oldZ * sinVal;
                newZ = oldY * sinVal + oldZ * cosVal;
            } else if (animAxis == 1) {
                newX =  oldX * cosVal + oldZ * sinVal;
                newZ = -oldX * sinVal + oldZ * cosVal;
            } else if (animAxis == 2) {
                newX = oldX * cosVal - oldY * sinVal;
                newY = oldX * sinVal + oldY * cosVal;
            }

            cubies[i].x = newX;
            cubies[i].y = newY;
            cubies[i].z = newZ;

            // Accumulate rotation transformation matrix for this cubie
            cubies[i].applyRotationMatrix(rotMat);
        }
    }
}

void RubiksCube::scramble(int count) {
    std::cout << "\n--- Scrambling Rubik's Cube (" << count << " random moves) ---" << std::endl;
    struct FaceChoice {
        int axis;
        int slice;
        int dir;
        const char* name;
    };

    FaceChoice choices[12] = {
        {0,  1,  1, "R"},
        {0,  1, -1, "R'"},
        {0, -1, -1, "L"},
        {0, -1,  1, "L'"},
        {1,  1,  1, "U"},
        {1,  1, -1, "U'"},
        {1, -1, -1, "D"},
        {1, -1,  1, "D'"},
        {2,  1,  1, "F"},
        {2,  1, -1, "F'"},
        {2, -1, -1, "B"},
        {2, -1,  1, "B'"}
    };

    for (int i = 0; i < count; ++i) {
        int r = rand() % 12;
        startMove(choices[r].axis, choices[r].slice, choices[r].dir, choices[r].name);
    }
}

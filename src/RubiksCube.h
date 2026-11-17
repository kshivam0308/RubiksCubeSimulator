/**
 * @file RubiksCube.h
 * @brief Manages the 3x3x3 Rubik's Cube structure, animations, and slice rotations.
 */

#ifndef RUBIKSCUBE_H
#define RUBIKSCUBE_H

#include <vector>
#include <queue>
#include <string>
#include "Cubie.h"

struct Move {
    int axis;  // 0 = X, 1 = Y, 2 = Z
    int slice; // -1, 0, or 1
    int dir;   // +1 = CW, -1 = CCW
    std::string notation; // e.g., "R", "R'", "U", etc.

    Move(int a = 0, int s = 0, int d = 1, const std::string& n = "")
        : axis(a), slice(s), dir(d), notation(n) {}
};

class RubiksCube {
public:
    static const int TOTAL_CUBIES = 27;
    Cubie cubies[TOTAL_CUBIES];

    // Animation variables
    bool isAnimating;
    int animAxis;      // 0: X, 1: Y, 2: Z
    int animSlice;     // -1, 0, 1
    int animDir;       // +1 or -1
    float animAngle;   // Current interpolated angle [0.0f, 90.0f]
    float animSpeed;   // Rotation increment per frame
    float animTarget;  // Target rotation angle (90.0f degrees)

    // Game stats
    int moveCount;
    std::string lastMove;

    // Queue for sequential move execution
    std::queue<Move> moveQueue;

    RubiksCube();

    /**
     * @brief Resets all 27 cubies to their initial solved positions and colors.
     */
    void reset();

    /**
     * @brief Renders the entire Rubik's Cube, applying dynamic slice rotations
     *        to active cubies if an animation is underway.
     */
    void render() const;

    /**
     * @brief Initiates a face or slice rotation. If another move is active,
     *        enqueues the move to be executed sequentially.
     */
    void startMove(int axis, int slice, int dir, const std::string& notation = "");

    /**
     * @brief Updates animation progress. Must be called every frame (~60 FPS).
     */
    void updateAnimation();

    /**
     * @brief Enqueues a randomized series of face moves to scramble the cube.
     */
    void scramble(int count = 15);

private:
    /**
     * @brief Finalizes a 90-degree turn: updates logical positions (x, y, z)
     *        and multiplies the rotation matrix into the cubies of the rotated slice.
     */
    void finalizeRotation();
};

#endif // RUBIKSCUBE_H

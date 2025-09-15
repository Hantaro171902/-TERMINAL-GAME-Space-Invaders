#pragma once

#include <iostream>
#include <vector>
#include "alien.hpp"
#include "bullet.hpp"

class Invader {
private:
    // Alien grid
    std::vector<std::vector<Alien>> aliens;
    std::vector<std::vector<Bullet>> bullets; // Track alive status
    int rows;
    int cols;
    int alienSpacingX; // Horizontal spacing between aliens
    int alienSpacingY; // Vertical spacing between aliens
    int startX; // Starting X position for the grid
    int startY; // Starting Y position for the grid
    int direction; // 1 = right, -1 = left
    int speed; // Movement speed (lower is faster)
    // int moveCounter; // Counter to control movement timing
    // int descentAmount; // How much the aliens descend when changing direction
    int timeScinceLastBullet;; // Time since last bullet fired
    int bulletInterval; // Minimum interval between bullets (in
public:
    // Constructor
    Invader(int _rows, int _cols, int _startX, int _startY, int _spacingX, int _spacingY);
    ~Invader();

    void update();
    void draw() const;
    bool hasChangedDirection();
    void moveDown();
    int getBottomAlliens();
    void nextLevel();
    int getAllXPositions();
    int initaliseAliens();
    bool checkCollision(int x, int y);
    void makeABottomAlienShoot(std::string bottomAlien);
    void updateBullets();


};

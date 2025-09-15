#pragma once
#include <iostream>
#include "bullet.hpp"
#include "utils.hpp"
#include "color.hpp"

class SpaceShip {
private:
    int x, y; // Position
    int energy; // Number of lives
    int hp;
    bool active; // Is the spaceship active (not destroyed)

public:
    // Constructor
    SpaceShip(int _x, int _y);
    ~SpaceShip();

    int getX() const { return x; }
    int getY() const { return y; }
    int getEnergy() const { return energy; }
    int getHP() const { return hp; }
    
    void update();
    void updateBullets(std::vector<Bullet>& bullets);
    void draw() const;
    void erase() const;
    void move();
    void damage();
    void explode();


};
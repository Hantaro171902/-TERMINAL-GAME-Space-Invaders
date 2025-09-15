#pragma once

#include <iostream>
#include <vector>

class Bullet {
private:
    int x, y; // Position
    bool active; // Is the bullet active (in motion)

public:
    // Constructor
    Bullet(int _x, int _y);
    ~Bullet();

    int getX() const { return x; }
    int getY() const { return y; }
    bool isActive() const { return active; }

    void setPosition(int _x, int _y) { x = _x; y = _y; }
    void deactivate() { active = false; }
    void draw();
};

class AlliedBullet : public Bullet {
public:
    // Constructor
    AlliedBullet(int _x, int _y) : Bullet(_x, _y) {}
    ~AlliedBullet();

    void update();
};

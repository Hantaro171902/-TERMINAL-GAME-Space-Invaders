#include <iostream>
#include "bullet.hpp"
#include "color.hpp"
#include "utils.hpp"

using namespace std;

Bullet::~Bullet() {
    // Destructor logic if needed
}

Bullet::Bullet(int _x, int _y) : x(_x), y(_y) {}

int Bullet::getX() {
    return x;
}

int Bullet::getY() {
    return y;
}

bool Bullet::isOut() {
    return y < 1; // Assuming the top of the screen is at y = 1
}

void Bullet::Move() {
    y--; // Move the bullet up
}

void Bullet::draw() const {
    move_cursor(x, y);
    setTextColor(TextColor::YELLOW);
    cout << "."; // Represent the bullet with '|'
    resetTextColor();
}

void Bullet::clear() const {
    move_cursor(x, y);
    cout << " "; // Clear the bullet's previous position
}
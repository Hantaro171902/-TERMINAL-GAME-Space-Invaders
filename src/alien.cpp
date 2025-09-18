#include <vector>
#include "alien.hpp"

using namespace std;

Alien::~Alien() {
    // Destructor logic if needed
}

int Alien::getX() const {
    return x;
}

int Alien::getY() const {
    return y;
}

void Alien::draw() const {
    move_cursor(x, y);
    setTextColor(TextColor::GREEN);
    // cout << "A"; // Represent the alien with 'A'
    resetTextColor();
}
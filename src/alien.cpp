#include <vector>
#include "alien.hpp"

using namespace std;

Alien::~Alien() {
    // Destructor logic if needed
}

void Alien::draw() const {
    move_cursor(x, y);
    setTextColor(TextColor::GREEN);
    // cout << "A"; // Represent the alien with 'A'
    resetTextColor();
}
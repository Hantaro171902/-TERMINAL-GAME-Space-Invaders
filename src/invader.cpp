#include <vector>
#include "invader.hpp"

using namespace std;

Invader::~Invader() {
    // Destructor logic if needed
}

int Invader::getX() const {
    return x;
}

int Invader::getY() const {
    return y;
}

void Invader::draw() const {
    move_cursor(x, y);
    setTextColor(TextColor::GREEN);
    cout << "V"; // Represent the invader with 'V'
    resetTextColor();
}

void Invader::clear() const {
    move_cursor(x, y);
    cout << " "; // Clear the invader's previous position
}

void Invader::update() {
    // Update invader position or state
}

void Invader::move(int dx, int dy) {
    x += dx;
    y += dy;
}


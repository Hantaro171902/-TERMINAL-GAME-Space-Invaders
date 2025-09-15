#pragma once 

#include <string>
#include <vector>
#include <iostream>

class Alien {
private:
    int x, y; // Position    
public:
    
    // Constructor
    Alien(int _x, int _y) : x(_x), y(_y) {}
    ~Alien();

    int getX() const { return x; }
    int getY() const { return y; }

    void setPosition(int _x, int _y) { x = _x; y = _y; }
    void draw() const;

};
#include "point.hpp"

#include <iostream>


Point::Point(const float x, const float y, const float z) {
    _x = x;
    _y = y;
    _z = z;
}


float Point::x() const {
    return _x;
}

float Point::y() const {
    return _y;
}

float Point::z() const {
    return _z;
}


void Point::update(const float x, const float y, const float z) {
    _x = x;
    _y = y;
    _z = z;
    return;
}

void Point::translate(const float dx, const float dy, const float dz) {
    _x += dx;
    _y += dy;
    _z += dz;
    return;
}

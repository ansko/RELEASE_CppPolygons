#include "point.hpp"

#include <iostream>

Point::Point() {
    _x = 0.;
    _y = 0.;
    _z = 0.;
}

Point::Point(float x, float y, float z) {
    _x = x;
    _y = y;
    _z = z;
}


float Point::x() {
    return _x;
}

void Point::set_x(float x_to_set) {
    _x = x_to_set;
}

float Point::y() {
    return _y;
}

void Point::set_y(float y_to_set) {
    _y = y_to_set;
}

float Point::z() {
    return _z;
}

void Point::set_z(float z_to_set) {
    _z = z_to_set;
}

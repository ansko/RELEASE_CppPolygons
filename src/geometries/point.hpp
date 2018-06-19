#pragma once

#include <memory>


class Point {
public:
    Point();
    Point(float x, float y, float z);

    float x();             // get x_value
    void set_x(float x_to_set); // set x_value
    float y();             // and so on ...
    void set_y(float y_to_set);
    float z();
    void set_z(float z_to_set);
private:
    float _x, _y, _z;
};

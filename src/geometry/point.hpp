#pragma once

#include <memory>


class Point {
public:
    Point(const float x=0, const float y=0, const float z=0);

    float x() const;
    float y() const;
    float z() const;

    void update(const float x, const float y, const float z);
    void translate(const float dx, const float dy, const float dz);
private:
    float _x, _y, _z;
};

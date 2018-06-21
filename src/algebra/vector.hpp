#pragma once

#include <cmath>
#include <iostream>
#include <memory>
#include <vector>

#include "matrix.hpp"
#include "../geometry/point.hpp"


class Vector : private Matrix {
public:
    Vector(const float x=0, const float y=0, const float z=0);
    Vector(const Matrix &m)
        throw(MyException);
    Vector(const Point& pt_end, const Point &pt_beg=Point(0, 0, 0));

    const Vector operator+(const Vector &other) const;
    const Vector operator-(const Vector &other) const;
    const Vector operator*(const float coefficient) const;
    const Vector operator*(const Matrix &m) const
        throw(MyException);
    const Vector operator/(float coefficient) const
        throw(MyException);

    const float x() const;
    const float y() const;
    const float z() const;
    const float length() const;

    const Vector cross_product(const Vector &other) const;
    const float scalar_product(const Vector &other) const;

    void update(const float x=0, const float y=0, const float z=0);
    void translate(const float dx=0, const float dy=0, const float dz=0);
private:
    Point _pt_beg = Point();
    float _x, _y, _z;
};

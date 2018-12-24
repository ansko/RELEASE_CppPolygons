#pragma once

#include <memory>

#include "point.hpp"

//#include "../algebra/vector.hpp"
//#include "../algebra/matrix.hpp"


class Sphere
{
public:
    Sphere(const double x_c, const double y_c, const double z_c, const double r)
        : x_(x_c), y_(y_c), z_(z_c), r_(r)
    {};

    Sphere(const Point &pt_c, const double r)
        : x_(pt_c.x()), y_(pt_c.y()), z_(pt_c.z()), r_(r)
    {};

    const float x_c() const;
    const float y_c() const;
    const float z_c() const;
    const float r() const;

    const bool crosses_with_sphere(const Sphere &sph);

private:
    double x_, y_, z_, r_;
};

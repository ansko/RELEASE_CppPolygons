#pragma once

#include <memory>

#include "line_segment.hpp"
#include "point.hpp"

#include "../algebra/vector.hpp"
#include "../algebra/matrix.hpp"


class Plane {
public:
    Plane(const float a, const float b, const float c, const float d);
    Plane(const Vector &normal, const Point &pt_on_plane);
    Plane(const Point &pt_0, const Point &pt_1, const Point &pt_2);

    const float a() const;
    const float b() const;
    const float c() const;
    const float d() const;
    const Vector normal() const;
    const Point pt_0() const;
    const Point pt_1() const;
    const Point pt_2() const;
    const Point pt_cross() const;
    const bool is_cross_set() const;
    const bool is_crossed_by_line_segment(const LineSegment &ls);

    void unset_cross();
private:
    float _a, _b, _c, _d;
    Vector _normal;
    Point _pt_0, _pt_1, _pt_2, _pt_cross;
    bool _is_cross_set = false;
};

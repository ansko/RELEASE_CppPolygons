#pragma once

#include <vector>

#include "line_segment.hpp"
#include "plane.hpp"
#include "point.hpp"


class Polygon {
public:
    Polygon();
    Polygon(const std::vector<Point> &vertices);

    const std::vector<Point> vertices() const;
    const Point pt_center() const;

    const bool crosses_other_polygon(const Polygon &other) const;
    //const bool crosses_other_polygon2(const Polygon &other) const;
    const bool contains_point(const Point &pt) const;  
    const int crosses_box(const float Lx, const float Ly=0, const float Lz=0) 
        const;
private:
    std::vector<Point> _vertices;
};

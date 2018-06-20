#pragma once

#include <vector>

#include "line_segment.hpp"
#include "plane.hpp"
#include "point.hpp"


class Polygon {
public:
    Polygon(std::vector<Point> vertices);
    std::vector<Point> vertices();
    bool crosses_other_polygon(std::shared_ptr<Polygon> other_polygon_ptr);
    bool contains_point(std::shared_ptr<Point> pt_ptr);        
    int crosses_box(float box_size_x, float box_size_y=0, float box_size_z=0);
    std::shared_ptr<Point> pt_center_ptr();
private:
    std::vector<Point> _vertices;
};

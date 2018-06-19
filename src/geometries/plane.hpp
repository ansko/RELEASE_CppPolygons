#pragma once

#include <memory>

#include "line_segment.hpp"
#include "point.hpp"
#include "vector.hpp"


#include "../algebra/matrix.hpp"


class Plane {
public:
    Plane(float a, float b, float c, float d);
    Plane(std::shared_ptr<Vector> normal_ptr,
            std::shared_ptr<Point> pt_on_plane_ptr);
    Plane(std::shared_ptr<Point> pt_0_ptr,
            std::shared_ptr<Point> pt_1_ptr,
              std::shared_ptr<Point> pt_2_ptr);

    float a();
    float b();
    float c();
    float d();
    std::shared_ptr<Vector> normal_ptr();
    std::shared_ptr<Point> pt_0_ptr();
    std::shared_ptr<Point> pt_1_ptr();
    std::shared_ptr<Point> pt_2_ptr();
    std::shared_ptr<Point> pt_cross_ptr();
    bool is_cross_set();
    bool is_crossed_by_line_segment(std::shared_ptr<LineSegment> ls_ptr);
private:
    float _a, _b, _c, _d;
    std::shared_ptr<Vector> _normal_ptr;
    std::shared_ptr<Point> _pt_0_ptr = nullptr, _pt_1_ptr = nullptr,
                           _pt_2_ptr = nullptr, _pt_cross_ptr = nullptr;
    bool _is_cross_set = false;
};

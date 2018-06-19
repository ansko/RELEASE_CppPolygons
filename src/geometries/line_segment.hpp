#pragma once

#include <memory>

#include "point.hpp"


class LineSegment {
public:
    LineSegment(std::shared_ptr<Point> pt_beg_ptr,
                std::shared_ptr<Point> pt_end_ptr);

    float x();
    float y();
    float z();
    float length();
    std::shared_ptr<Point> pt_beg_ptr();
    std::shared_ptr<Point> pt_end_ptr();
private:
    std::shared_ptr<Point> _pt_beg_ptr, _pt_end_ptr;
};

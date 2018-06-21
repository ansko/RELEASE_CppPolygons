#pragma once

#include <memory>

#include "point.hpp"


class LineSegment {
public:
    LineSegment(const Point &pt_beg_ptr, const Point &pt_end);

    const float x() const;
    const float y() const;
    const float z() const;
    const float length() const;
    const Point pt_beg() const;
    const Point pt_end() const;
private:
    Point _pt_beg, _pt_end;
};

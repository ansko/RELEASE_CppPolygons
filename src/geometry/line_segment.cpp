#include <cmath>

#include "line_segment.hpp"


LineSegment::LineSegment(const Point &pt_beg, const Point &pt_end) {
    _pt_beg = pt_beg;
    _pt_end = pt_end;
};

const float LineSegment::x() const {
    return _pt_end.x() - _pt_beg.x();
};

const float LineSegment::y() const {
    return _pt_end.y() - _pt_beg.y();
};

const float LineSegment::z() const {
    return _pt_end.z() - _pt_beg.z();
};

const float LineSegment::length() const {
    float dx = x(),
          dy = y(),
          dz = z();
    return sqrt(dx*dx + dy*dy + dz*dz);
};

const Point LineSegment::pt_beg() const {
    return _pt_beg;
};

const Point LineSegment::pt_end() const {
    return _pt_end;
};

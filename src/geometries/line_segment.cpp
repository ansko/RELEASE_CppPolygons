#include <cmath>

#include "line_segment.hpp"


LineSegment::LineSegment(std::shared_ptr<Point> pt_beg_ptr,
                         std::shared_ptr<Point> pt_end_ptr) {
    _pt_beg_ptr = pt_beg_ptr;
    _pt_end_ptr = pt_end_ptr;
};

float LineSegment::x() {
    return _pt_end_ptr->x() - _pt_beg_ptr->x();
};

float LineSegment::y() {
    return _pt_end_ptr->y() - _pt_beg_ptr->y();
};

float LineSegment::z() {
    return _pt_end_ptr->z() - _pt_beg_ptr->z();
};

float LineSegment::length() {
    float dx = this->x(),
          dy = this->y(),
          dz = this->z();
    return sqrt(dx*dx + dy*dy + dz*dz);
};

std::shared_ptr<Point> LineSegment::pt_beg_ptr() {
    return _pt_beg_ptr;
};

std::shared_ptr<Point> LineSegment::pt_end_ptr() {
    return _pt_end_ptr;
};

#include "plane.hpp"


Plane::Plane(float a, float b, float c, float d) {
    _a = a;
    _b = b;
    _c = c;
    _d = d;
};

Plane::Plane(std::shared_ptr<Vector> normal_ptr,
               std::shared_ptr<Point> pt_on_plane_ptr) {
    _normal_ptr = normal_ptr;
    _pt_0_ptr = pt_on_plane_ptr;
    _a = normal_ptr->x();
    _b = normal_ptr->y();
    _c = normal_ptr->z();
    _d = -normal_ptr->x() * pt_on_plane_ptr->x() -
            normal_ptr->y() * pt_on_plane_ptr->y() -
              normal_ptr->z() * pt_on_plane_ptr->z();
};

Plane::Plane(std::shared_ptr<Point> pt_0_ptr,
               std::shared_ptr<Point> pt_1_ptr,
                 std::shared_ptr<Point> pt_2_ptr) {
    _pt_0_ptr = pt_0_ptr;
    _pt_1_ptr = pt_1_ptr;
    _pt_2_ptr = pt_2_ptr;
    if (_pt_0_ptr->x() == _pt_1_ptr->x() && _pt_0_ptr->x() == _pt_2_ptr->x()) {
        _a = 1.f;
        _b = 0.f;
        _c = 0.f;
        _d = -_a;
    }
    if (_pt_0_ptr->y() == _pt_1_ptr->y() && _pt_0_ptr->y() == _pt_2_ptr->y()) {
        _a = 0.f;
        _b = 1.f;
        _c = 0.f;
        _d = -_b;
    }
    if (_pt_0_ptr->z() == _pt_1_ptr->z() && _pt_0_ptr->z() == _pt_2_ptr->z()) {
        _a = 0.f;
        _b = 0.f;
        _c = 1.f;
        _d = -_c;
    }
    _a = (pt_1_ptr->y() - pt_0_ptr->y()) * (pt_2_ptr->z() - pt_0_ptr->z()) -
         (pt_1_ptr->z() - pt_0_ptr->z()) * (pt_2_ptr->y() - pt_0_ptr->y());
    _b = -(pt_1_ptr->x() - pt_0_ptr->x()) * (pt_2_ptr->z() - pt_0_ptr->z()) +
          (pt_2_ptr->x() - pt_0_ptr->x()) * (pt_1_ptr->z() - pt_0_ptr->z());
    _c = (pt_1_ptr->x() - pt_0_ptr->x()) * (pt_2_ptr->y() - pt_0_ptr->y()) -
         (pt_2_ptr->x() - pt_0_ptr->x()) * (pt_1_ptr->y() - pt_0_ptr->y());
    _d = -pt_0_ptr->x() *
            (pt_1_ptr->y() - pt_0_ptr->y()) * (pt_2_ptr->z() - pt_0_ptr->z()) +
          pt_0_ptr->x() *
            (pt_1_ptr->z() - pt_0_ptr->z()) * (pt_2_ptr->y() - pt_0_ptr->y()) +
          pt_0_ptr->y() *
            (pt_1_ptr->x() - pt_0_ptr->x()) * (pt_2_ptr->z() - pt_0_ptr->z()) -
          pt_0_ptr->y() *
            (pt_2_ptr->x() - pt_0_ptr->x()) * (pt_1_ptr->z() - pt_0_ptr->z()) -
          pt_0_ptr->z() *
            (pt_1_ptr->x() - pt_0_ptr->x()) * (pt_2_ptr->y() - pt_0_ptr->y()) +
          pt_0_ptr->z() *
            (pt_2_ptr->x() - pt_0_ptr->x()) * (pt_1_ptr->y() - pt_0_ptr->y());
};


float Plane::a() {
    return _a;
};

float Plane::b() {
    return _b;
};

float Plane::c() {
    return _c;
};

float Plane::d() {
    return _d;
};

std::shared_ptr<Vector> Plane::normal_ptr() {
    return std::make_shared<Vector>(_a, _b, _c);
};

std::shared_ptr<Point> Plane::pt_0_ptr() {
    return _pt_0_ptr;
};

std::shared_ptr<Point> Plane::pt_1_ptr() {
    return _pt_1_ptr;
};

std::shared_ptr<Point> Plane::pt_2_ptr() {
    return _pt_2_ptr;
};

std::shared_ptr<Point> Plane::pt_cross_ptr() {
    if (!_is_cross_set)
        std::cout << "Warning! Cross in not set but is requested\n";
    return _pt_cross_ptr;
};


bool Plane::is_crossed_by_line_segment(std::shared_ptr<LineSegment> ls_ptr) {
    std::shared_ptr<Point> pt_3_ptr = ls_ptr->pt_beg_ptr();
    std::shared_ptr<Point> pt_4_ptr = ls_ptr->pt_end_ptr();
    std::shared_ptr<Vector> v01 = std::make_shared<Vector>(_pt_0_ptr, _pt_1_ptr);
    std::shared_ptr<Vector> v02 = std::make_shared<Vector>(_pt_0_ptr, _pt_2_ptr);
    std::shared_ptr<Vector> v03 = std::make_shared<Vector>(_pt_0_ptr, pt_3_ptr);
    std::shared_ptr<Vector> v04 = std::make_shared<Vector>(_pt_0_ptr, pt_4_ptr);

    std::vector<float> str_0 = {v01->x(), v01->y(), v01->z()};
    std::vector<float> str_1 = {v02->x(), v02->y(), v02->z()};
    std::vector<float> str_2 = {v03->x(), v03->y(), v03->z()};
    std::vector<float> str_3 = {v04->x(), v04->y(), v04->z()};
    std::vector<std::vector<float> > m_1_elements = {str_0, str_1, str_2};
    std::vector<std::vector<float> > m_2_elements = {str_0, str_1, str_3};

    float det1 = Matrix(
      std::make_shared<std::vector<std::vector<float> > >(m_1_elements)).det();
    float det2 = Matrix(
      std::make_shared<std::vector<std::vector<float> > >(m_2_elements)).det();

    if (std::abs(det1) > std::numeric_limits<float>::epsilon() &&
        std::abs(det2) > std::numeric_limits<float>::epsilon()) {
            if (det1 * det2 < 0) {
                Vector vec_to_3 = Vector(std::make_shared<Point>(), pt_3_ptr);
                Vector vec_3_to_4 = Vector(pt_3_ptr, pt_4_ptr);
                float coeff = std::abs(det1) / (std::abs(det1) + std::abs(det2));
                Vector v_to_cross_with_plane =
                    vec_to_3 + vec_3_to_4 * coeff;
                _pt_cross_ptr = std::make_shared<Point>(
                        v_to_cross_with_plane.x(),
                        v_to_cross_with_plane.y(),
                        v_to_cross_with_plane.z());
                _is_cross_set = true;
                return true;
            }
            return false;
    }
    else if (std::abs(det1) > std::numeric_limits<float>::epsilon()) {
        _pt_cross_ptr = pt_4_ptr;
        _is_cross_set = true;
        return true;
    }
    else if (std::abs(det2) > std::numeric_limits<float>::epsilon()) {
        _pt_cross_ptr = pt_3_ptr;
        _is_cross_set = true;
        return true;
    }
    else    // both dets are small -> return true to avoid numerial errors
        return true;
};

bool Plane::is_cross_set() {
    return _is_cross_set;
}

#include "plane.hpp"


Plane::Plane(const float a, const float b, const float c, const float d) {
    _a = a;
    _b = b;
    _c = c;
    _d = d;
};

Plane::Plane(const Vector &normal, const Point &pt_on_plane) {
    _normal = normal;
    _pt_0 = pt_on_plane;
    _a = normal.x();
    _b = normal.y();
    _c = normal.z();
    _d = -_a * pt_on_plane.x() - _b * pt_on_plane.y() - _c * pt_on_plane.z();
};

Plane::Plane(const Point &pt_0, const Point  &pt_1, const Point &pt_2) {
    _pt_0 = pt_0;
    _pt_1 = pt_1;
    _pt_2 = pt_2;
    if (_pt_0.x() == _pt_1.x() && _pt_0.x() == _pt_2.x()) {
        _a = 1;
        _b = 0;
        _c = 0;
        _d = -_a;
    } else if (_pt_0.y() == _pt_1.y() && _pt_0.y() == _pt_2.y()) {
        _a = 0;
        _b = 1;
        _c = 0;
        _d = -_b;
    } else if (_pt_0.z() == _pt_1.z() && _pt_0.z() == _pt_2.z()) {
        _a = 0;
        _b = 0;
        _c = 1;
        _d = -_c;
    }
    _a = (pt_1.y() - pt_0.y()) * (pt_2.z() - pt_0.z()) -
         (pt_1.z() - pt_0.z()) * (pt_2.y() - pt_0.y());
    _b = -(pt_1.x() - pt_0.x()) * (pt_2.z() - pt_0.z()) +
          (pt_2.x() - pt_0.x()) * (pt_1.z() - pt_0.z());
    _c = (pt_1.x() - pt_0.x()) * (pt_2.y() - pt_0.y()) -
         (pt_2.x() - pt_0.x()) * (pt_1.y() - pt_0.y());
    _d = -pt_0.x() * (pt_1.y() - pt_0.y()) * (pt_2.z() - pt_0.z()) +
          pt_0.x() * (pt_1.z() - pt_0.z()) * (pt_2.y() - pt_0.y()) +
          pt_0.y() * (pt_1.x() - pt_0.x()) * (pt_2.z() - pt_0.z()) -
          pt_0.y() * (pt_2.x() - pt_0.x()) * (pt_1.z() - pt_0.z()) -
          pt_0.z() * (pt_1.x() - pt_0.x()) * (pt_2.y() - pt_0.y()) +
          pt_0.z() * (pt_2.x() - pt_0.x()) * (pt_1.y() - pt_0.y());
};


const float Plane::a() const {
    return _a;
};

const float Plane::b() const {
    return _b;
};

const float Plane::c() const {
    return _c;
};

const float Plane::d() const {
    return _d;
};

const Vector Plane::normal() const {
    return Vector(_a, _b, _c);
};

const Point Plane::pt_0() const {
    return _pt_0;
};

const Point Plane::pt_1() const {
    return _pt_1;
};

const Point Plane::pt_2() const {
    return _pt_2;
};

const Point Plane::pt_cross() const {
    if (!_is_cross_set)
        std::cout << "Warning! Cross in not set but is requested\n";
    return _pt_cross;
};


const bool Plane::is_crossed_by_line_segment(const LineSegment &ls) {
    const Point &pt_3 = ls.pt_beg();
    const Point &pt_4 = ls.pt_end();

    const Vector v01(_pt_0, _pt_1);
    const Vector v02(_pt_0, _pt_2);
    const Vector v03(_pt_0, pt_3);
    const Vector v04(_pt_0, pt_4);
    const std::vector<float> str_0 = {v01.x(), v01.y(), v01.z()};
    const std::vector<float> str_1 = {v02.x(), v02.y(), v02.z()};
    const std::vector<std::vector<float> > m_1_elements = {
        str_0, str_1, {v03.x(), v03.y(), v03.z()}
    };
    std::vector<std::vector<float> > m_2_elements = {
        str_0, str_1, {v04.x(), v04.y(), v04.z()}
    };
    const float det1 = Matrix(m_1_elements).det();
    const float det2 = Matrix(m_2_elements).det();

 //   std::cout << "dets: " << det1 << " " << det2 << std::endl;

    if (std::abs(det1) > std::numeric_limits<float>::epsilon() &&
        std::abs(det2) > std::numeric_limits<float>::epsilon()) {
            if (det1 * det2 < 0) {
                Vector vec_to_3(pt_3);
                Vector vec_3_to_4(pt_4, pt_3);
                float coeff = std::abs(det1) / (std::abs(det1) + std::abs(det2));
//                std::cout << "coeff: " << coeff << "\n";
//                std::cout << "to3: " << vec_to_3.x() << " " << vec_to_3.y()
//                                     << " " << vec_to_3.z() << "\n";
//                std::cout << "34: " << vec_3_to_4.x() << " " << vec_3_to_4.y()
//                                     << " " << vec_3_to_4.z() << "\n";
                Vector v_to_cross_with_plane = vec_to_3 + vec_3_to_4 * coeff;

                /*std::cout << "vec_cross: "
                          << v_to_cross_with_plane.x() << " "
                          << v_to_cross_with_plane.y() << " "
                          << v_to_cross_with_plane.z() << std::endl;
*/
                _pt_cross = Point(v_to_cross_with_plane.x(),
                                  v_to_cross_with_plane.y(),
                                  v_to_cross_with_plane.z());
                _is_cross_set = true;
                return true;
            }
            return false;
    } else if (std::abs(det1) > std::numeric_limits<float>::epsilon()) {
        _pt_cross = pt_4;
        _is_cross_set = true;
        return true;
    } else if (std::abs(det2) > std::numeric_limits<float>::epsilon()) {
        _pt_cross = pt_3;
        _is_cross_set = true;
        return true;
    } else
        return true; // small determinnat value are interpreted as intersection
};

const bool Plane::is_cross_set() const {
    return _is_cross_set;
}

void Plane::unset_cross() {
    _is_cross_set = false;
    return;
}

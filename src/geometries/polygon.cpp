#include "polygon.hpp"


Polygon::Polygon(std::vector<Point> vertices) {
    _vertices = vertices;
};

std::vector<Point> Polygon::vertices() {
    return _vertices;
};

std::shared_ptr<Point> Polygon::pt_center_ptr() {
    Vector center;
    for (auto vertex : _vertices)
        center = center + Vector(vertex.x(), vertex.y(), vertex.z());
    return std::make_shared<Point>(
               center.x() / _vertices.size(),
               center.y() / _vertices.size(),
               center.z() / _vertices.size());
}

bool Polygon::crosses_other_polygon(std::shared_ptr<Polygon> other_polygon_ptr) {
    Plane plane = Plane(
        std::make_shared<Point>(_vertices[0]),
        std::make_shared<Point>(_vertices[1]),
        std::make_shared<Point>(_vertices[2]));
    std::vector<Point> other_vertices = other_polygon_ptr->vertices();
    Plane other_plane = Plane(
        std::make_shared<Point>(other_vertices[0]),
        std::make_shared<Point>(other_vertices[1]),
        std::make_shared<Point>(other_vertices[2]));
    bool flag_self_crosses_other = false;
    bool flag_other_crosses_self = false;
    // any of this edges crosses other
    for (int i = 0; i < _vertices.size(); ++i) {
        Point pt_beg = _vertices[i];
        Point pt_end;
        if (i != _vertices.size() - 1)
            pt_end = _vertices[i + 1];
        else
            pt_end = _vertices[0];
        std::shared_ptr<LineSegment> ls_ptr =
            std::make_shared<LineSegment>(std::make_shared<Point>(pt_beg),
                                          std::make_shared<Point>(pt_end));
        if (other_plane.is_crossed_by_line_segment(ls_ptr)) {
            if (!other_plane.is_cross_set())
                std::cout << "Error! cross_pt is not set but requested\n";
            std::shared_ptr<Point> pt_cross_ptr = other_plane.pt_cross_ptr();
            if (other_polygon_ptr->contains_point(pt_cross_ptr)) {
                flag_self_crosses_other = true;
                break;
            }
        }
    }
    if (flag_self_crosses_other == true) {
            return true;
    }
    for (int i = 0; i < other_vertices.size(); ++i) {
        Point pt_beg = other_vertices[i];
        Point pt_end;
        if (i != other_vertices.size() - 1)
            pt_end = other_vertices[i + 1];
        else
            pt_end = other_vertices[0];
        std::shared_ptr<LineSegment> ls_ptr =
            std::make_shared<LineSegment>(std::make_shared<Point>(pt_beg),
                                          std::make_shared<Point>(pt_end));
        if (plane.is_crossed_by_line_segment(ls_ptr)) {
            std::shared_ptr<Point> pt_cross_ptr = plane.pt_cross_ptr();
            if (this->contains_point(pt_cross_ptr)) {
                flag_other_crosses_self = true;
                break;
            }
        }
    }
    if (flag_other_crosses_self == true)
        return true;
    return false;
};

bool Polygon::contains_point(std::shared_ptr<Point> pt_ptr) {
    int s = _vertices.size();
    bool flag_contains_point = false;
    Vector center, other_center;
    for (auto vertex : _vertices)
        center = center + Vector(vertex.x(), vertex.y(), vertex.z());
    center = center / s;
    Point pt_0, pt_1, pt_2;
    for(int i = 0; i < s; ++i) {
        if (i == s - 1) {
            pt_0 = _vertices[i];
            pt_1 = _vertices[0];
            pt_2 = _vertices[1];
        }
        else if (i == s - 2) {
            pt_0 = _vertices[i];
            pt_1 = _vertices[i + 1];
            pt_2 = _vertices[0];
        }
        else {
            pt_0 = _vertices[i];
            pt_1 = _vertices[i + 1];
            pt_2 = _vertices[i + 2];
        }
        Plane pl = Plane(std::make_shared<Point>(pt_0),
                         std::make_shared<Point>(pt_1),
                         std::make_shared<Point>(pt_2));
        Vector v02 = Vector(std::make_shared<Point>(pt_0),
                            std::make_shared<Point>(pt_2));
        Vector v01 = Vector(std::make_shared<Point>(pt_0),
                            std::make_shared<Point>(pt_1));
        Vector v0pt = Vector(std::make_shared<Point>(pt_0),
                             pt_ptr);
        float tmp = v02.cross_product_ptr(
                           std::make_shared<Vector>(v01))
                       ->scalar_product(v01.cross_product_ptr(
                           std::make_shared<Vector>(v0pt)));
        if (tmp > std::numeric_limits<float>::epsilon())
            flag_contains_point = true;
    }
    return flag_contains_point;
}; 

int Polygon::crosses_box(float box_size_x, float box_size_y, float box_size_z) {
    if (std::abs(box_size_y) < std::numeric_limits<float>::epsilon())
        box_size_y = box_size_x;
    if (std::abs(box_size_z) < std::numeric_limits<float>::epsilon())
        box_size_z = box_size_x;
    for (int i = 0; i < _vertices.size(); ++i) {
        auto vertex = _vertices[i];
        if (vertex.x() < 0)
            return 1;
        if(vertex.x() > box_size_x)
            return 2;
        if (vertex.y() < 0)
            return 3;
        if (vertex.y() > box_size_y)
            return 4;
        if (vertex.z() < 0)
            return 5;
        if (vertex.z() > box_size_z)
            return 6;
    }
    return false;
};

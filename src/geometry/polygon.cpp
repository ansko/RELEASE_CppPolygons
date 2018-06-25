#include "polygon.hpp"


Polygon::Polygon() {
}

Polygon::Polygon(const std::vector<Point> &vertices) {
    _vertices = vertices;
};

const std::vector<Point> Polygon::vertices() const {
    return _vertices;
};

const Point Polygon::pt_center() const {
    const int s = _vertices.size();
    float x=0, y=0, z=0;
    for (auto &vertex : _vertices) {
        x += vertex.x();
        y += vertex.y();
        z += vertex.z();
    }
    return Point(x / s, y / s, z / s);
}
/*
const bool Polygon::crosses_other_polygon(const Polygon &other) const {
    Plane plane(_vertices[0], _vertices[1], _vertices[2]);
    const std::vector<Point> other_vertices = other.vertices();
    Plane other_plane = Plane(other_vertices[0], other_vertices[1],
                                    other_vertices[2]);
    // any of this edges crosses other
    for (int i = 0; i < _vertices.size(); ++i) {
        const Point &pt_beg = _vertices[i];
        Point pt_end;
        if (i != _vertices.size() - 1)
            pt_end = _vertices[i + 1];
        else
            pt_end = _vertices[0];
        const LineSegment ls(pt_beg, pt_end);
        if (other_plane.is_crossed_by_line_segment(ls)) {
            if (!other_plane.is_cross_set())
                std::cout << "Error! cross_pt is not set but requested\n";
            const Point pt_cross = other_plane.pt_cross();
            if (other.contains_point(pt_cross))
                return true;
        }
    }
    for (int i = 0; i < other_vertices.size(); ++i) {
        const Point &pt_beg = other_vertices[i];
        Point pt_end;
        if (i != other_vertices.size() - 1)
            pt_end = other_vertices[i + 1];
        else
            pt_end = other_vertices[0];
        const LineSegment ls(pt_beg, pt_end);
        if (plane.is_crossed_by_line_segment(ls)) {
            const Point pt_cross = plane.pt_cross();
            if (this->contains_point(pt_cross))
                return true;
        }
    }
    return false;
};
*/
const bool Polygon::crosses_other_polygon(const Polygon &other) const {
    Plane pl(other.vertices()[0], other.vertices()[1], other.vertices()[2]);
    for (int idx = 0; idx < _vertices.size(); ++idx) {
        Point pt1 = _vertices[idx],
              pt2 = (idx > 0) ? _vertices[idx - 1]
                              : _vertices[_vertices.size() - 1];
        LineSegment ls(pt1, pt2);
        bool cross = pl.is_crossed_by_line_segment(ls);
        if (cross)
            if(other.contains_point(pl.pt_cross()))
                return true;
    }
    return false;
}

const bool Polygon::contains_point(const Point &pt) const {
    int s = _vertices.size();
    for(int i = 0; i < s; ++i) {
        Point pt_0, pt_1, pt_2;
        if (i == s - 1) {
            pt_0 = _vertices[i];
            pt_1 = _vertices[0];
            pt_2 = _vertices[1];
        } else if (i == s - 2) {
            pt_0 = _vertices[i];
            pt_1 = _vertices[i + 1];
            pt_2 = _vertices[0];
        } else {
            pt_0 = _vertices[i];
            pt_1 = _vertices[i + 1];
            pt_2 = _vertices[i + 2];
        }
        float area012 =
            Vector(pt_0, pt_1).cross_product(Vector(pt_0, pt_2)).length();
        float area01 =
            Vector(pt, pt_0).cross_product(Vector(pt, pt_1)).length();
        float area02 =
            Vector(pt, pt_2).cross_product(Vector(pt, pt_0)).length();
        float area12 =
            Vector(pt, pt_1).cross_product(Vector(pt, pt_2)).length();
        float areas_difference = std::fabs(area012 - (area01+area02+area12));
        if (areas_difference < std::numeric_limits<float>::epsilon())
            return true;
    }
    return false;
}; 

const int Polygon::crosses_box(
    const float Lx, const float Ly, const float Lz) const {
        int flag = 0;
        for (int i = 0; i < _vertices.size(); ++i) {
            auto vertex = _vertices[i];
            if (vertex.x() < 0)
                flag += 1;
            if (vertex.x() > Lx)
                flag += 2;
            if (vertex.y() < 0)
                flag += 3;
            if (Ly > 0 && vertex.y() > Ly)
                flag += 4;
            if (vertex.z() < 0)
                flag += 5;
            if (Lz > 0 && vertex.z() > Lz)
                flag += 6;
        }
        return flag;
};

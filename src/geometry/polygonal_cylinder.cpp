#include "polygonal_cylinder.hpp"


PolygonalCylinder::PolygonalCylinder(const PolygonalPolyhedron &polyhedron) {
    _vertices_number = polyhedron.polygons()[0].vertices().size();
    _top_facet = polyhedron.polygons()[0];
    _bot_facet = polyhedron.polygons()[1];
    for (int idx = 2; idx < _vertices_number; ++idx)
        _facets.push_back(polyhedron.polygons()[idx]);
    _outer_radius = Vector(_top_facet.pt_center(), _top_facet.vertices()[0])
                   .length();
    _thickness = Vector(_top_facet.vertices()[0], _bot_facet.vertices()[0])
                 .length();
    _central_angle = 2 * M_PI / _vertices_number;
    _edge_length = _outer_radius * 2 * sin(M_PI / _vertices_number);
    _inner_radius = _edge_length / 2 / tan(M_PI / _vertices_number);
    _polygons = {_top_facet, _bot_facet};
    for (auto &facet : _facets)
        _polygons.push_back(facet);
    _top_facet_center.update(0, 0, _thickness / 2);
    _bot_facet_center.update(0, 0, -_thickness / 2);
}

PolygonalCylinder::PolygonalCylinder(const int vertices_number,
    const float thickness, const float outer_radius) {
        _outer_radius = outer_radius;
        _vertices_number = vertices_number;
        _thickness = thickness;
        _central_angle = 2 * M_PI / vertices_number;
        _edge_length = _outer_radius * 2 * sin(M_PI / _vertices_number);
        _inner_radius = _edge_length / 2 / tan(M_PI / _vertices_number);
        std::vector<Point> central_vertices, top_vertices, bot_vertices;
        // central_vertices == vertices of side facets
        // top_vertices == vertices of top faccet
        // bot_vertices == vertices on bottom facet
        for(int idx = 0; idx < vertices_number; ++idx) {
            central_vertices.clear();
            top_vertices.push_back(Point(_outer_radius * cos(_central_angle * idx),
                                         _outer_radius * sin(_central_angle * idx),
                                         _thickness / 2));
            bot_vertices.push_back(Point(_outer_radius * cos(_central_angle * idx),
                                         _outer_radius * sin(_central_angle * idx),
                                         -_thickness / 2));
            central_vertices.push_back(
                Point(_outer_radius * cos(_central_angle * (idx - 1)),
                      _outer_radius * sin(_central_angle * (idx - 1)),
                     _thickness / 2));
            central_vertices.push_back(
                Point(_outer_radius * cos(_central_angle * idx),
                      _outer_radius * sin(_central_angle * idx),
                      _thickness / 2));
            central_vertices.push_back(
                Point(_outer_radius * cos(_central_angle * idx),
                      _outer_radius * sin(_central_angle * idx),
                      -_thickness / 2));
            central_vertices.push_back(
                Point(_outer_radius * cos(_central_angle * (idx - 1)),
                      _outer_radius * sin(_central_angle * (idx - 1)),
                     -_thickness / 2));
            _facets.push_back(Polygon(central_vertices));
        }
        _top_facet = Polygon(top_vertices);
        _bot_facet = Polygon(bot_vertices);
        _polygons = {_top_facet, _bot_facet};
        for (auto &facet : _facets)
            _polygons.push_back(facet);
        _top_facet_center.update(0, 0, thickness / 2);
        _bot_facet_center.update(0, 0, -thickness / 2);
};


PolygonalCylinder::PolygonalCylinder(
    const Polygon &top_facet, const Polygon &bottom_facet) {
        const std::vector<Point> &top_vertices = top_facet.vertices();
        const std::vector<Point> &bot_vertices = bottom_facet.vertices();
        _vertices_number = top_vertices.size();
        for (int vertex_number = 0;
               vertex_number < _vertices_number;
                 ++ vertex_number) {
            std::vector<Point> side_vertices;
            int other_vertex_number = vertex_number - 1;
            if (vertex_number == 0)
                other_vertex_number = _vertices_number - 1;
            side_vertices.push_back(top_vertices[vertex_number]);
            side_vertices.push_back(top_vertices[other_vertex_number]);
            side_vertices.push_back(bot_vertices[other_vertex_number]);
            side_vertices.push_back(bot_vertices[vertex_number]);
            _facets.push_back(Polygon(side_vertices));
        }
        Point pt1 = top_vertices[0],
              pt2 = top_vertices[1],
              pt3 = bot_vertices[0];
        _edge_length = Vector(pt1, pt2).length();
        _inner_radius = _edge_length / 2 / tan(M_PI / _vertices_number);
        _outer_radius = _edge_length / 2 / sin(M_PI / _vertices_number);
        _thickness = Vector(pt1, pt3).length();
        float tfc_x = 0, tfc_y = 0, tfc_z = 0,
              bfc_x = 0, bfc_y = 0, bfc_z = 0;
        for (int vertex_number = 0;
               vertex_number < _vertices_number;
                 ++vertex_number) {
            tfc_x += top_vertices[vertex_number].x();
            tfc_y += top_vertices[vertex_number].y();
            tfc_z += top_vertices[vertex_number].z();
            bfc_x += bot_vertices[vertex_number].x();
            bfc_y += bot_vertices[vertex_number].y();
            bfc_z += bot_vertices[vertex_number].z();
        }
        _top_facet_center.update(tfc_x / _vertices_number,
            tfc_y / _vertices_number, tfc_z / _vertices_number);
        _bot_facet_center.update(bfc_x / _vertices_number,
            bfc_y / _vertices_number, bfc_z / _vertices_number);
        _top_facet = top_facet;
        _bot_facet = bottom_facet;
}


const Point PolygonalCylinder::top_facet_center() const {
    return this->_top_facet_center;
}

const Point PolygonalCylinder::bot_facet_center() const {
    return this->_bot_facet_center;
}


const std::vector<Polygon> PolygonalCylinder::facets() const {
    return _facets;
};

const Polygon PolygonalCylinder::top_facet() const {
    return _top_facet;
};

const Polygon PolygonalCylinder::bot_facet() const {
    return _bot_facet;
};

const float PolygonalCylinder::outer_radius() const {
    return _outer_radius;
}


const bool PolygonalCylinder::crosses_other_polygonal_cylinder(
    const PolygonalCylinder &other) const {
        Point tc = _top_facet_center,
              bc = _bot_facet_center,
              other_tc = other.top_facet().pt_center(),
              other_bc = other.bot_facet().pt_center();
        Vector vc = Vector(tc.x() + bc.x(), tc.y() + bc.y(), tc.z() + bc.z()) / 2;
        Point c(vc.x(), vc.y(), vc.z());
        Vector other_vc(other_tc.x() + other_bc.x(),
                        other_tc.y() + other_bc.y(),
                        other_tc.z() + other_bc.z());
        other_vc = other_vc / 2;
        Point other_c(other_vc.x(), other_vc.y(), other_vc.z());
        float centers_distance = Vector(c, other_c).length();
        float very_close_distance = std::min(_thickness, 2 * _inner_radius);
        float very_far_distance = 2 * sqrt(_inner_radius * _inner_radius +
                                           _thickness * _thickness/4);
        if (centers_distance > very_far_distance)
            return false;
        else if (centers_distance < very_close_distance)
            return true;
        // starting precise checking
        // if main axes of the cylinders lie on far lines
        // http://en.wikipedia.org/wiki/Skew_lines#Distance_between_two_skew_lines
        // x = x1 + td1 and x = x2 + td2
        // d = |(d1 x d2) / |d1 x d2| * (x1 - x2)|
        Point x1 = bc, x2 = other_bc;
        Vector d1(x1, tc);
        Vector d2(x2, other_tc);
        Vector n = d1.cross_product(d2);
        n = n / n.length();
        Vector dx(x1, x2);
        float d = fabs(n.scalar_product(dx));
        if (d > 2 * _outer_radius)
            return false;
        std::vector<Polygon> polys = {_top_facet, _bot_facet},
                             other_polys = {other.top_facet(), other.bot_facet()};
        for (auto& facet : _facets)
            polys.push_back(facet);
        for (auto& facet : other.facets())
            other_polys.push_back(facet);
        Point this_center(tc.x()/2 + bc.x()/2,
                          tc.y()/2 + bc.y()/2,
                          tc.z()/2 + bc.z()/2);
       Vector vec_this_center(c);
       Vector vec_other_center(other_c);
       int polys_size = polys.size();
       for (int poly_idx = 0; poly_idx < polys_size; ++poly_idx) {
           auto poly = polys[poly_idx];
           Point poly_c = poly.pt_center();
           Vector vec_this_center_poly_center(c, poly_c);
           std::vector<Point> pts; // vertices of poly (this->polygons[poly_idx])
                                   // polygons = top + bot + facets
           for (auto vertex : poly.vertices()) {
               Vector vec_this_center_vertex(c, vertex);
               Vector vec_vertex = vec_this_center + vec_this_center_vertex;
               vertex.update(vec_vertex.x(), vec_vertex.y(), vec_vertex.z());
               pts.push_back(vertex);
           }
           poly = Polygon(pts);
           int other_polys_size = other_polys.size();
           for (int ohter_poly_idx = 0;
                  ohter_poly_idx < other_polys_size;
                    ++ohter_poly_idx) {
               auto other_poly = other_polys[ohter_poly_idx];
               Point other_poly_c = other_poly.pt_center();
               Vector other_center_poly_center(other_c, other_poly_c);
               std::vector<Point> other_pts; // vertices of other's poly
                                          // (other_ptr->polygons[other_poly_idx])
                                          // polygons = top + bot + facets
               for (auto vertex : other_poly.vertices()) {
                   Vector vec_other_center_vertex(other_c, vertex);
                   Vector vec_vertex = vec_other_center + vec_other_center_vertex;
                   vertex.update(vec_vertex.x(), vec_vertex.y(), vec_vertex.z());
                   other_pts.push_back(vertex);
               }
               other_poly = Polygon(other_pts);
               if (poly.crosses_other_polygon(other_poly))
                   return true;
           }
       }
       return false;
}

const int PolygonalCylinder::crosses_box(const float Lx,
    const float Ly, const float Lz) const  {
        std::vector<Polygon> polygons = {_top_facet, _bot_facet};
        for(int i = 0; i < 2; i++) {
            int a = polygons[i].crosses_box(Lx, Ly, Lz);
            if (a > 0)
                return a;
        }
        return false;
};


void PolygonalCylinder::change_by_matrix(const Matrix &m) {
    std::vector<Polygon> polygons = {_top_facet, _bot_facet};
    for (auto& facet : _facets)
        polygons.push_back(facet);
    for (int i = 0; i < polygons.size(); ++i) {
        std::vector<Point> pts;
        for (auto& vertex : polygons[i].vertices()) {
            Vector v(vertex.x(), vertex.y(), vertex.z());
            v = v * m; 
            pts.push_back(Point(v.x(), v.y(), v.z()));
        }
        polygons[i] = Polygon(pts);
    }
    _top_facet = polygons[0];
    _bot_facet = polygons[1];
    _facets.clear();
    for (int i = 2; i < polygons.size(); ++i)
        _facets.push_back(polygons[i]);
    Vector v(_top_facet_center.x(),
             _top_facet_center.y(),
             _top_facet_center.z());
    v = v * m; 
    _top_facet_center.update(v.x(), v.y(), v.z());
    v = Vector(_bot_facet_center.x(),
               _bot_facet_center.y(),
               _bot_facet_center.z());
    v = v * m;
    _bot_facet_center.update(v.x(), v.y(), v.z());
};

void PolygonalCylinder::translate(const float dx, const float dy, const float dz) {
    std::vector<std::vector<float> > elements = {
        {1, 0, 0, 0},
        {0, 1, 0, 0},
        {0, 0, 1, 0},
        {dx, dy, dz, 1}
    };
    this->change_by_matrix(Matrix(elements));
}

void PolygonalCylinder::rotate_around_x(float angle) {
    std::vector<std::vector<float> > elements = {
        {1, 0, 0, 0},
        {0, (float)cos(angle), -(float)sin(angle), 0},
        {0, (float)sin(angle), (float)cos(angle), 0},
        {0, 0, 0, 1}
    };
    this->change_by_matrix(Matrix(elements));
};

void PolygonalCylinder::rotate_around_y(float angle) {
    std::vector<std::vector<float> > elements = {
        {(float)cos(angle), 0, (float)sin(angle), 0},
        {0, 1, 0, 0},
        {-(float)sin(angle), 0, (float)cos(angle), 0},
        {0, 0, 0, 1}
    };
    this->change_by_matrix(Matrix(elements));
};

void PolygonalCylinder::rotate_around_z(float angle) {
    std::vector<std::vector<float> > elements = {
        {(float)cos(angle), -(float)sin(angle), 0, 0},
        {(float)sin(angle), (float)cos(angle), 0, 0},
        {0, 0, 1, 0},
        {0, 0, 0, 1}
    };
    this->change_by_matrix(Matrix(elements));
};

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

const std::vector<Polygon> PolygonalCylinder::all_polygons() const {
    std::vector<Polygon> all_polygons = _facets;
    all_polygons.push_back(_top_facet);
    all_polygons.push_back(_bot_facet);
    return all_polygons;
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
        std::vector<Polygon> polys = this->all_polygons(),
                       other_polys = other.all_polygons();
        for (Polygon &poly : polys)
            for (Polygon &other : other_polys)
                if(poly.crosses_other_polygon(other) || // <- what is this???
                                                        //    idk why but should be
                   other.crosses_other_polygon(poly))
                    return true;
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
        return 0;
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

#include "polygonal_cylinder.hpp"


PolygonalCylinder::PolygonalCylinder(PolygonalPolyhedron polyhedron) {
    _vertices_number = polyhedron.polygon_ptrs()[0]->vertices().size();
    _top_facet_ptr = polyhedron.polygon_ptrs()[0];
    _bot_facet_ptr = polyhedron.polygon_ptrs()[1];
    for (int idx = 2; idx < _vertices_number; ++idx)
        _facets.push_back(*polyhedron.polygon_ptrs()[idx]);
    _outer_radius = Vector(_top_facet_ptr->pt_center_ptr(),
                           std::make_shared<Point>(
                               _top_facet_ptr->vertices()[0])).length();
    _thickness = Vector(std::make_shared<Point>(
                               _top_facet_ptr->vertices()[0]),
                        std::make_shared<Point>(
                               _bot_facet_ptr->vertices()[0])).length();
    _central_angle = 2 * M_PI / _vertices_number;
    _edge_length = _outer_radius * 2 * sin(M_PI / _vertices_number);
    _polygon_ptrs = {_top_facet_ptr, _bot_facet_ptr};
    for (auto facet : _facets)
        _polygon_ptrs.push_back(std::make_shared<Polygon>(facet));
    _top_facet_center_ptr = std::make_shared<Point>(0, 0, _thickness / 2);
    _bot_facet_center_ptr = std::make_shared<Point>(0, 0, -_thickness / 2);
}

PolygonalCylinder::PolygonalCylinder(
    int vertices_number,
    float thickness,
    float outer_radius
    ) {
    _outer_radius = outer_radius;
    _vertices_number = vertices_number;
    _thickness = thickness;
    _central_angle = 2 * M_PI / vertices_number;
    _edge_length = _outer_radius * 2 * sin(M_PI / _vertices_number);

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
        central_vertices.push_back(Point(_outer_radius * cos(_central_angle * idx),
                                         _outer_radius * sin(_central_angle * idx),
                                         _thickness / 2));
        central_vertices.push_back(Point(_outer_radius * cos(_central_angle * idx),
                                         _outer_radius * sin(_central_angle * idx),
                                         -_thickness / 2));
        central_vertices.push_back(
            Point(_outer_radius * cos(_central_angle * (idx - 1)),
                  _outer_radius * sin(_central_angle * (idx - 1)),
                 -_thickness / 2));
        _facets.push_back(Polygon(central_vertices));
    }
    _top_facet_ptr = std::make_shared<Polygon>(top_vertices);
    _bot_facet_ptr = std::make_shared<Polygon>(bot_vertices);
    _polygon_ptrs = {_top_facet_ptr, _bot_facet_ptr};
    for (auto facet : _facets)
        _polygon_ptrs.push_back(std::make_shared<Polygon>(facet));
    _top_facet_center_ptr = std::make_shared<Point>(0, 0, thickness / 2);
    _bot_facet_center_ptr = std::make_shared<Point>(0, 0, -thickness / 2);
};


PolygonalCylinder::PolygonalCylinder(
        std::shared_ptr<Polygon> top_facet_ptr,
        std::shared_ptr<Polygon> bottom_facet_ptr) {
    std::shared_ptr<std::vector<Point> > top_vertices_ptr =
        std::make_shared<std::vector<Point> >(top_facet_ptr->vertices());
    std::shared_ptr<std::vector<Point> > bottom_vertices_ptr =
        std::make_shared<std::vector<Point> >(bottom_facet_ptr->vertices());
    _vertices_number = top_facet_ptr->vertices().size();
    for (int vertex_number = 0;
           vertex_number < _vertices_number;
             ++ vertex_number) {
        std::vector<Point> side_vertices;
        int other_vertex_number = vertex_number - 1;
        if (vertex_number == 0)
            other_vertex_number = _vertices_number - 1;
        side_vertices.push_back(top_facet_ptr->vertices()[vertex_number]);
        side_vertices.push_back(top_facet_ptr->vertices()[other_vertex_number]);
        side_vertices.push_back(bottom_facet_ptr->vertices()[other_vertex_number]);
        side_vertices.push_back(bottom_facet_ptr->vertices()[vertex_number]);
        _facets.push_back(Polygon(side_vertices));
    }
    Point pt1 = top_facet_ptr->vertices()[0],
          pt2 = top_facet_ptr->vertices()[1],
          pt3 = bottom_facet_ptr->vertices()[0];
    _edge_length = Vector(
        std::make_shared<Point>(pt1), std::make_shared<Point>(pt2)).length();
    _outer_radius = _edge_length / 2 / sin(M_PI / _vertices_number);
    _thickness = Vector(
       std::make_shared<Point>(pt1), std::make_shared<Point>(pt3)).length();
    float tfc_x = 0, tfc_y = 0, tfc_z = 0,
          bfc_x = 0, bfc_y = 0, bfc_z = 0;
    for (int vertex_number = 0;
           vertex_number < _vertices_number;
             ++vertex_number) {
        tfc_x += top_facet_ptr->vertices()[vertex_number].x();
        tfc_y += top_facet_ptr->vertices()[vertex_number].y();
        tfc_z += top_facet_ptr->vertices()[vertex_number].z();
        bfc_x += bottom_facet_ptr->vertices()[vertex_number].x();
        bfc_y += bottom_facet_ptr->vertices()[vertex_number].y();
        bfc_z += bottom_facet_ptr->vertices()[vertex_number].z();
    }
    _top_facet_center_ptr = std::make_shared<Point>(
        tfc_x / _vertices_number,
        tfc_y / _vertices_number,
        tfc_z / _vertices_number);
    _bot_facet_center_ptr = std::make_shared<Point>(
        bfc_x / _vertices_number,
        bfc_y / _vertices_number,
        bfc_z / _vertices_number);
    _top_facet_ptr = top_facet_ptr;
    _bot_facet_ptr = bottom_facet_ptr;
}


std::shared_ptr<Point> PolygonalCylinder::top_facet_center_ptr() {
    return this->_top_facet_center_ptr;
}

std::shared_ptr<Point> PolygonalCylinder::bot_facet_center_ptr() {
    return this->_bot_facet_center_ptr;
}


std::vector<Polygon> PolygonalCylinder::facets() {
    return _facets;
};

std::shared_ptr<Polygon> PolygonalCylinder::top_facet_ptr() {
    return _top_facet_ptr;
};

std::shared_ptr<Polygon> PolygonalCylinder::bot_facet_ptr() {
    return _bot_facet_ptr;
};

float PolygonalCylinder::get_outer_radius() {
    return _outer_radius;
}

bool PolygonalCylinder::crosses_other_polygonal_cylinder(
        std::shared_ptr<PolygonalCylinder> other_polygonal_cylinder_ptr) {
    _inner_radius = _edge_length / 2 / tan(M_PI / _vertices_number);
    std::shared_ptr<Point> tc_ptr = _top_facet_ptr->pt_center_ptr();
    std::shared_ptr<Point> bc_ptr = _bot_facet_ptr->pt_center_ptr();
    std::shared_ptr<Point> other_tc_ptr = other_polygonal_cylinder_ptr
        ->top_facet_ptr()->pt_center_ptr();
    std::shared_ptr<Point> other_bc_ptr = other_polygonal_cylinder_ptr
        ->bot_facet_ptr()->pt_center_ptr();
    Vector vc = Vector(tc_ptr->x() + bc_ptr->x(),
                       tc_ptr->y() + bc_ptr->y(),
                       tc_ptr->z() + bc_ptr->z()) / 2;
    std::shared_ptr<Point> c_ptr = std::make_shared<Point>(vc.x(), vc.y(), vc.z());
    Vector other_vc = Vector(other_tc_ptr->x() + other_bc_ptr->x(),
                             other_tc_ptr->y() + other_bc_ptr->y(),
                             other_tc_ptr->z() + other_bc_ptr->z()) / 2;
    std::shared_ptr<Point> other_c_ptr = std::make_shared<Point>(
        other_vc.x(), other_vc.y(), other_vc.z());
    float centers_distance = Vector(c_ptr, other_c_ptr).length();
    float very_close_distance = std::min(_thickness, 2 * _inner_radius);
    float very_far_distance = 2 * sqrt(_inner_radius*_inner_radius +
                                       _thickness*_thickness/4);
    if (centers_distance > very_far_distance)
        return false;
    else if (centers_distance < very_close_distance)
        return true;
    // starting precise checking
    // if main axes of the cylinders lie on far lines
    // http://en.wikipedia.org/wiki/Skew_lines#Distance_between_two_skew_lines
    // x = x1 + td1 and x = x2 + td2
    // d = |(d1 x d2) / |d1 x d2| * (x1 - x2)|
    std::shared_ptr<Point> x1_ptr = _bot_facet_ptr->pt_center_ptr();
    Vector d1(_bot_facet_ptr->pt_center_ptr(), _top_facet_ptr->pt_center_ptr());
    std::shared_ptr<Point> x2_ptr = other_polygonal_cylinder_ptr
        ->bot_facet_ptr()->pt_center_ptr();
    Vector d2(other_polygonal_cylinder_ptr->bot_facet_ptr()->pt_center_ptr(),
              other_polygonal_cylinder_ptr->top_facet_ptr()->pt_center_ptr());
    std::shared_ptr<Vector> n_ptr =
        d1.cross_product_ptr(std::make_shared<Vector>(d2));
    (*n_ptr) = (*n_ptr) / n_ptr->length();
    Vector dx(x1_ptr, x2_ptr);
    float d = fabs(n_ptr->scalar_product(std::make_shared<Vector>(dx)));
    if (d > 2 * _outer_radius)
        return false;
    std::vector<Polygon> polys, other_polys;
    polys.push_back(*_top_facet_ptr);
    polys.push_back(*_bot_facet_ptr);
    for (auto& facet : _facets)
        polys.push_back(facet);
    other_polys.push_back(*other_polygonal_cylinder_ptr->top_facet_ptr());
    other_polys.push_back(*other_polygonal_cylinder_ptr->bot_facet_ptr());
    for (auto& facet : other_polygonal_cylinder_ptr->facets())
        other_polys.push_back(facet);
    Point this_center(tc_ptr->x() / 2 + bc_ptr->x() / 2,
                      tc_ptr->y() / 2 + bc_ptr->y() / 2,
                      tc_ptr->z() / 2 + bc_ptr->z() / 2);
    Vector vec_this_center = Vector(this_center.x(),
                                       this_center.y(),
                                       this_center.z());
    Point other_center(other_tc_ptr->x() / 2 + other_bc_ptr->x() / 2,
                       other_tc_ptr->y() / 2 + other_bc_ptr->y() / 2,
                       other_tc_ptr->z() / 2 + other_bc_ptr->z() / 2);
    Vector vec_other_center = Vector(other_center.x(),
                                        other_center.y(),
                                        other_center.z());
    int polys_size = polys.size();
    for (int poly_idx = 0; poly_idx < polys_size; ++poly_idx) {
        auto poly = polys[poly_idx];
        std::shared_ptr<Point> poly_center_ptr = poly.pt_center_ptr();
        Vector vec_this_center_poly_center = Vector(
            std::make_shared<Point>(this_center), poly_center_ptr);
        std::vector<Point> pts; // vertices of poly (this->polygons[poly_idx])
                                // polygons = top + bot + facets
        for (auto& vertex : poly.vertices()) {
            Vector vec_this_center_vertex = Vector(
                std::make_shared<Point>(this_center),
                std::make_shared<Point>(vertex));
            Vector vec_vertex = vec_this_center + vec_this_center_vertex;
            vertex = Point(vec_vertex.x(), vec_vertex.y(), vec_vertex.z());
            pts.push_back(vertex);
        }
        poly = Polygon(pts);
        int other_polys_size = other_polys.size();
        for (int ohter_poly_idx = 0;
               ohter_poly_idx < other_polys_size;
                 ++ohter_poly_idx) {
            auto other_poly = other_polys[ohter_poly_idx];
            Point other_poly_center = *other_poly.pt_center_ptr();
            Vector other_center_poly_center = Vector(
                std::make_shared<Point>(other_center),
                std::make_shared<Point>(other_poly_center));
            std::vector<Point> other_pts; // vertices of other's poly
                                          // (other_ptr->polygons[other_poly_idx])
                                          // polygons = top + bot + facets
            for (auto& vertex : other_poly.vertices()) {
                Vector vec_other_center_vertex = Vector(
                    std::make_shared<Point>(other_center),
                    std::make_shared<Point>(vertex));
                vec_other_center_vertex = vec_other_center_vertex;
                Vector vec_vertex = vec_other_center + vec_other_center_vertex;
                vertex = Point(vec_vertex.x(), vec_vertex.y(), vec_vertex.z());
                other_pts.push_back(vertex);
            }
            other_poly = Polygon(other_pts);
            if (poly.crosses_other_polygon(std::make_shared<Polygon>(other_poly)))
                return true;
        }
    }
    return false;
}

int PolygonalCylinder::crosses_box(float box_size_x,
        float box_size_y, float box_size_z) {

    if (std::abs(box_size_y) < std::numeric_limits<float>::epsilon())
        box_size_y = box_size_x;
    if (std::abs(box_size_z) < std::numeric_limits<float>::epsilon())
        box_size_z = box_size_x;
    std::vector<Polygon> polygons = {*_top_facet_ptr, *_bot_facet_ptr};
    for(int i = 0; i < 2; i++) {
        int a = polygons[i].crosses_box(box_size_x, box_size_y, box_size_z);
        if (a > 0)
            return a;
    }
    return false;
};


void PolygonalCylinder::change_by_matrix(Matrix m) {
    std::vector<Polygon> polygons = {*_top_facet_ptr, *_bot_facet_ptr};
    for (auto& facet : _facets)
        polygons.push_back(facet);
    for (int i = 0; i < polygons.size(); ++i) {
        std::vector<Point> pts;
        for (auto& vertex : polygons[i].vertices()) {
            Vector v(vertex.x(), vertex.y(), vertex.z());
            v = *(v.matrix_product_ptr(m)); 
            pts.push_back(Point(v.x(), v.y(), v.z()));
        }
        polygons[i] = Polygon(pts);
    }
    _top_facet_ptr = std::make_shared<Polygon>(polygons[0]);
    _bot_facet_ptr = std::make_shared<Polygon>(polygons[1]);
    _facets.clear();
    for (int i = 2; i < polygons.size(); ++i)
        _facets.push_back(polygons[i]);
    Vector v(_top_facet_center_ptr->x(),
             _top_facet_center_ptr->y(),
             _top_facet_center_ptr->z());
    v = *v.matrix_product_ptr(m);
    _top_facet_center_ptr = std::make_shared<Point>(v.x(), v.y(), v.z());
    v = Vector(_bot_facet_center_ptr->x(),
               _bot_facet_center_ptr->y(),
               _bot_facet_center_ptr->z());
    v = *v.matrix_product_ptr(m);
    _bot_facet_center_ptr = std::make_shared<Point>(v.x(), v.y(), v.z());
};

void PolygonalCylinder::translate(float dx, float dy, float dz) {
    std::vector<std::vector<float> > elements = {
        {1, 0, 0, 0},
        {0, 1, 0, 0},
        {0, 0, 1, 0},
        {dx, dy, dz, 1}
    };
    this->change_by_matrix(Matrix(
        std::make_shared<std::vector<std::vector<float> > > (elements)));
}

void PolygonalCylinder::rotate_around_x(float angle) {
    std::vector<std::vector<float> > elements = {
        {1, 0, 0, 0},
        {0, (float)cos(angle), -(float)sin(angle), 0},
        {0, (float)sin(angle), (float)cos(angle), 0},
        {0, 0, 0, 1}
    };
    this->change_by_matrix(Matrix(
        std::make_shared<std::vector<std::vector<float> > > (elements)));
};

void PolygonalCylinder::rotate_around_y(float angle) {
    std::vector<std::vector<float> > elements = {
        {(float)cos(angle), 0, (float)sin(angle), 0},
        {0, 1, 0, 0},
        {-(float)sin(angle), 0, (float)cos(angle), 0},
        {0, 0, 0, 1}
    };
    this->change_by_matrix(Matrix(
        std::make_shared<std::vector<std::vector<float> > > (elements)));
};

void PolygonalCylinder::rotate_around_z(float angle) {
    std::vector<std::vector<float> > elements = {
        {(float)cos(angle), -(float)sin(angle), 0, 0},
        {(float)sin(angle), (float)cos(angle), 0, 0},
        {0, 0, 1, 0},
        {0, 0, 0, 1}
    };
    this->change_by_matrix(Matrix(
        std::make_shared<std::vector<std::vector<float> > > (elements)));
};

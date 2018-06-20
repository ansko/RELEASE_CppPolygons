#pragma once

#include <memory>
#include <string>
#include <vector>

#include <math.h>

#include "../algebra/matrix.hpp"
#include "point.hpp"
#include "polygon.hpp"
#include "polygonal_polyhedron.hpp"


class PolygonalCylinder : PolygonalPolyhedron
{
public:
    PolygonalCylinder(PolygonalPolyhedron polyhedron);
    PolygonalCylinder(std::vector<std::shared_ptr<Plane> > plane_ptrs);
    PolygonalCylinder(int verticesNumber, float thickness, float outer_radius);
    PolygonalCylinder(std::shared_ptr<Polygon> top_facet_ptr,
                      std::shared_ptr<Polygon> bot_facet_ptr);
    std::shared_ptr<Polygon> top_facet_ptr();
    std::shared_ptr<Polygon> bot_facet_ptr();
    std::vector<Polygon> facets();
    bool crosses_other_polygonal_cylinder(
        std::shared_ptr<PolygonalCylinder> other_polygonal_cylinder);
    int crosses_box(float box_size_x,
                        float box_size_y=0, float box_size_z=0);
    void change_by_matrix(Matrix m);
    void translate(float dx, float dy, float dz);
    void rotate_around_x(float angle);
    void rotate_around_y(float angle);
    void rotate_around_z(float angle);
    std::shared_ptr<Point> top_facet_center_ptr();
    std::shared_ptr<Point> bot_facet_center_ptr();
    float get_outer_radius();
protected:
    std::vector<Polygon> _facets;
    std::shared_ptr<Polygon> _top_facet_ptr=nullptr, _bot_facet_ptr=nullptr;
    int _vertices_number = 0;
    float _thickness = 0.f;
    float _outer_radius = 0.f;
    float _inner_radius = 0.f;
    float _central_angle = 0.f;
    float _edge_length = 0.f;
    std::shared_ptr<Point> _top_facet_center_ptr;
    std::shared_ptr<Point> _bot_facet_center_ptr;
    std::shared_ptr<Matrix> operation_history;
};

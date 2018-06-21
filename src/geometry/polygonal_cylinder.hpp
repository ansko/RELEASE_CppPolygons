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
    PolygonalCylinder(const PolygonalPolyhedron &polyhedron);
    PolygonalCylinder(const std::vector<Plane> &planes);
    PolygonalCylinder(const int verticesNumber, const float thickness,
                      const float outer_radius);
    PolygonalCylinder(const Polygon &top_facet, const Polygon &bot_facet);

    const Polygon top_facet() const;
    const Polygon bot_facet() const;
    const std::vector<Polygon> facets() const;
    const Point top_facet_center() const;
    const Point bot_facet_center() const;
    const float outer_radius() const;

    void change_by_matrix(const Matrix &m);
    void translate(const float dx, const float dy, const float dz);
    void rotate_around_x(const float angle);
    void rotate_around_y(const float angle);
    void rotate_around_z(const float angle);

    const bool crosses_other_polygonal_cylinder(const PolygonalCylinder &other)
        const;
    const int crosses_box(const float Lx, const float Ly=0, const float Lz=0)
        const;
protected:
    std::vector<Polygon> _facets;
    Polygon _top_facet, _bot_facet;
    int _vertices_number = 0;
    float _thickness = 0.f;
    float _outer_radius = 0.f;
    float _inner_radius = 0.f;
    float _central_angle = 0.f;
    float _edge_length = 0.f;
    Point _top_facet_center;
    Point _bot_facet_center;
    //Matrix operation_history(4, 4); // not ready
};

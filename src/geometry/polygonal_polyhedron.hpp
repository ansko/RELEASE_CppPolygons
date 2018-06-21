#pragma once

#include <memory>
#include <vector>

#include "polygon.hpp"
#include "polygonal_polyhedron.hpp"


class PolygonalPolyhedron
{
public:
    const std::vector<Polygon> polygons() const;
protected:
    std::vector<Polygon> _polygons;
};

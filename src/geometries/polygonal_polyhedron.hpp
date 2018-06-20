#pragma once

#include <memory>
#include <vector>

#include "polygon.hpp"
#include "polygonal_polyhedron.hpp"


class PolygonalPolyhedron
{
public:
    std::vector<std::shared_ptr<Polygon> > polygon_ptrs();
protected:
    std::vector<std::shared_ptr<Polygon> > _polygon_ptrs;
};

#pragma once

#include <memory>
#include <string>
#include <vector>

#include "../geometries/polygonal_polyhedron.hpp"


class PolyhedronalSystem
{
public:
    PolyhedronalSystem(); // empty
    PolyhedronalSystem(
        std::vector<std::vector<std::shared_ptr<PolygonalPolyhedron> > > phases);
    std::vector<std::vector<std::shared_ptr<PolygonalPolyhedron> > > phases();
    int phases_number();
    std::string construction_method();
protected:
    int _phases_number;
    std::string _construction_method;
    std::vector<std::vector<std::shared_ptr<PolygonalPolyhedron> > > _phases;
};

#pragma once

#include <memory>
#include <string>
#include <vector>

#include "../geometry/polygonal_polyhedron.hpp"


class PolyhedronalSystem
{
public:
    PolyhedronalSystem();
    PolyhedronalSystem(
        std::vector<std::vector<PolygonalPolyhedron> > &phases);

    const std::vector<std::vector<PolygonalPolyhedron> > phases() const;
    const int phases_number() const;
    const std::string construction_method() const;
protected:
    int _phases_number;
    std::string _construction_method;
    std::vector<std::vector<PolygonalPolyhedron> > _phases;
};

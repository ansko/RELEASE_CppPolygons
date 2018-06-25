#pragma once

#include <memory>
#include <vector>

#include "../geometry/polygonal_polyhedron.hpp"
#include "../geometry/orthobrick.hpp"
#include "../geometry/polygonal_cylinder.hpp"
#include "polyhedronal_system.hpp"


class TernaryPolygonalSystem : public PolyhedronalSystem
{
public:
    TernaryPolygonalSystem();
    TernaryPolygonalSystem(std::vector<std::vector<PolygonalPolyhedron> > phases);

    const std::vector<PolygonalCylinder> shells() const ;
    const std::vector<PolygonalCylinder> fillers() const;
    const float Lx() const;
    const float Ly() const;
    const float Lz() const;
    const int random_attempts_made() const;

    const bool signle_tactoid(const float Lx, const float Ly, const float Lz,
        const int vertices_number, const float thickness,
        const float shell_thickness, const float outer_radius,
        const int fillers_number);
    const bool random_polygonal_cylinders(const float Lx, const float Ly,
        const  float Lz, const int vertices_number, const float thickness,
        const float shell_thickness, const float outer_radius,
        const int fillers_number, const int max_attempts);
    const bool mix();
protected:
    Orthobrick _matrix;
    std::vector<PolygonalCylinder> _fillers;
    std::vector<PolygonalCylinder> _shells;
    int _random_attempts_made = 0;
    float _Lx = 0;
    float _Ly = 0;
    float _Lz = 0;
};

#pragma once

#include <memory>
#include <vector>

#include "../geometries/polygonal_polyhedron.hpp"
#include "../geometries/orthobrick.hpp"
#include "../geometries/polygonal_cylinder.hpp"
#include "polyhedronal_system.hpp"


class TernaryPolygonalSystem : public PolyhedronalSystem
{
public:
    TernaryPolygonalSystem() : PolyhedronalSystem() {};
    TernaryPolygonalSystem(
        std::vector<std::vector<std::shared_ptr<PolygonalPolyhedron> > > phases)
            : PolyhedronalSystem()
   {
       _matrix = std::make_shared<Orthobrick>(*phases[0][0]);
       int fillers_number = phases[1].size();
       int shells_number = phases[2].size();
       _fillers.reserve(fillers_number);
       for (int idx = 0; idx < fillers_number; ++idx)
           _fillers[idx] = std::make_shared<PolygonalCylinder>(*phases[1][idx]);
       _shells.reserve(shells_number);
       for (int idx = 0; idx < shells_number; ++idx)
           _fillers[idx] = std::make_shared<PolygonalCylinder>(*phases[2][idx]);
       _phases_number = 3;
   };
    bool signle_tactoid(float Lx, float Ly, float Lz,
        int vertices_number, float thickness, float shell_thickness,
        float outer_radius, int fillers_number);
    bool random_polygonal_cylinders(float Lx, float Ly, float Lz,
        int vertices_number, float thickness, float shell_thickness,
        float outer_radius, int fillers_number, int max_attempts);
    bool mix();
    std::vector<std::shared_ptr<PolygonalCylinder> > shells();
    std::vector<std::shared_ptr<PolygonalCylinder> > fillers();
    float Lx();
    float Ly();
    float Lz();
protected:
    std::shared_ptr<Orthobrick> _matrix;
    std::vector<std::shared_ptr<PolygonalCylinder> > _fillers;
    std::vector<std::shared_ptr<PolygonalCylinder> > _shells;
    float _Lx = 0;
    float _Ly = 0;
    float _Lz = 0;
};

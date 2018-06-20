#pragma once

#include <memory>
#include <vector>

#include "polygonal_polyhedron.hpp"


class Orthobrick : PolygonalPolyhedron
{
public:
    Orthobrick(float Lx, float Ly, float Lz);
    Orthobrick(PolygonalPolyhedron polyhedron);
    float Lx();
    float Ly();
    float Lz();
protected:
    float _Lx = 0;
    float _Ly = 0;
    float _Lz = 0;
};

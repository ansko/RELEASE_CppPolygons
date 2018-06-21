#pragma once

#include <memory>
#include <vector>

#include "polygonal_polyhedron.hpp"


class Orthobrick : PolygonalPolyhedron
{
public:
    Orthobrick();
    Orthobrick(const float Lx, const float Ly, const float Lz);
    Orthobrick(const PolygonalPolyhedron &polyhedron);
    const float Lx() const;
    const float Ly() const;
    const float Lz() const;
protected:
    float _Lx = 0;
    float _Ly = 0;
    float _Lz = 0;
};

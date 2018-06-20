#pragma once

#include <memory>
#include <set>
#include <vector>

#include "../geometries/polygonal_polyhedron.hpp"
#include "../geometries/polygonal_cylinder.hpp"
#include "../systems_architecture/polyhedronal_system.hpp"
#include "../systems_architecture/ternary_polygonal_system.hpp"


class PercolationChecker
{
public:
    std::vector<std::pair<int, int> > get_intersections(
        TernaryPolygonalSystem system);
    void convert_intersections_into_percolation();
    void clean();
    bool percolation_along_x();
    bool percolation_along_y();
    bool percolation_along_z();
private:
    std::vector<std::pair<int, int> > _last_intersections_found;
    std::vector<int> _last_box_crosses_found;
    float _Lx = 0;
    float _Ly = 0;
    float _Lz = 0;
    TernaryPolygonalSystem _system;
    bool _percolation_checked=false;
    bool _percolation_along_x=false;
    bool _percolation_along_y=false;
    bool _percolation_along_z=false;
};


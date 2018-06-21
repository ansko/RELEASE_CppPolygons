#include "orthobrick.hpp"


Orthobrick::Orthobrick() {
    _Lx = 0;
    _Ly = 0,
    _Lz = 0;
}

Orthobrick::Orthobrick(const float Lx, const float Ly, const float Lz) {
    _Lx = Lx;
    _Ly = Ly;
    _Lz = Lz;
}

Orthobrick::Orthobrick(const PolygonalPolyhedron &polyhedron) {
    Polygon top_facet = polyhedron.polygons()[0];
    Polygon bot_facet = polyhedron.polygons()[1];
    if (top_facet.vertices().size() != 4 || bot_facet.vertices().size() != 4) {
        std::cout << "Error in Orthobrick(PolygonalPolyhedron) constructor\n";
        return;
    }
    _Lx = Vector(top_facet.vertices()[0], top_facet.vertices()[1]).length();
    _Ly = Vector(top_facet.vertices()[2], top_facet.vertices()[1]).length();
    _Lz = Vector(top_facet.vertices()[0], bot_facet.vertices()[0]).length();
}


const float Orthobrick::Lx() const {
    return _Lx;
}

const float Orthobrick::Ly() const {
    return _Ly;
}

const float Orthobrick::Lz() const {
    return _Lz;
}

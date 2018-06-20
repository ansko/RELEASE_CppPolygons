#include "orthobrick.hpp"


Orthobrick::Orthobrick(float Lx, float Ly, float Lz) {
    _Lx = Lx;
    _Ly = Ly;
    _Lz = Lz;
}

Orthobrick::Orthobrick(PolygonalPolyhedron polyhedron) {
    auto top_facet_ptr = polyhedron.polygon_ptrs()[0];
    auto bot_facet_ptr = polyhedron.polygon_ptrs()[1];
    if (top_facet_ptr->vertices().size() != 4) {
        std::cout << "Error in Orthobrick(PolygonalPolyhedron) constructor\n";
        return;
    }
    _Lx = Vector(std::make_shared<Point>(top_facet_ptr->vertices()[0]),
                 std::make_shared<Point>(top_facet_ptr->vertices()[1])).length();
    _Ly = Vector(std::make_shared<Point>(top_facet_ptr->vertices()[1]),
                 std::make_shared<Point>(top_facet_ptr->vertices()[2])).length();
    _Lz = Vector(std::make_shared<Point>(top_facet_ptr->vertices()[0]),
                 std::make_shared<Point>(bot_facet_ptr->vertices()[0])).length();

}


float Orthobrick::Lx() {
    return _Lx;
}

float Orthobrick::Ly() {
    return _Ly;
}

float Orthobrick::Lz() {
    return _Lz;
}

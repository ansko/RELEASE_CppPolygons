#include "polyhedronal_system.hpp"


PolyhedronalSystem::PolyhedronalSystem() {
}

PolyhedronalSystem::PolyhedronalSystem(
    std::vector<std::vector<std::shared_ptr<PolygonalPolyhedron> > > phases) {
        _phases_number = phases.size();
        _phases = phases;
}


std::vector<std::vector<std::shared_ptr<PolygonalPolyhedron> > >
    PolyhedronalSystem::phases() {
        return _phases;
}

int PolyhedronalSystem::phases_number() {
    return _phases_number;
}

std::string PolyhedronalSystem::construction_method() {
    return _construction_method;
}

#include "polyhedronal_system.hpp"


PolyhedronalSystem::PolyhedronalSystem() {
}

PolyhedronalSystem::PolyhedronalSystem(
    std::vector<std::vector<PolygonalPolyhedron> > &phases) {
        _phases_number = phases.size();
        _phases = phases;
}


const std::vector<std::vector<PolygonalPolyhedron> > PolyhedronalSystem::phases()
    const {
        return _phases;
}

const int PolyhedronalSystem::phases_number() const {
    return _phases_number;
}

const std::string PolyhedronalSystem::construction_method() const {
    return _construction_method;
}

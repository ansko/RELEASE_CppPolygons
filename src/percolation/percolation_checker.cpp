#include "percolation_checker.hpp"



std::vector<std::pair<int, int> > PercolationChecker::get_intersections(
    TernaryPolygonalSystem system) {
        _system = system;
        _Lx = system.Lx();
        _Ly = system.Ly();
        _Lz = system.Lz();
        std::vector<int> box_crosses;
        std::vector<std::pair<int, int> > intersections;
        int shells_number = system.shells().size();
        for(int idx = 0; idx < shells_number; ++idx) {
            if (system.shells()[idx]->crosses_box(_Lx, _Ly, _Lz))
                box_crosses.push_back(1);
            for(int idx_other = 0; idx_other < shells_number; ++idx_other) {
                if (idx == idx_other)
                    continue;
                if (system.shells()[idx]->crosses_other_polygonal_cylinder(
                        system.shells()[idx_other]) &&
                    system.shells()[idx_other]->crosses_other_polygonal_cylinder(
                        system.shells()[idx])) {
                    //std::cout << "inter " << idx << " " << idx_other << std::endl;
                    intersections.push_back(std::pair<int, int>(idx, idx_other));
                }
            }
        }
        _last_intersections_found = intersections;
        _last_box_crosses_found = box_crosses;
        return intersections;
}


void PercolationChecker::convert_intersections_into_percolation() {
    _percolation_checked = true;
    std::cout << "intersections number: " << _last_intersections_found.size()
              << "\nbox crosses number: " << _last_box_crosses_found.size()
              << "\n";
    std::vector<std::set<int> > clusters;
    for (auto intersection : _last_intersections_found) {
        int first = intersection.first;
        int second = intersection.second;
        bool flag_any_cluster_with_them_exists = false;
        for (auto& cluster : clusters) {
            if (cluster.count(first) > 0 && cluster.count(second) == 0) {
                flag_any_cluster_with_them_exists = true;
                cluster.insert(second);
            } else if (cluster.count(first) == 0 && cluster.count(second) > 0) {
                flag_any_cluster_with_them_exists = true;
                cluster.insert(first);
            }
            else if (cluster.count(first) > 0 && cluster.count(second) > 0)
                flag_any_cluster_with_them_exists = true;
        }
        if (clusters.size() == 0 || !flag_any_cluster_with_them_exists) {
            std::set<int> new_cluster = {first, second};
            clusters.push_back(new_cluster);
        }
    }
    std::cout << clusters.size() << " clusters found\n";

    float ove_xmin = _Lx;
    float ove_ymin = _Ly;
    float ove_zmin = _Lz;
    float ove_xmax = 0;
    float ove_ymax = 0;
    float ove_zmax = 0;
    for (auto cluster : clusters) {
        float xmin = _Lx;
        float ymin = _Ly;
        float zmin = _Lz;
        float xmax = 0;
        float ymax = 0;
        float zmax = 0;
        for (auto polyhedron_idx : cluster)
            for (auto poly : _system.shells()[polyhedron_idx]->facets())
                for (auto vertex : poly.vertices()) {
                    float x = vertex.x();
                    float y = vertex.y();
                    float z = vertex.z();
                    if (x < xmin)
                        xmin = x;
                    if (y < ymin)
                        ymin = y;
                    if (z < zmin)
                        zmin = z;
                    if (x > xmax)
                        xmax = x;
                    if (y > ymax)
                        ymax = y;
                    if (z > zmax)
                        zmax = z;
                };
        if (xmin <= 0 && xmax >= _Lx)
            _percolation_along_x = true;
        if (ymin <= 0 && ymax >= _Ly)
            _percolation_along_y = true;
        if (zmin <= 0 && zmax >= _Lz)
            _percolation_along_z = true;
    }
}


void PercolationChecker::clean() {
    _percolation_checked=false;
    _percolation_along_x=false;
    _percolation_along_y=false;
    _percolation_along_z=false;
}

bool PercolationChecker::percolation_along_x() {
    if (!_percolation_checked)
        std::cout << "Warning: _percolation_checked is false\n";
    return _percolation_along_x;
}

bool PercolationChecker::percolation_along_y() {
    if (!_percolation_checked)
        std::cout << "Warning: _percolation_checked is false\n";
    return _percolation_along_y;
}

bool PercolationChecker::percolation_along_z() {
    if (!_percolation_checked)
        std::cout << "Warning: _percolation_checked is false\n";
    return _percolation_along_z;
}

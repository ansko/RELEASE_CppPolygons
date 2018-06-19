#include "percolation_checker.hpp"


PercolationChecker::PercolationChecker(
    std::vector<std::shared_ptr<PolygonalCylinder> > fils,
    std::vector<std::shared_ptr<PolygonalCylinder> > shells) {
        _fils = fils;
        _shells = shells;
}

std::vector<std::pair<int, int> > PercolationChecker::get_intersections() {
    std::vector<std::pair<int, int> > intersections;
    int fils_number = _fils.size();
    for(int idx = 0; idx < fils_number; idx++)
        for(int idx_other = 0; idx_other < fils_number; idx_other++) {
            if (idx == idx_other)
                continue;
            if (_shells[idx]->crosses_other_polygonal_cylinder(_shells[idx_other]))
                intersections.push_back(std::pair<int, int>(idx, idx_other));
        }
    return intersections;
}

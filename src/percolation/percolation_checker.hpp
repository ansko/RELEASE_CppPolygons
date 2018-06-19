#pragma once

#include <memory>
#include <vector>

#include "../geometries/polygonal_cylinder.hpp"


class PercolationChecker
{
public:
    PercolationChecker(
        std::vector<std::shared_ptr<PolygonalCylinder> > fils,
        std::vector<std::shared_ptr<PolygonalCylinder> > shells);

    std::vector<std::shared_ptr<PolygonalCylinder> > fils();
    std::vector<std::shared_ptr<PolygonalCylinder> > shells();
    std::vector<std::pair<int, int> > get_intersections();
private:
    std::vector<std::shared_ptr<PolygonalCylinder> > _fils;
    std::vector<std::shared_ptr<PolygonalCylinder> > _shells;
};


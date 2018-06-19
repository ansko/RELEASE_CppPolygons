#include <iostream>
#include <memory>
#include <vector>
#include <ctime>
#include <cstdlib>

#include "../settings/settings_parser.hpp"
#include "../io/csg_printer_polygons.hpp"
#include "../geometries/polygonal_cylinder.hpp"


class SystemArchitector {
public:
    float random_polygonal_cylinders(
        float Lx, float Ly, float Lz, int vertices_number, float thickness,
        float shell_thickness, float outer_radius, int disks_number,
        int max_attempts);
    std::vector<std::shared_ptr<PolygonalCylinder> > fils;
    std::vector<std::shared_ptr<PolygonalCylinder> > shells;
};

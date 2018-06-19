#pragma once

#include <fstream>
#include <memory>
#include <string>
#include <vector>

#include <math.h>

#include "../geometries/plane.hpp"
#include "../geometries/polygonal_cylinder.hpp"
#include "../settings/settings_parser.hpp"


class CSGPrinterPolygons
{
public:
    void print_CSG_ternary_reading_settings(
         std::string fname,
         std::vector<std::shared_ptr<PolygonalCylinder> > fil_ptrs,
         std::vector<std::shared_ptr<PolygonalCylinder> > int_ptrs
    );
};

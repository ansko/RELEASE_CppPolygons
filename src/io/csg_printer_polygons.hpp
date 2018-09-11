#pragma once

#include <fstream>
#include <memory>
#include <string>
#include <vector>

#include <math.h>

#include "../geometry/plane.hpp"
#include "../geometry/polygonal_cylinder.hpp"
#include "../settings/settings_parser.hpp"


class CSGPrinterPolygons
{
public:
    void print_CSG_ternary_reading_settings(
         const std::string fname,
         const std::vector<PolygonalCylinder> &fil,
         const std::vector<PolygonalCylinder> &she,
         const std::string settgins_fname) const;

    void print_CSG_binary_reading_settings(
         const std::string fname,
         const std::vector<PolygonalCylinder> &fil,
         const std::string settgins_fname) const;
};

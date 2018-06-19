#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "src/geometries/point.hpp"
#include "src/geometries/line_segment.hpp"
#include "src/geometries/vector.hpp"
#include "src/geometries/plane.hpp"
#include "src/geometries/polygon.hpp"
#include "src/geometries/polygonal_cylinder.hpp"
#include "src/systems_architecture/random_polygonal_cylinders.hpp"
#include "src/io/csg_printer_polygons.hpp"
#include "src/percolation/percolation_checker.hpp"


int main(int argc, char**argv) {

    // parse settings
    SettingsParser sp("settings");
    sp.print();
    float Lx = std::stof(sp.get_setting(std::string("Lx")));
    float Ly = Lx;  // std::stof(sp.get_setting(std::string("Ly")));
    float Lz = Lx;  // std::stof(sp.get_setting(std::string("Lz")));
    float thickness = std::stof(sp.get_setting(std::string("thickness")));
    float shell_thickness = std::stof(
        sp.get_setting(std::string("shell_thickness")));
    float outer_radius = std::stof(sp.get_setting(std::string("outer_radius")));
    int vertices_number = std::stoi(
        sp.get_setting(std::string("vertices_number")));
    int disks_number = std::stoi(sp.get_setting(std::string("disks_number")));
    int max_attempts = std::stoi(sp.get_setting(std::string("max_attempts")));
    std::string output_fname = sp.get_setting(std::string("output_fname"));

    // create system
    SystemArchitector arch;
    float fi;
    fi = arch.random_polygonal_cylinders(Lx, Ly, Lz,
        vertices_number, thickness, shell_thickness, outer_radius,
        disks_number, max_attempts);

    // output to .geo
    CSGPrinterPolygons p;
    p.print_CSG_ternary_reading_settings(
        std::string("one.geo"), arch.fils, arch.shells);

    PercolationChecker pc(arch.fils, arch.shells);
    std::vector<std::pair<int, int> > intersections = pc.get_intersections();
    std::cout << "intersections number: " << intersections.size() << std::endl;


    return 0;


}

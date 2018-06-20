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
#include "src/io/csg_printer_polygons.hpp"
#include "src/percolation/percolation_checker.hpp"


int main(int argc, char**argv) {
    /*
       1.  Create system of polygonal cylinders by monte-carlo method
       2.  Study percolation
    */
    std::string taks_name("ternary_polygonal_cylinders");
    SettingsParser sp("settings", "ternary_polygonal_cylinders");
    if (!sp.success_of_parsing()) {
        std::cout << "Error: parsed settings are not OK\n";
        return 0;
    }
    sp.print();
    float Lx = std::stof(sp.get_setting(std::string("Lx")));
    float Ly = Lx;
    float Lz = Lx;
    if (sp.check_setting("Ly"))
        Ly = std::stof(sp.get_setting(std::string("Ly")));
    if (sp.check_setting("Lz"))
        Ly = std::stof(sp.get_setting(std::string("Lz")));
    float thickness = std::stof(sp.get_setting(std::string("thickness")));
    float shell_thickness = std::stof(
        sp.get_setting(std::string("shell_thickness")));
    float outer_radius = std::stof(sp.get_setting(std::string("outer_radius")));
    int vertices_number = std::stoi(
        sp.get_setting(std::string("vertices_number")));
    int disks_number = std::stoi(sp.get_setting(std::string("fillers_number")));
    int max_attempts = std::stoi(sp.get_setting(std::string("max_attempts")));
    std::string output_fname = sp.get_setting(std::string("output_fname"));
    //making system
    TernaryPolygonalSystem ternary_system;
    bool status = ternary_system.random_polygonal_cylinders(
                      Lx, Ly, Lz, vertices_number, thickness, shell_thickness,
                      outer_radius, disks_number, max_attempts);
    std::cout << "status of system making (bool): " << status << std::endl;
    //output
    CSGPrinterPolygons p;
    p.print_CSG_ternary_reading_settings(
        "MC_exfoliation.geo", ternary_system.fillers(), ternary_system.shells());
    // study percolation
    PercolationChecker pc;
    std::vector<std::pair<int, int> > intersections =
        pc.get_intersections(ternary_system);
    pc.convert_intersections_into_percolation();
    std::cout << "percolation:\n"
              << "along x: " << pc.percolation_along_x() << std::endl
              << "along y: " << pc.percolation_along_y() << std::endl
              << "along z: " << pc.percolation_along_z() << std::endl
              << std::endl;
    return 0;
}

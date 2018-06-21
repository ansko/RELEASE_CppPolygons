#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "src/geometry/point.hpp"
#include "src/geometry/line_segment.hpp"
#include "src/algebra/vector.hpp"
#include "src/geometry/plane.hpp"
#include "src/geometry/polygon.hpp"
#include "src/geometry/polygonal_cylinder.hpp"
#include "src/io/csg_printer_polygons.hpp"
#include "src/percolation/percolation_checker.hpp"


int main(int argc, char**argv) {
    /*
       1.  Create system of polygonal cylinders by monte-carlo method
       2.  Study percolation
    */
    std::string taks_name("ternary_polygonal_cylinders");
    SettingsParser sp("settings", "MC");
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
    if (!sp.check_setting("thickness"))
        throw MyException("No thickness specified");
    float thickness = std::stof(sp.get_setting(std::string("thickness")));
    if (!sp.check_setting("shell_thickness"))
        throw MyException("No shell thickness specified");
    float shell_thickness = std::stof(
        sp.get_setting(std::string("shell_thickness")));
    if (!sp.check_setting("outer_radius"))
        throw MyException("No outre radius specified");
    float outer_radius = std::stof(sp.get_setting(std::string("outer_radius")));
    if (!sp.check_setting("vertices_number"))
        throw MyException("No vertices_number specified");
    int vertices_number = std::stoi(
        sp.get_setting(std::string("vertices_number")));
    if (!sp.check_setting("disks_number"))
        throw MyException("No fillers number specified");
    int disks_number = std::stoi(sp.get_setting(std::string("disks_number")));
    if (!sp.check_setting("max_attempts"))
        throw MyException("No max attempts specified");
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

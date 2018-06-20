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
//#include "src/io/my_format_printer.hpp"
//#include "src/io/my_format_reader.hpp"
#include "src/percolation/percolation_checker.hpp"

#include "src/systems_architecture/ternary_polygonal_system.hpp"
#include "src/systems_architecture/polyhedronal_system.hpp"


int main(int argc, char**argv) {
    /*
       1.  Create system of tacroid made from polygonal cylinders
       2.  Mix
       3.  Study percolation
    */
    // parse settings
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
    int mixing_steps = std::stoi(sp.get_setting(std::string("mixing_steps")));
    std::string output_fname = sp.get_setting(std::string("output_fname"));
    //making system
    TernaryPolygonalSystem ternary_system;
    bool status = ternary_system.signle_tactoid(
                      Lx, Ly, Lz, vertices_number, thickness, shell_thickness,
                      outer_radius, disks_number);
    std::cout << "status of system making (bool): " << status << std::endl;
    //output
    CSGPrinterPolygons p;
    p.print_CSG_ternary_reading_settings(
        "mixing_start.geo", ternary_system.fillers(), ternary_system.shells());
    // mixing
    {
        int steps_made = 0;
        while (steps_made < mixing_steps)
            steps_made += ternary_system.mix();
    }
    p.print_CSG_ternary_reading_settings(
        "mixing_mixed.geo", ternary_system.fillers(), ternary_system.shells());
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

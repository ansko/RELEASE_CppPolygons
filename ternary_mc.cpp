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
#include "src/systems_architecture/ternary_polygonal_system.hpp"

int main(int argc, char**argv) {
    /*
       usage: ./exe settings_fname
    */
    if (argc < 2) {
        std::cout << "Not enuogh arguments\n";
        return 0;
    }
    std::string taks_name("ternary_polygonal_cylinders");
    SettingsParser sp(std::string(argv[1]), "MC");
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
    std::string LOG = std::string("LOG");
    if (sp.check_setting("LOG"))
        LOG = sp.get_setting(std::string("LOG"));
    //making system
    TernaryPolygonalSystem ternary_system;
    bool status = ternary_system.random_polygonal_cylinders(
                      Lx, Ly, Lz, vertices_number, thickness, shell_thickness,
                      outer_radius, disks_number, max_attempts);
    std::cout << "status of system making (bool): " << status << std::endl;
    // checking prepared system that may be incorrect
    //     for some unknown reason (check uses the same function
    //                              and reveals error in system formation).
    bool flag_testing = false;
    auto fillers = ternary_system.fillers();
    for (int i = 0; i < fillers.size(); ++i) {
        auto polys_i = fillers[i].all_polygons();
        for (int j = 0; j < fillers.size(); ++j) {
            if (i == j)
                continue;
            auto polys_j = fillers[j].all_polygons();
            for (auto pi : polys_i)
                for (auto pj : polys_j)
                    if (pi.crosses_other_polygon(pj)) {
                        flag_testing = true;
                        std::cout << "cross!\n";
                        break;
                    }
        }
    }
    //output
    CSGPrinterPolygons p;
    p.print_CSG_ternary_reading_settings(
        sp.get_setting(std::string("geo_fname")), ternary_system.fillers(),
        ternary_system.shells(), std::string(argv[1]));
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
    // logging
    std::ofstream logger(LOG);
    logger
        << "MC_exfoliation (algorithm used)\n"
        << status << " (status of system formation)\n"
        << ternary_system.fillers().size() << " (number of fillers prepared)\n"
        << disks_number << " (requested number of fillers)\n"
        << max_attempts << " (possible max attempts number)\n"
        << ternary_system.random_attempts_made() << " (real attempts number)\n"
        << flag_testing << "  (flag_testing == is system ok)\n"
        << intersections.size() << " (number of intersections in system)\n"
        << pc.percolation_along_x() << " (percolation flag along x: )\n"
        << pc.percolation_along_y() << " (percolation flag along y: )\n"
        << pc.percolation_along_z() << " (percolation flag along z: )\n";
    return 0;
}

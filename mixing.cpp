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
#include "src/systems_architecture/polyhedronal_system.hpp"


int main(int argc, char**argv) {
    /*
       1.  Create system of tacroid made from polygonal cylinders
               (aligned perpendicullarily to z)
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
    if (!sp.check_setting("thickness"))
        throw MyException(std::string("no thickness specified in settings"));
    float thickness = std::stof(sp.get_setting(std::string("thickness")));
    if (!sp.check_setting("shell_thickness"))
        throw MyException(std::string("no shell thickness specified in settings"));
    float shell_thickness = std::stof(
        sp.get_setting(std::string("shell_thickness")));
    if (!sp.check_setting("outer_radius"))
        throw MyException(std::string("no outer radius specified in settings"));
    float outer_radius = std::stof(sp.get_setting(std::string("outer_radius")));
    if (!sp.check_setting("vertices_number"))
        throw MyException(std::string("no vertices number specified in settings"));
    int vertices_number = std::stoi(
        sp.get_setting(std::string("vertices_number")));
    if (!sp.check_setting("disks_number"))
        throw MyException(std::string("no disks number specified in settings"));
    int disks_number = std::stoi(sp.get_setting(std::string("disks_number")));
    if (!sp.check_setting("max_attempts"))
        throw MyException(std::string("no max attempts specified in settings"));
    int max_attempts = std::stoi(sp.get_setting(std::string("max_attempts")));
    if (!sp.check_setting("mixing_steps"))
        throw MyException(std::string("no mixing steps specified in settings"));
    int mixing_steps = std::stoi(sp.get_setting(std::string("mixing_steps")));
    std::string output_fname = sp.get_setting(std::string("output_fname"));
    std::string LOG = std::string("LOG");
    if (sp.check_setting("LOG"))
        LOG = sp.get_setting(std::string("LOG"));
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
        int steps_tried = 0;
        while (steps_made < mixing_steps) {
            std::cout << "mixing, steps made " << steps_made
                      << " / " << mixing_steps
                      << " (steps tried: " << ++steps_tried << ")\n";
            steps_made += ternary_system.mix();
        }
    }
    // check mixing quality
    float order_parameter = 0;
    int not_moved = 0;
    for (auto fil : ternary_system.fillers()) {
        Vector top_bot(fil.bot_facet_center(), fil.top_facet_center());
        float tmp_order_parameter = std::abs(top_bot.z() / top_bot.length());
        order_parameter += tmp_order_parameter;
        if (tmp_order_parameter > 0.99)
            ++not_moved;
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
    // logging
    std::ofstream logger(LOG);
    logger
        << "mixing_single_tactoid (algorithm used)\n"
        << status << " (status of system formation)\n"
        << ternary_system.fillers().size() << " (number of fillers prepared)\n"
        << disks_number << " (requested number of fillers)\n"
        << max_attempts << " (possible max attempts number)\n"
        << ternary_system.random_attempts_made() << " (real attempts number)\n"
        << flag_testing << "  (flag_testing == is system ok)\n"
        << intersections.size() << " (number of intersections in system)\n"
        << pc.percolation_along_x() << " (percolation flag along x: )\n"
        << pc.percolation_along_y() << " (percolation flag along y: )\n"
        << pc.percolation_along_z() << " (percolation flag along z: )\n"
        << not_moved << " (particles that seems not to move at any step)\n"
        << order_parameter / ternary_system.fillers().size()
            << " (average order parameter as 0th Chebushev's polynome)\n";
    return 0;
}

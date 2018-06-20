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


    Point pt_1(0, 0, 0);
    Point pt_2(0, 1, 0);
    Point pt_3(0, 0, 1);
    Point pt_4(5, 0, 0);
    Point pt_5(5, 1, 0);
    Point pt_6(5, 0, 1);
    std::vector<Point> vertices_1 = {pt_1, pt_2, pt_3};
    std::vector<Point> vertices_2 = {pt_4, pt_5, pt_6};
    Polygon poly_1(vertices_1);
    Polygon poly_2(vertices_2);

    PolygonalCylinder pc(6, 1, 1);

    /*{
        Point top_facet_center = *(pc.top_facet_center_ptr());
        Point bot_facet_center = *(pc.bot_facet_center_ptr());
        Point center = Point(
            (top_facet_center.x() + bot_facet_center.x()) / 2,
            (top_facet_center.y() + bot_facet_center.y()) / 2,
            (top_facet_center.z() + bot_facet_center.z()) / 2);
        Vector vec_top_bottom_centers(
            std::make_shared<Point>(top_facet_center),
            std::make_shared<Point>(bot_facet_center));
        std::cout << "plane("
                 << top_facet_center.x() << ", "
                 << top_facet_center.y() << ", "
                 << top_facet_center.z() << "; "
                 << -vec_top_bottom_centers.x() << ", "
                 << -vec_top_bottom_centers.y() << ", "
                 << -vec_top_bottom_centers.z() << ")"
                 << "\n\tand\n\tplane("
                 << bot_facet_center.x() << ", "
                 << bot_facet_center.y() << ", "
                 << bot_facet_center.z() << "; "
                 << vec_top_bottom_centers.x() << ", "
                 << vec_top_bottom_centers.y() << ", "
                 << vec_top_bottom_centers.z() << ")\n";
        for (int facet_idx = 0; facet_idx < 3; ++facet_idx) {
            Polygon facet = pc.facets()[facet_idx];
            Vector vec_center_facet_center = Vector(
                std::make_shared<Point>(center),
                facet.pt_center_ptr());
            std::cout << "\tand\n\tplane("
                      << facet.pt_center_ptr()->x() << ", "
                      << facet.pt_center_ptr()->y() << ", "
                      << facet.pt_center_ptr()->z() << "; "
                      << vec_center_facet_center.x() << ", "
                      << vec_center_facet_center.y() << ", "
                      << vec_center_facet_center.z() << ")";
        }

    }*/

    pc.translate(2.5, 2.5, 2.5);

    std::vector<std::shared_ptr<PolygonalCylinder> > fil_ptrs = {
        std::make_shared<PolygonalCylinder>(pc)
    };
    CSGPrinterPolygons pr;
    pr.print_CSG_ternary_reading_settings(std::string("tmp.geo"),
        fil_ptrs, fil_ptrs);

    //return 0;

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

    TernaryPolygonalSystem ternary_system;
/*    bool status = ternary_system.random_polygonal_cylinders(
                      Lx, Ly, Lz, vertices_number, thickness, shell_thickness,
                      outer_radius, disks_number, max_attempts);
*/
    bool status = ternary_system.signle_tactoid(
                      Lx, Ly, Lz, vertices_number, thickness, shell_thickness,
                      outer_radius, disks_number);
    std::cout << "status of system making (bool): " << status << std::endl;

    CSGPrinterPolygons p;
    p.print_CSG_ternary_reading_settings(
        "start.geo", ternary_system.fillers(), ternary_system.shells());
    // mixing
    {
        int steps_made = 0;
        while (steps_made < 10)
            steps_made += ternary_system.mix();
    }
    p.print_CSG_ternary_reading_settings(
        "mixed.geo", ternary_system.fillers(), ternary_system.shells());

/*
    // study percolation
    PercolationChecker pc;
    std::vector<std::pair<int, int> > intersections =
        pc.get_intersections(ternary_system);

    pc.convert_intersections_into_percolation();
*/

    return 0;
}

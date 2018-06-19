#include "csg_printer_polygons.hpp"


void CSGPrinterPolygons::print_CSG_ternary_reading_settings(
         std::string fname,
         std::vector<std::shared_ptr<PolygonalCylinder> > fil_ptrs,
         std::vector<std::shared_ptr<PolygonalCylinder> > int_ptrs
) {
    if (fil_ptrs.size() == 0 && int_ptrs.size() == 0) {
        std::cout << "Warning: empty ternary system\n";
        return;
    }
    std::vector<std::string> required_settings = {
        std::string("Lx"),
        std::string("vertices_number"),
        std::string("thickness"),
        std::string("outer_radius"),
        std::string("shell_thickness")
    };
    std::ofstream fout;
    fout.open(fname);
    SettingsParser sp("settings");
    for (std::string key : required_settings)
        if (!sp.check_setting(key)) {
            std::cout << "Required setting is not set: " << key << std::endl;
            return;
        }

    float Lx = std::stof(sp.get_setting("Lx"));
    float Ly = Lx, Lz = Lx;
    if (sp.check_setting(std::string("Ly")) &&
        sp.check_setting(std::string("Lz"))) {
            Ly = std::stof(sp.get_setting("Ly"));
            Lz = std::stof(sp.get_setting("Lz"));
    }
    float vertices_number = std::stof(sp.get_setting("vertices_number"));
    float th = std::stof(sp.get_setting("thickness"));
    float sh = std::stof(sp.get_setting("shell_thickness"));
    float outer_radius = std::stof(sp.get_setting("outer_radius"));
    float edge_length = outer_radius * 2 * sin(M_PI / vertices_number);
    float inner_radius = edge_length / 2 / tan(M_PI / vertices_number);

    std::string fillers_string = "solid filler =\n\tpc_filler_0";
    std::string shells_string = "solid shells =\n\tpc_shell_0";
    fout << "algebraic3d\n\n";
    fout << "solid cell =\n\torthobrick(0, 0, 0; ";
    fout << Lx << ", " << Ly << ", " << Lz << ");\n";
    if (fil_ptrs.size() > 0) {
        // fillers
        for (int fil_idx = 0; fil_idx < fil_ptrs.size(); ++fil_idx) {
            Point top_facet_center = *(fil_ptrs[fil_idx]->top_facet_center_ptr());
            Point bot_facet_center = *(fil_ptrs[fil_idx]->bot_facet_center_ptr());
            Point center = Point(
                (top_facet_center.x() + bot_facet_center.x()) / 2,
                (top_facet_center.y() + bot_facet_center.y()) / 2,
                (top_facet_center.z() + bot_facet_center.z()) / 2);
            Vector vec_top_bottom_centers(
                std::make_shared<Point>(top_facet_center),
                std::make_shared<Point>(bot_facet_center));
            fout << "solid pc_filler_" << fil_idx << " =\n\tplane("
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
            for (int facet_idx = 0;
                   facet_idx < fil_ptrs[fil_idx]->facets().size();
                     ++facet_idx) {
                Polygon facet = fil_ptrs[fil_idx]->facets()[facet_idx];
                Vector vec_center_facet_center = Vector(
                    std::make_shared<Point>(center),
                    facet.pt_center_ptr());
                fout << "\tand\n\tplane("
                     << facet.pt_center_ptr()->x() << ", "
                     << facet.pt_center_ptr()->y() << ", "
                     << facet.pt_center_ptr()->z() << "; "
                     << vec_center_facet_center.x() << ", "
                     << vec_center_facet_center.y() << ", "
                     << vec_center_facet_center.z() << ")";
                if (facet_idx == fil_ptrs[fil_idx]->facets().size() - 1)
                     fout << "\n\tand\n\tcell;";
                fout << "\n";
            }

            if (fil_idx != 0)
                fillers_string += "\n\tor\n\tpc_filler_" + std::to_string(fil_idx);
        }
        // shells (shell == interface + filler)
        for (int shell_idx = 0; shell_idx < int_ptrs.size(); ++shell_idx) {
            Point top_facet_center =
                *(int_ptrs[shell_idx]->top_facet_center_ptr());
            Point bot_facet_center =
                *(int_ptrs[shell_idx]->bot_facet_center_ptr());
            Point center = Point(
                (top_facet_center.x() + bot_facet_center.x()) / 2,
                (top_facet_center.y() + bot_facet_center.y()) / 2,
                (top_facet_center.z() + bot_facet_center.z()) / 2);
            Vector vec_top_bottom(
                std::make_shared<Point>(top_facet_center),
                std::make_shared<Point>(bot_facet_center));
            fout << "solid pc_shell_" << shell_idx << " =\n\tplane("
                 << top_facet_center.x() << ", "
                 << top_facet_center.y() << ", "
                 << top_facet_center.z() << "; "
                 << -vec_top_bottom.x() << ", "
                 << -vec_top_bottom.y() << ", "
                 << -vec_top_bottom.z() << ")" 
                 << "\n\tand\n\tplane("
                 << bot_facet_center.x() << ", "
                 << bot_facet_center.y() << ", "
                 << bot_facet_center.z() << "; "
                 << vec_top_bottom.x() << ", "
                 << vec_top_bottom.y() << ", "
                 << vec_top_bottom.z() << ")\n";
            for (int facet_idx = 0; 
                    facet_idx < int_ptrs[shell_idx]->facets().size();
                      ++facet_idx) {
                Polygon facet = int_ptrs[shell_idx]->facets()[facet_idx];
                Vector vec_center_facet_center = Vector(
                    std::make_shared<Point>(center),
                    facet.pt_center_ptr());
                fout << "\tand\n\tplane("
                     << facet.pt_center_ptr()->x() << ", "
                     << facet.pt_center_ptr()->y() << ", "
                     << facet.pt_center_ptr()->z() << "; "
                     << vec_center_facet_center.x() << ", "
                     << vec_center_facet_center.y() << ", "
                     << vec_center_facet_center.z() << ")";
                if (facet_idx == int_ptrs[shell_idx]->facets().size() - 1)
                     fout << "\n\tand\n\tcell;\n";
                fout << "\n";
            }
            if (shell_idx != 0)
                shells_string += "\n\tor\n\tpc_shell_" + std::to_string(shell_idx);
        }
    }
    fout << fillers_string << ";" << std::endl;
    fout << shells_string << ";" << std::endl;
    fout << "\ntlo filler;\n";
    fout << "solid interface = shells and not filler;\n";
    fout << "tlo interface -transparent;\n";
    fout << "solid matrix = not shells and cell;\n";
    fout << "tlo matrix -transparent;\n";    
    fout.close();
    return;
};

#include <iostream>
#include <memory>
#include <random>
#include <string>
#include <vector>

#include "src/geometry/point.hpp"
#include "src/geometry/line_segment.hpp"
#include "src/geometry/sphere.hpp"
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

       settings file should include:
           Lx [Ly, Lz]
           radius
           shell_thickness
           spheres_number
           max_attempts
           geo_fname
    */
    if (argc < 2) {
        std::cout << "Not enough arguments\n";
        return 0;
    }
    std::string taks_name("ternary_polygonal_cylinders");
    SettingsParser sp(std::string(argv[1]), "MC");
    sp.print();
    float Lx = std::stof(sp.get_setting(std::string("Lx")));
    float Ly = Lx;
    float Lz = Lx;
    if (sp.check_setting("Ly"))
        Ly = std::stof(sp.get_setting(std::string("Ly")));
    if (sp.check_setting("Lz"))
        Ly = std::stof(sp.get_setting(std::string("Lz")));
    if (!sp.check_setting("radius"))
        throw MyException("No radius specified");
    float r = std::stof(sp.get_setting(std::string("radius")));
    if (!sp.check_setting("shell_thickness"))
        throw MyException("No shell thickness specified");
    float shell_thickness = std::stof(
        sp.get_setting(std::string("shell_thickness")));
    if (!sp.check_setting("spheres_number"))
        throw MyException("No spheres_number specified");
    int spheres_number = std::stoi(sp.get_setting(std::string("spheres_number")));
    if (!sp.check_setting("max_attempts"))
        throw MyException("No max attempts specified");
    int max_attempts = std::stoi(sp.get_setting(std::string("max_attempts")));
    if (!sp.check_setting("geo_fname"))
        throw MyException("No fname specified");
    std::string fname = sp.get_setting(std::string("geo_fname"));
    std::string output_fname = sp.get_setting(std::string("output_fname"));
    std::string LOG = std::string("LOG");
    if (sp.check_setting("LOG"))
        LOG = sp.get_setting(std::string("LOG"));

    //making system
    int N_done = 0;
    std::vector<Sphere> sphs;
    {
        std::cout << "Making system\n";
        int current_attempt = 0;
        while (current_attempt++ < max_attempts)
          {
            float x = static_cast<float>(rand()) / RAND_MAX * Lx;
            float y = static_cast<float>(rand()) / RAND_MAX * Ly;
            float z = static_cast<float>(rand()) / RAND_MAX * Lz;
            if (x-r < 0 || x+r > Lx || y-r < 0 || y+r > Ly || z-r < 0 || z+r > Lz)
                continue;
            bool crosses_existing = false;
            for (auto &old : sphs)
              {
                if (old.crosses_with_sphere(Sphere(x,y,z, r)))
                  {
                    crosses_existing = true;
                    break;
                  }
              }
            if (!crosses_existing)
              {
                sphs.push_back(Sphere(x,y,z, r));
                N_done++;
              }
            if (N_done == spheres_number)
              break;
          }
        std::cout << "Made " << N_done << " spheres\n";
    }
    // output
    {
        std::ofstream fout;
        fout.open(fname);

        fout << "algebraic3d\n\n";
        fout << "solid cell = orthobrick(0, 0, 0; "
             << Lx << ", " << Ly << ", " << Lz << ");\n\n";

        for (int idx = 0; idx < N_done; ++idx) {
            fout << "solid fil_" << idx << " = sphere("
                 << sphs[idx].x_c() << ", "
                 << sphs[idx].y_c() << ", "
                 << sphs[idx].z_c() << "; "
                 << sphs[idx].r() << ");\n";

            fout << "solid sh_" << idx << " = sphere("
                 << sphs[idx].x_c() << ", "
                 << sphs[idx].y_c() << ", "
                 << sphs[idx].z_c() << "; "
                 << sphs[idx].r() + shell_thickness << ");\n";
        }

        fout << "\nsolid filler = ";
        for (int idx = 0; idx < N_done; ++idx)
          {
            fout << "fil_" << idx;
            if (idx != N_done - 1)
                fout << " or ";
          }
        fout << ";";

        fout << "\nsolid interface = ";
        for (int idx = 0; idx < N_done; ++idx)
          {
            fout << "sh_" << idx;
            if (idx != N_done - 1)
                fout << " or ";
          }
        fout << ";";
        fout << "\nsolid matrix = cell and not filler and not interface;\n\n";

        fout << "tlo filler\n;";
        fout << "tlo interface\n;";
        fout << "tlo matrix\n;";
    }
    // logging
    /*{
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
    }*/
    return 0;
}

#include "random_polygonal_cylinders.hpp"


float SystemArchitector::random_polygonal_cylinders(
    float Lx, float Ly, float Lz, int vertices_number, float thickness,
    float shell_thickness, float outer_radius, int disks_number,
    int max_attempts) {
        std::cout << "--polygonal_start--\n";
        float central_angle = M_PI / vertices_number;
        float edge_length = outer_radius * 2  * sin(central_angle);
        float inner_radius = edge_length / 2 / tan(central_angle);
        std::cout << "*** system parameters ***\n"
                  << "\tfillers, shells: polygonal cylinders\n"
                  << "\tcell: brick, Lx = "
                      << Lx << ", Ly = " << Ly << ", Lz = " << Lz
                  << "\taspect ratio = " << 2 * inner_radius / thickness
                      << std::endl
                  << "\tnumber of filler particles = " << disks_number << std::endl
                  << "\touter radius = " << outer_radius << std::endl
                  << "\tinner radius = " << inner_radius << std::endl
                  << "\tthickness = " << thickness << std::endl
                  << "\tshell thickness = " << shell_thickness << std::endl
                  << "\tmax attempts = " << max_attempts << std::endl
              << "*** system parameters end ***" << std::endl;
        // starting to create initial configuration
        std::cout << "\n\n\t*** algorythm starts ***\n\n";
        int attempt = 0;
        srand(time(NULL));
        while(fils.size() < disks_number && ++attempt < max_attempts) {
            std::cout << "Attempt: " << attempt
                      << " / " << max_attempts << std::endl;
            std::shared_ptr<PolygonalCylinder> next_fil_ptr =
                std::make_shared<PolygonalCylinder>(
                    vertices_number, thickness, outer_radius);
            float dx = static_cast<float>(rand()) / RAND_MAX * Lx;
            float dy = static_cast<float>(rand()) / RAND_MAX * Ly;
            float dz = static_cast<float>(rand()) / RAND_MAX * Lz;
            float alpha = static_cast<float>(rand()) / RAND_MAX;
            float beta = static_cast<float>(rand()) / RAND_MAX;
            float gamma = static_cast<float>(rand()) / RAND_MAX;
            next_fil_ptr->rotate_around_x(M_PI * alpha);
            next_fil_ptr->rotate_around_y(M_PI * beta);
            next_fil_ptr->rotate_around_z(M_PI * gamma);
            next_fil_ptr->translate(dx, dy, dz);
            if (next_fil_ptr->crosses_box(Lx, Ly, Lz))
                continue;
            bool flag_intersection_with_some_old = false;
            for(auto old_fil : fils)
                if (next_fil_ptr->crosses_other_polygonal_cylinder(old_fil)) {
                    flag_intersection_with_some_old = true;
                    break;
                }
            if(flag_intersection_with_some_old)
                continue;
            fils.push_back(next_fil_ptr);
            std::shared_ptr<PolygonalCylinder> shell_ptr =
                std::make_shared<PolygonalCylinder>(
                    vertices_number, thickness + 2 * shell_thickness,
                    outer_radius + shell_thickness);
            shell_ptr->rotate_around_x(M_PI * alpha);
            shell_ptr->rotate_around_y(M_PI * beta);
            shell_ptr->rotate_around_z(M_PI * gamma);
            shell_ptr->translate(dx, dy, dz);
            shells.push_back(shell_ptr);
        }
    std::cout << "\n\n\t*** algorithm finished ***\n\n";
    float pc_volume = 0.5 * outer_radius*outer_radius*sin(central_angle) *
                      vertices_number;
    float shVolume =  0.5 * (outer_radius + shell_thickness) *
                            (outer_radius + shell_thickness) *
                            sin(central_angle) *
                      vertices_number;
    float cube_volume = Lx * Ly * Lz;
    std::cout << "volume fraction of filler = "
              << pc_volume * fils.size() / cube_volume << std::endl;
    std::cout << "number of particles in system = " << fils.size() << ", ";
    std::cout << "number of attempts made = " << attempt << std::endl;
    std::cout << "--polygonal_end--\n";
    return 0;
}

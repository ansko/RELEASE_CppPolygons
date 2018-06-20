#include "ternary_polygonal_system.hpp"


bool TernaryPolygonalSystem::random_polygonal_cylinders(
    float Lx, float Ly, float Lz, int vertices_number, float thickness,
    float shell_thickness, float outer_radius, int fillers_number,
    int max_attempts) {
        std::cout << "--polygonal_start--\n";
        _Lx = Lx;
        _Ly = Ly;
        _Lz = Lz;
        float central_angle = M_PI / vertices_number;
        float edge_length = outer_radius * 2  * sin(central_angle);
        float inner_radius = edge_length / 2 / tan(central_angle);
        std::cout << "*** system parameters ***\n"
                  << "\tfillers, shells: polygonal cylinders\n"
                  << "\tcell: brick, Lx = "
                      << Lx << ", Ly = " << Ly << ", Lz = " << Lz << std::endl
                  << "\taspect ratio = " << 2 * inner_radius / thickness
                      << std::endl
                  << "\tnumber of filler particles = " << fillers_number
                      << std::endl
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
        while(_fillers.size() < fillers_number && ++attempt < max_attempts) {
            std::cout << "Attempt: " << attempt
                      << " / " << max_attempts;// << std::endl;
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
            if (next_fil_ptr->crosses_box(Lx, Ly, Lz)) {
                std::cout << " crosses box\n";
                continue;
            }
            bool flag_intersection_with_some_old = false;
            for(std::shared_ptr<PolygonalCylinder> old_fil : _fillers)
                if (next_fil_ptr->crosses_other_polygonal_cylinder(old_fil)) {
                    std::cout << " crosses other filler\n";
                    flag_intersection_with_some_old = true;
                    break;
                }
            if(flag_intersection_with_some_old)
                continue;
            _fillers.push_back(next_fil_ptr);
            std::shared_ptr<PolygonalCylinder> shell_ptr =
                std::make_shared<PolygonalCylinder>(
                    vertices_number, thickness + 2 * shell_thickness,
                    outer_radius + shell_thickness);
            shell_ptr->rotate_around_x(M_PI * alpha);
            shell_ptr->rotate_around_y(M_PI * beta);
            shell_ptr->rotate_around_z(M_PI * gamma);
            shell_ptr->translate(dx, dy, dz);
            _shells.push_back(shell_ptr);
            std::cout << " appended\n";
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
              << pc_volume * _fillers.size() / cube_volume << std::endl;
    std::cout << "number of particles in system = " << _fillers.size() << ", ";
    std::cout << "number of attempts made = " << attempt << std::endl;
    std::cout << "--polygonal_end--\n";
    if (_fillers.size() == fillers_number)
        return true;
    return false;
}


bool TernaryPolygonalSystem::signle_tactoid(
    float Lx, float Ly, float Lz, int vertices_number, float thickness,
    float shell_thickness, float outer_radius, int fillers_number) {
        std::cout << "--polygonal_start--\n";
        _Lx = Lx;
        _Ly = Ly;
        _Lz = Lz;
        float spacing = thickness / 10;
        float central_angle = M_PI / vertices_number;
        float edge_length = outer_radius * 2  * sin(central_angle);
        float inner_radius = edge_length / 2 / tan(central_angle);
        std::cout << "*** system parameters ***\n"
                  << "\tfillers, shells: polygonal cylinders\n"
                  << "\tcell: brick, Lx = "
                      << Lx << ", Ly = " << Ly << ", Lz = " << Lz << std::endl
                  << "\taspect ratio = " << 2 * inner_radius / thickness
                      << std::endl
                  << "\tnumber of filler particles = " << fillers_number
                      << std::endl
                  << "\touter radius = " << outer_radius << std::endl
                  << "\tinner radius = " << inner_radius << std::endl
                  << "\tthickness = " << thickness << std::endl
                  << "\tshell thickness = " << shell_thickness << std::endl
              << "*** system parameters end ***" << std::endl;
        // starting to create initial configuration
        std::cout << "\n\n\t*** algorythm starts ***\n\n";
        int attempt = 0;
        srand(time(NULL));
        float Lz_min = thickness * fillers_number;
        if (_Lz < Lz_min) {
            std::cout << "Warning: too small Lz or too big fillers_number\n";
            return false;
        }
        float interlayer;
        if (fillers_number == 1)
            interlayer = 0;
        else
            interlayer = (_Lz - Lz_min - 2*spacing) / (fillers_number - 1);
        for (int idx = 0; idx < fillers_number; ++idx) {
            PolygonalCylinder filler(vertices_number, thickness, outer_radius);
            PolygonalCylinder shell(vertices_number,
                thickness + 2 * shell_thickness, outer_radius + shell_thickness);
            float dx = _Lx/2;
            float dy = _Ly/2;
            float dz = thickness/2 + (thickness + interlayer) * idx;
            if (idx == 0)
                dz += spacing;
            filler.translate(dx, dy, dz);
            shell.translate(dx, dy, dz);
            _fillers.push_back(std::make_shared<PolygonalCylinder>(filler));
            _shells.push_back(std::make_shared<PolygonalCylinder>(shell));
        }
        std::cout << "\n\n\t*** algorythm ended ***\n\n";
        if (_fillers.size() == fillers_number)
            return true;
        return false;
}


std::vector<std::shared_ptr<PolygonalCylinder> >
    TernaryPolygonalSystem::shells() {
        return _shells;
}

std::vector<std::shared_ptr<PolygonalCylinder> >
    TernaryPolygonalSystem::fillers() {
        return _fillers;
}

float TernaryPolygonalSystem::Lx() {
    return _Lx;
}

float TernaryPolygonalSystem::Ly() {
    return _Ly;
}

float TernaryPolygonalSystem::Lz() {
    return _Lz;
}


bool TernaryPolygonalSystem::mix() {
    float big = 1;
    float dx = static_cast<float>(rand() - RAND_MAX/2) / RAND_MAX * _Lx / big;
    float dy = static_cast<float>(rand() - RAND_MAX/2) / RAND_MAX * _Ly / big;
    float dz = static_cast<float>(rand() - RAND_MAX/2) / RAND_MAX * _Lz / big;
    float alpha = static_cast<float>(rand()) / RAND_MAX / big;
    float beta = static_cast<float>(rand()) / RAND_MAX / big;
    float gamma = static_cast<float>(rand()) / RAND_MAX / big;
    for (int idx = 0; idx < _fillers.size(); ++idx) {
        auto fil = _fillers[idx];
        auto top_center = fil->top_facet_center_ptr();
        auto bot_center = fil->bot_facet_center_ptr();
        float dx_to_zero = -(top_center->x() + bot_center->x())/2;
        float dy_to_zero = -(top_center->y() + bot_center->y())/2;
        float dz_to_zero = -(top_center->z() + bot_center->z())/2;
        auto moved_fil = (*fil);
        moved_fil.translate(dx_to_zero, dy_to_zero, dz_to_zero);
        moved_fil.rotate_around_x(alpha);
        moved_fil.rotate_around_y(beta);
        moved_fil.rotate_around_z(gamma);
        moved_fil.translate(dx, dy, dz);
        moved_fil.translate(-dx_to_zero, -dy_to_zero, -dz_to_zero);
        if (moved_fil.crosses_box(_Lx, _Ly, _Lz))
            continue;
        bool flag_intersection_moved = false;
        for (int idx_old = 0; idx_old < _fillers.size(); ++idx_old) {
            if (idx_old == idx)
                continue;
            if (_fillers[idx_old]->crosses_other_polygonal_cylinder(
                    std::make_shared<PolygonalCylinder>(moved_fil))) {
                flag_intersection_moved = true;
                break;
            }
        }
        if (!flag_intersection_moved) {
            _fillers[idx] = std::make_shared<PolygonalCylinder>(moved_fil);
            _shells[idx]->translate(dx_to_zero, dy_to_zero, dz_to_zero);
            _shells[idx]->rotate_around_x(alpha);
            _shells[idx]->rotate_around_y(beta);
            _shells[idx]->rotate_around_z(gamma);
            _shells[idx]->translate(dx, dy, dz);
            _shells[idx]->translate(-dx_to_zero, -dy_to_zero, -dz_to_zero);
            return true;
        }
    }
    return false;
}
#include "binary_polygonal_system.hpp"


BinaryPolygonalSystem::BinaryPolygonalSystem() {
}

BinaryPolygonalSystem::BinaryPolygonalSystem
    (std::vector<std::vector<PolygonalPolyhedron> > phases)
        : PolyhedronalSystem(phases) {
       _matrix = Orthobrick(phases[0][0]);
       const int fillers_number = phases[1].size();
       _fillers.reserve(fillers_number);
       for (int idx = 0; idx < fillers_number; ++idx)
           _fillers[idx] = phases[1][idx];
       _phases_number = 2;
}


const bool BinaryPolygonalSystem::random_polygonal_cylinders(
    const float Lx, const float Ly, const  float Lz,
    const int vertices_number, const float thickness,
    const float outer_radius, const int fillers_number,
    const int max_attempts) {
        std::cout << "--polygonal_start--\n";
        _Lx = Lx;
        _Ly = Ly;
        _Lz = Lz;
        float central_angle = M_PI / vertices_number;
        float edge_length = outer_radius * 2  * sin(central_angle);
        float inner_radius = edge_length / 2 / tan(central_angle);
        std::cout << "*** system parameters ***\n"
                  << "\tfillers: polygonal cylinders\n"
                  << "\tcell: brick, "
                      << "Lx = " << Lx << ", "
                      << "Ly = " << Ly << ", "
                      << "Lz = " << Lz << std::endl
                  << "\taspect ratio = " << 2 * inner_radius / thickness
                      << std::endl
                  << "\tnumber of filler particles = " << fillers_number
                      << std::endl
                  << "\touter radius = " << outer_radius << std::endl
                  << "\tinner radius = " << inner_radius << std::endl
                  << "\tthickness = " << thickness << std::endl
                  << "\tmax attempts = " << max_attempts << std::endl
              << "*** system parameters end ***" << std::endl;
        // starting to create initial configuration
        std::cout << "\n\n\t*** algorythm starts ***\n\n";
        int attempt = 0;
        srand(time(NULL));
        while(_fillers.size() < fillers_number && ++attempt < max_attempts) {
            std::cout << "Attempt: " << attempt
                      << " / " << max_attempts;// << std::endl;
            PolygonalCylinder next_fil(vertices_number, thickness, outer_radius);
            float dx = static_cast<float>(rand()) / RAND_MAX * Lx;
            float dy = static_cast<float>(rand()) / RAND_MAX * Ly;
            float dz = static_cast<float>(rand()) / RAND_MAX * Lz;
            float alpha = static_cast<float>(rand()) / RAND_MAX;
            float beta = static_cast<float>(rand()) / RAND_MAX;
            float gamma = static_cast<float>(rand()) / RAND_MAX;
            next_fil.rotate_around_x(M_PI * alpha);
            next_fil.rotate_around_y(M_PI * beta);
            next_fil.rotate_around_z(M_PI * gamma);
            next_fil.translate(dx, dy, dz);
            if (next_fil.crosses_box(Lx, Ly, Lz)) {
                std::cout << " crosses box\n";
                continue;
            }
            bool flag_intersection_with_some_old = false;
            bool flag_some_old_intersects_me = false;

            for(auto old_fil : _fillers)
                if (next_fil.crosses_other_polygonal_cylinder(old_fil)) {
                    std::cout << " crosses other filler\n";
                    flag_intersection_with_some_old = true;
                    break;
                }

            for(auto old_fil : _fillers)
                if (old_fil.crosses_other_polygonal_cylinder(next_fil)) {
                    flag_some_old_intersects_me = true;
                    break;
                }


            if(flag_intersection_with_some_old)
                continue;

            if (flag_some_old_intersects_me)
                std::cout << "me_other = " << flag_intersection_with_some_old
                                           << std::endl
                          << "other_me = " << flag_some_old_intersects_me
                                           << std::endl;

            _fillers.push_back(next_fil);
            std::cout << " appended\n";
        }
        _random_attempts_made = attempt;
        // testing inside funs
        bool flag_testing = false;
        int num_ = _fillers.size();
        for(int i = 0; i < num_; ++i)
            for(int j = 0; j < num_; ++j) {
                if (i == j)
                    continue;
                if (_fillers[i].crosses_other_polygonal_cylinder(
                        _fillers[j]))
                    flag_testing = true;
            }
        std::cout << "flag_testing (from ternary_polygonal_system) : "
                  << flag_testing << std::endl;
        std::cout << "\n\n\t*** algorithm finished ***\n\n";
        float pc_volume = 0.5 * outer_radius * outer_radius * sin(central_angle) *
                                vertices_number * thickness;
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


const bool BinaryPolygonalSystem::signle_tactoid(
    const float Lx, const float Ly, const float Lz,
    const int vertices_number, const float thickness,
    const float outer_radius, const int fillers_number) {
        std::cout << "--polygonal_start--\n";
        _Lx = Lx;
        _Ly = Ly;
        _Lz = Lz;
        float spacing = thickness / 10;
        float central_angle = M_PI / vertices_number;
        float edge_length = outer_radius * 2  * sin(central_angle);
        float inner_radius = edge_length / 2 / tan(central_angle);
        std::cout << "*** system parameters ***\n"
                  << "\tfillers: polygonal cylinders\n"
                  << "\tcell: brick, Lx = "
                      << Lx << ", Ly = " << Ly << ", Lz = " << Lz << std::endl
                  << "\taspect ratio = " << 2 * inner_radius / thickness
                      << std::endl
                  << "\tnumber of filler particles = " << fillers_number
                      << std::endl
                  << "\touter radius = " << outer_radius << std::endl
                  << "\tinner radius = " << inner_radius << std::endl
                  << "\tthickness = " << thickness << std::endl
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
            float dx = _Lx/2;
            float dy = _Ly/2;
            float dz = thickness/2 + (thickness + interlayer) * idx;
            if (idx == 0)
                dz += spacing;
            filler.translate(dx, dy, dz);
            _fillers.push_back(filler);
        }
        std::cout << "\n\n\t*** algorythm ended ***\n\n";
        if (_fillers.size() == fillers_number)
            return true;
        return false;
}


const std::vector<PolygonalCylinder> BinaryPolygonalSystem::fillers() const {
        return _fillers;
}

const float BinaryPolygonalSystem::Lx() const {
    return _Lx;
}

const float BinaryPolygonalSystem::Ly() const {
    return _Ly;
}

const float BinaryPolygonalSystem::Lz() const {
    return _Lz;
}

const int BinaryPolygonalSystem::random_attempts_made() const {
    return _random_attempts_made;
}

const bool BinaryPolygonalSystem::mix() {
    const float big = 1;
    const float dx =
        static_cast<float>(rand() - RAND_MAX/2) / RAND_MAX * _Lx / big;
    const float dy =
        static_cast<float>(rand() - RAND_MAX/2) / RAND_MAX * _Ly / big;
    const float dz =
        static_cast<float>(rand() - RAND_MAX/2) / RAND_MAX * _Lz / big;
    const float alpha = static_cast<float>(rand()) / RAND_MAX / big;
    const float beta = static_cast<float>(rand()) / RAND_MAX / big;
    const float gamma = static_cast<float>(rand()) / RAND_MAX / big;
    for (int idx = 0; idx < _fillers.size(); ++idx) {
        auto fil = _fillers[idx];
        auto top_center = fil.top_facet_center();
        auto bot_center = fil.bot_facet_center();
        float dx_to_zero = -(top_center.x() + bot_center.x())/2;
        float dy_to_zero = -(top_center.y() + bot_center.y())/2;
        float dz_to_zero = -(top_center.z() + bot_center.z())/2;
        auto moved_fil = fil;
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
            if (_fillers[idx_old].crosses_other_polygonal_cylinder(moved_fil)) {
                flag_intersection_moved = true;
                break;
            }
        }
        if (!flag_intersection_moved) {
            _fillers[idx] = moved_fil;
            return true;
        }
    }
    return false;
}

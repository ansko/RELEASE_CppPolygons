#include "sphere.hpp"


const float Sphere::x_c() const
{
    return this->x_;
}

const float Sphere::y_c() const
{
    return this->y_;
}

const float Sphere::z_c() const
{
    return this->z_;
}

const float Sphere::r() const
{
    return this->r_;
}


const bool Sphere::crosses_with_sphere(const Sphere &sph)
{
    double dx = sph.x_c() - this->x_,
           dy = sph.y_c() - this->y_,
           dz = sph.z_c() - this->z_,
           r_sum = r_ + sph.r();
    return (dx*dx + dy*dy + dz*dz < r_sum*r_sum);
}

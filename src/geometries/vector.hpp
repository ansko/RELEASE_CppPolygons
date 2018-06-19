#pragma once

#include <cmath>
#include <iostream>
#include <memory>
#include <vector>

#include "../algebra/matrix.hpp"
#include "point.hpp"


class Vector {
public:
    Vector();
    Vector(std::shared_ptr<Point> pt_end_ptr);
    Vector(std::shared_ptr<Point> pt_beg_ptr, std::shared_ptr<Point> pt_end_ptr);
    Vector(float x, float y, float z);

    Vector operator+(Vector otherVector);
    Vector operator-(Vector otherVector);
    Vector operator*(float coefficient);
    Vector operator/(float coefficient);

    float x();
    void set_x(float x_to_set);
    float y();
    void set_y(float y_to_set);
    float z();
    void set_z(float z_to_set);

    float length();

    std::shared_ptr<Vector> cross_product_ptr(
        std::shared_ptr<Vector> other_vector_ptr);
    float scalar_product(std::shared_ptr<Vector> other_vector_ptr);
    std::shared_ptr<Vector> matrix_product_ptr(Matrix m);
private:
    std::shared_ptr<Point> _pt_beg_ptr, _pt_end_ptr;
};

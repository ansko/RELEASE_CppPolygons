#include "vector.hpp"


Vector::Vector() {
    _pt_beg_ptr = std::make_shared<Point>();
    _pt_end_ptr = std::make_shared<Point>();
};

Vector::Vector(std::shared_ptr<Point> pt_beg_ptr,
                 std::shared_ptr<Point> pt_end_ptr) {
    _pt_beg_ptr = std::make_shared<Point>();
    _pt_end_ptr = std::make_shared<Point>(pt_end_ptr->x() - pt_beg_ptr->x(),
                                          pt_end_ptr->y() - pt_beg_ptr->y(),
                                          pt_end_ptr->z() - pt_beg_ptr->z());
}

Vector::Vector(std::shared_ptr<Point> pt_end_ptr) {
    _pt_beg_ptr = std::make_shared<Point>();
    _pt_end_ptr = pt_end_ptr;
}

Vector::Vector(float x, float y, float z) {
    _pt_beg_ptr = std::make_shared<Point>();
    _pt_end_ptr = std::make_shared<Point>(x, y, z);
}


Vector Vector::operator+(Vector otherVector) {
    float x = this->x() + otherVector.x();
    float y = this->y() + otherVector.y();
    float z = this->z() + otherVector.z();
    return Vector(x, y, z);
}

Vector Vector::operator-(Vector otherVector) {
    float x = this->x() - otherVector.x();
    float y = this->y() - otherVector.y();
    float z = this->z() - otherVector.z();
    return Vector(x, y, z);
}

Vector Vector::operator*(float coefficient) {
    if (std::abs(coefficient) < std::numeric_limits<float>::epsilon())
        std::cout << "Warning: multiplication by 0\n";
    float x = this->x() * coefficient;
    float y = this->y() * coefficient;
    float z = this->z() * coefficient;
    return Vector(x, y, z);
}

Vector Vector::operator/(float coefficient) {
    if (std::abs(coefficient) < std::numeric_limits<float>::epsilon())
        std::cout << "Warning: division by 0\n";
    float x = this->x() / coefficient;
    float y = this->y() / coefficient;
    float z = this->z() / coefficient;
    return Vector(x, y, z);
}


float Vector::x() {
    return _pt_end_ptr->x();
}

void Vector::set_x(float x_to_set) {
    _pt_end_ptr->set_x(x_to_set);
}

float Vector::y() {
    return _pt_end_ptr->y();
}

void Vector::set_y(float y_to_set) {
    _pt_end_ptr->set_y(y_to_set);
}

float Vector::z() {
    return _pt_end_ptr->z();
}

void Vector::set_z(float z_to_set) {
    _pt_end_ptr->set_z(z_to_set);
}

float Vector::length() {
    float dx = _pt_end_ptr->x(),
          dy = _pt_end_ptr->y(),
          dz = _pt_end_ptr->z();
    return sqrt(dx*dx + dy*dy + dz*dz);
}


std::shared_ptr<Vector> Vector::cross_product_ptr(
    std::shared_ptr<Vector> other_vector_ptr) {
        float x = this->y() * other_vector_ptr->z() -
                  this->z() * other_vector_ptr->y();
        float y = this->z() * other_vector_ptr->x() -
                  this->x() * other_vector_ptr->z();
        float z = this->x() * other_vector_ptr->y() -
                  this->y() * other_vector_ptr->x();
        return std::make_shared<Vector>(x, y, z);
}

float Vector::scalar_product(std::shared_ptr<Vector> other_vector_ptr) {
    return this->x() * other_vector_ptr->x() +
           this->y() * other_vector_ptr->y() +
           this->z() * other_vector_ptr->z();
}

std::shared_ptr<Vector> Vector::matrix_product_ptr(Matrix m) {
    if (m.columns_number() < 3 || m.rows_number() < 3) {
        std::cout << "warning: "
                  << "trying to multiply vector with too small matrix";
        return nullptr;
    }
    float x = (m[0][0] * _pt_end_ptr->x() +
               m[1][0] * _pt_end_ptr->y() +
               m[2][0] * _pt_end_ptr->z() +
               m[3][0] * 1);
    float y = (m[0][1] * _pt_end_ptr->x() +
               m[1][1] * _pt_end_ptr->y() +
               m[2][1] * _pt_end_ptr->z() +
               m[3][1] * 1);
    float z = (m[0][2] * _pt_end_ptr->x() +
               m[1][2] * _pt_end_ptr->y() +
               m[2][2] * _pt_end_ptr->z() +
               m[3][2] * 1);
    return std::make_shared<Vector>(x, y, z);
}

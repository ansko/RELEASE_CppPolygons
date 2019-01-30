#include "vector.hpp"


Vector::Vector(const Matrix &m) : Matrix(1, 3) {
    if (m.columns_number() != 3 or m.rows_number() != 1)
        throw MyException(
            std::string("Vector::Vector(Matrix): incorrect matrix size"));
    _x = m[0][0];
    _y = m[0][1];
    _z = m[0][2];
}

Vector::Vector(const Point& pt_end, const Point &pt_beg)
 : Matrix(1, 3) {
    _x = pt_end.x() - pt_beg.x();
    _y = pt_end.y() - pt_beg.y();
    _z = pt_end.z() - pt_beg.z();
}

Vector::Vector(const float x, const float y, const float z) : Matrix(1, 3) {
    _x = x;
    _y = y;
    _z = z;
}


const Vector Vector::operator+(const Vector &other) const {
    float x = _x + other.x();
    float y = _y + other.y();
    float z = _z + other.z();
    return Vector(x, y, z); 
}

const Vector Vector::operator-(const Vector &other) const {
    float x = _x - other.x();
    float y = _y - other.y();
    float z = _z - other.z();
    return Vector(x, y, z);
}

const Vector Vector::operator*(const float  number) const {
    if (std::abs(number) < std::numeric_limits<float>::epsilon())
        std::cout << "Warning: multiplication by 0\n";
    float x = _x * number;
    float y = _y * number;;
    float z = _z * number;;
    return Vector(x, y, z);
}

const Vector Vector::operator*(const Matrix &m) const {
    if (m.columns_number() < 3 || m.rows_number() < 3)
        throw MyException(
            std::string("Vector::operator*(Matrix): incorrect matrix size"));
    float x = m[0][0] * _x + m[1][0] * _y + m[2][0] * _z + m[3][0] * 1,
          y = m[0][1] * _x + m[1][1] * _y + m[2][1] * _z + m[3][1] * 1,
          z = m[0][2] * _x + m[1][2] * _y + m[2][2] * _z + m[3][2] * 1;
    return Vector(x, y, z);
}

const Vector Vector::operator/(const float number) const {
    if (std::abs(number) < std::numeric_limits<float>::epsilon())
        throw MyException(std::string("Vector::operator/: division by 0"));
    float x = _x * number;
    float y = _y * number;
    float z = _z * number;
    return Vector(x, y, z);
}


const float Vector::x() const {
    return _x;
}

const float Vector::y() const {
    return _y;
}

const float Vector::z() const {
    return _z;
}

const float Vector::length() const {
    return sqrt(_x*_x + _y*_y + _z*_z);
}


const Vector Vector::cross_product(const Vector &other) const {
    return Vector(_y * other.z() - _z * other.y(),
                  _z * other.x() - _x * other.z(),
                  _x * other.y() - _y * other.x());
}

const float Vector::scalar_product(const Vector &other) const {
    return _x * other.x() + _y * other.y() + _z * other.z();
}


void Vector::update(const float x, const float y, const float z) {
    _x = x;
    _y = y;
    _z = z;
};

void Vector::translate(const float dx, const float dy, const float dz) {
    _x += dx;
    _y += dy;
    _z += dz;
};

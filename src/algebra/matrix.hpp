#pragma once

#include <cmath>     // for abs(float)

#include <iomanip>   // for print()
#include <iostream>
#include <limits>
#include <memory>
#include <vector>

#include <stdarg.h>


#include "../error_handling/my_exception.hpp"


enum class MatrixMode {
    fastest_construction = 0,
    fastest_manipulation = 1,
    balanced = 2
};

enum class DeterminantCalculationMode {
    top_row = 0,
    left_column = 1
};


class Matrix
{
public:
    Matrix(int rows_number, int columns_number,
        MatrixMode mode=MatrixMode::balanced);
    Matrix(const std::vector<std::vector<float> > &elements);

    const Matrix operator*(const float number) const;
    const Matrix operator*(const Matrix &other) const;

    std::vector<float> operator[](const int idx) const;

    const int rows_number() const;
    const int columns_number() const;

    const bool is_square();
    const bool is_diagonal();
    const bool is_identity();
    const bool is_zero();

    void print() const;

    Matrix get_minor(const int minor_row_idx, const int minor_column_idx)
        const;
    const float det(
        DeterminantCalculationMode mode=DeterminantCalculationMode::top_row)
        const;
private:
    std::vector<std::vector<float> > _elements;
    /* Properties */
    bool _det_calculated;
    bool _det;
    int _rows_number, _columns_number;
    /* Different checks */
    bool _is_square_checked = false;
    bool _is_square = false;
    bool _is_diagonal_checked = false;
    bool _is_diagonal = false;
    bool _is_identity_checked = false;
    bool _is_identity = false;
    bool _is_zero_checked = false;
    bool _is_zero = false;
    /* Utilities */
    /*      for easy checks (is_square and so on) */
    enum class _CheckedParameter {
        square = 0,
        diagonal = 1,
        identity = 2,
        zero = 3
    };
    void _set_checked(_CheckedParameter parameter, bool value_to_set);
};

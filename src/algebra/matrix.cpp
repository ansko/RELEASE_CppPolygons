#include "matrix.hpp"


Matrix::Matrix(int rows_number, int columns_number, MatrixMode mode)
    throw (MyException) {
    /*
    Create a matrix with rows_number rows and columns_number columns
        filled with zeros.
    Input parameters:
        n - rows number
        m - columns number
        mode - matrix working mode
          MatrixMode::fastest_construction - minimal constructor, all properties
              are not set (flags equal false by default)
          MatrixMode::fastest_manipulation - maximal constructor that performs all
              possible checks
          MatrixMode::balanced - only fast checks are performed (marked with +):
            is_square()    +
            is_diagonal()  -
    */
    if (rows_number <= 0 || columns_number <= 0)
        throw MyException(std::string("incorrect matrix size"));
    for (int row_idx = 0; row_idx < rows_number; ++row_idx)
        this->_elements_ptr->push_back(std::vector<float>(columns_number, 0.0));
    this->_rows_number = rows_number;
    this->_columns_number = columns_number;
    if (mode != MatrixMode::fastest_construction)
        is_square();
    if (mode == MatrixMode::fastest_manipulation);
}

Matrix::Matrix(std::shared_ptr<std::vector<std::vector<float> > > elements_ptr)
        throw (MyException) {
    int rows_number = elements_ptr->size();
    if (rows_number <= 0)
        throw MyException(std::string("incorrect matrix size"));
    int columns_number = (*elements_ptr)[0].size();
    if (columns_number <= 0)
        throw MyException(std::string("incorrect matrix size"));
    for (int idx = 0; idx < rows_number; ++idx)
        if ((*elements_ptr)[idx].size() != columns_number)
            throw MyException(std::string("incorrect matrix form"));
    _elements_ptr = elements_ptr;
    _rows_number = rows_number;
    _columns_number = columns_number;
}

std::vector<float> Matrix::operator[](int idx) {
    return (*_elements_ptr)[idx];
}


int Matrix::rows_number() {
    return _rows_number;
}

int Matrix::columns_number() {
    return _columns_number;
}

bool Matrix::_set_checked(_CheckedParameter parameter, bool value_to_set)
throw (MyException) {
    if (parameter == _CheckedParameter::square) {
        _is_square_checked = true;
        _is_square = value_to_set;
    }
    else if (parameter == _CheckedParameter::diagonal) {
        _is_diagonal_checked = true;
        _is_diagonal = value_to_set;
    }
    else if (parameter == _CheckedParameter::identity) {
        _is_identity_checked = true;
        _is_identity = value_to_set;
    }
    else if (parameter == _CheckedParameter::zero) {
        _is_zero_checked = true;
        _is_zero = value_to_set;
    }
    else
        throw MyException(std::string("Check of unknown parameter"));
    return value_to_set;
}

bool Matrix::is_square() {
    if (_is_square_checked)
        return _is_square;
    _set_checked(_CheckedParameter::square,
        this->_rows_number == this->_columns_number);
}

bool Matrix::is_diagonal() {
    if (_is_diagonal_checked)
        return _is_diagonal;
    if (!is_square())
        return _set_checked(_CheckedParameter::diagonal, false);
    for (int row_idx = 0; row_idx < _rows_number; ++row_idx)
        for (int column_idx = 0; column_idx < _columns_number; ++column_idx)
            if (std::abs((*_elements_ptr)[row_idx][column_idx]) >=
                    std::numeric_limits<float>::epsilon())
                return _set_checked(_CheckedParameter::diagonal, false);
    return _set_checked(_CheckedParameter::square, true);
}

bool Matrix::is_identity() {
    if (_is_identity_checked)
        return _is_identity;
    if (!is_diagonal())
        return _set_checked(_CheckedParameter::diagonal, false);
    for (int idx = 0; idx <= _rows_number; ++idx)
        if (std::abs((*_elements_ptr)[idx][idx] - 1) >=
                std::numeric_limits<float>::epsilon())
            return _set_checked(_CheckedParameter::identity, false);
    return _set_checked(_CheckedParameter::identity, true);
}

bool Matrix::is_zero() {
    if (_is_zero_checked)
        return _is_zero;
    for (int row_idx = 0; row_idx < _rows_number; ++row_idx)
        for (int column_idx = 0; column_idx < _columns_number; ++column_idx)
            if (std::abs((*_elements_ptr)[row_idx][column_idx]) >=
                    std::numeric_limits<float>::epsilon())
                return _set_checked(_CheckedParameter::zero, false);
    return true;
}


void Matrix::print() {
    int too_big_matrix = 6;
    if (_rows_number > too_big_matrix || _columns_number > too_big_matrix) {
        std::cout << std::string("Matrix is too big\n");
        return;
    }
    for (int row_idx = 0; row_idx < _rows_number; ++row_idx) {
        for (int column_idx = 0; column_idx < _columns_number; ++column_idx)
            std::cout << std::setw(11) << (*_elements_ptr)[row_idx][column_idx];
        std::cout << std::endl;
    }
}

std::shared_ptr<Matrix> Matrix::get_minor(int minor_row_idx, int minor_column_idx)
        throw (MyException) {
    if (minor_row_idx >= _rows_number || minor_column_idx >= _columns_number)
        throw MyException("Index out of range");
    std::vector<std::vector<float> > minor_elements;
    for (int row_idx = 0; row_idx < _rows_number; ++row_idx) {
        if (row_idx == minor_row_idx)
            continue;
        std::vector<float> row;
        for (int column_idx = 0; column_idx < _columns_number; ++column_idx) {
            if (column_idx == minor_column_idx)
                continue;
            row.push_back((*_elements_ptr)[row_idx][column_idx]);
        }
        minor_elements.push_back(row);
    }
    return std::make_shared<Matrix>(
             std::make_shared<std::vector<std::vector<float> > >(minor_elements));
}

float Matrix::det(DeterminantCalculationMode mode) throw (MyException) {
    float result = 0;
    if (!is_square())
        throw MyException("Determinant of matix that is not square");
    if (_columns_number == 1)
        return (*_elements_ptr)[0][0];
    if (mode == DeterminantCalculationMode::top_row) {
        int row_idx = 0;
        int one = -1;
        for (int column_idx = 0; column_idx < _columns_number; ++column_idx) {
            one *= -1;
            result += (*_elements_ptr)[row_idx][column_idx] * one *
                          get_minor(row_idx, column_idx)->det();
        }
    }
    else
        throw MyException("Unknown method");
    return result;
}

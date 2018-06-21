#include "matrix.hpp"


Matrix::Matrix(const int rows_number, const int columns_number, MatrixMode mode)
    throw (MyException) {
    if (rows_number <= 0 || columns_number <= 0)
        throw MyException(std::string("incorrect matrix size"));
    for (int row_idx = 0; row_idx < rows_number; ++row_idx) {
        this->_elements.push_back(std::vector<float>(columns_number, 0.0));
    }
    this->_rows_number = rows_number;
    this->_columns_number = columns_number;
    if (mode != MatrixMode::fastest_construction)
        is_square();
}

Matrix::Matrix(const std::vector<std::vector<float> > &elements)
        throw (MyException) {
    int rows_number = elements.size();
    if (rows_number <= 0)
        throw MyException(std::string("incorrect matrix size"));
    int columns_number = elements[0].size();
    if (columns_number <= 0)
        throw MyException(std::string("incorrect matrix size"));
    for (int idx = 0; idx < rows_number; ++idx)
        if (elements[idx].size() != columns_number)
            throw MyException(std::string("incorrect matrix form"));
    _elements = elements;
    _rows_number = rows_number;
    _columns_number = columns_number;
}


const Matrix Matrix::operator*(const float number) const {
    std::vector<std::vector<float> > elements;
    for (int idx_row = 0; idx_row < _rows_number; ++idx_row) {
        std::vector<float> row;
        for (int idx_column; idx_column < _columns_number; ++idx_column)
            row.push_back(_elements[idx_row][idx_column] * number);
        elements.push_back(row);
    }
    return Matrix(elements);
}

const Matrix Matrix::operator*(const Matrix &other) const throw (MyException) {
    if (_columns_number != other.rows_number())
        throw MyException(
            std::string("Error: inconsistent size when multiplying matrices"));
    std::vector<std::vector<float> > elements;
    for (int idx_row = 0; idx_row < _rows_number; ++idx_row) {
        std::vector<float> row;
        for (int idx_column = 0; idx_column < other.columns_number();
                 ++idx_column) {
            float element = 0;
            for (int idx_sum = 0; idx_sum < _columns_number; ++idx_sum)
                element += _elements[idx_row][idx_sum] *
                           other[idx_sum][idx_column];
            row.push_back(element);
        }
        elements.push_back(row);
    }
    return Matrix(elements);
}

std::vector<float> Matrix::operator[](const int idx) const throw(MyException) {
    if (idx > _columns_number)
        throw MyException(std::string("Matrix::operator[]: index out of range"));
    return _elements[idx];
}


const int Matrix::rows_number() const {
    return _rows_number;
}

const int Matrix::columns_number() const{
    return _columns_number;
}

void Matrix::_set_checked(_CheckedParameter parameter, bool value_to_set)
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
    return;
}

const bool Matrix::is_square() {
    if (_is_square_checked)
        return _is_square;
    _set_checked(_CheckedParameter::square,
        this->_rows_number == this->_columns_number);
}

const bool Matrix::is_diagonal() {
    if (_is_diagonal_checked)
        return _is_diagonal;
    if (!is_square()) {
        _set_checked(_CheckedParameter::diagonal, false);
        return false;
    }
    for (int row_idx = 0; row_idx < _rows_number; ++row_idx)
        for (int column_idx = 0; column_idx < _columns_number; ++column_idx)
            if (std::abs(_elements[row_idx][column_idx]) >=
                    std::numeric_limits<float>::epsilon()) {
                _set_checked(_CheckedParameter::diagonal, false);
                return false;
            }
    _set_checked(_CheckedParameter::square, true);
    return true;
}

const bool Matrix::is_identity() {
    if (_is_identity_checked)
        return _is_identity;
    if (!is_diagonal()) {
        _set_checked(_CheckedParameter::diagonal, false);
        return false;
    }
    for (int idx = 0; idx <= _rows_number; ++idx)
        if (std::abs(_elements[idx][idx] - 1) >=
                std::numeric_limits<float>::epsilon()) {
            _set_checked(_CheckedParameter::identity, false);
            return false;
        }
    _set_checked(_CheckedParameter::identity, true);
    return true;
}

const bool Matrix::is_zero() {
    if (_is_zero_checked)
        return _is_zero;
    for (int row_idx = 0; row_idx < _rows_number; ++row_idx)
        for (int column_idx = 0; column_idx < _columns_number; ++column_idx)
            if (std::abs(_elements[row_idx][column_idx]) >=
                    std::numeric_limits<float>::epsilon()) {
                _set_checked(_CheckedParameter::zero, false);
                return false;
            }
    return true;
}


void Matrix::print() const {
    int too_big_matrix = 6;
    if (_rows_number > too_big_matrix || _columns_number > too_big_matrix) {
        std::cout << std::string("Matrix is too big\n");
        std::cout << _rows_number << " " << _columns_number << std::endl;
        return;
    }
    for (int row_idx = 0; row_idx < _rows_number; ++row_idx) {
        for (int column_idx = 0; column_idx < _columns_number; ++column_idx)
            std::cout << std::setw(11) << _elements[row_idx][column_idx];
        std::cout << std::endl;
    }
}

Matrix Matrix::get_minor(const int minor_row_idx, const int minor_column_idx)
        const
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
            row.push_back(_elements[row_idx][column_idx]);
        }
        minor_elements.push_back(row);
    }
    Matrix result = Matrix(minor_elements);
    return result;
}

const float Matrix::det(
    DeterminantCalculationMode mode) const throw (MyException) {
        if (_is_square_checked && !_is_square ||
                (!_is_square_checked && (_columns_number != _rows_number)))
                    throw MyException("Determinant of matix that is not square");
        if (_columns_number == 1)
            return _elements[0][0];
        float result = 0;
        if (mode == DeterminantCalculationMode::top_row) {
            int row_idx = 0;
            int one = -1;
            for (int column_idx = 0; column_idx < _columns_number; ++column_idx) {
                one *= -1;
                result += _elements[row_idx][column_idx] * one *
                              get_minor(row_idx, column_idx).det();
            }
        }
        else
            throw MyException("Unknown method");
        return result;
}

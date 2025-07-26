#pragma once
#include <iostream>
#include <vector>
#include <math.h>
#include <cstdlib>

template <class T>
class Matrix {
    private:
    std::vector<std::vector<T>> *data;
    int rows, cols;

    public:
    // Constructors
    Matrix();
    Matrix(int rows, int cols);
    Matrix(int rows, int cols, T defaultValue);
    Matrix(const std::vector<std::vector<T>>& input);
    Matrix(const Matrix<T>& other); // copy constructor

    // basic operations
    Matrix<T>& operator=(const Matrix<T>& other);
    std::vector<T>& operator[](int row);
    const std::vector<T>& operator[](int row) const;
    
    // getters
    int getRows() const;
    int getCols() const;
    bool isEmpty() const;
    bool isSquare() const;

    // arithmetic
    Matrix<T> operator + (const Matrix<T>& other) const;
    Matrix<T> operator - (const Matrix<T>& other) const;
    Matrix<T> operator * (const Matrix<T>& other) const;
    Matrix<T> operator * (T scalar) const;

    // helpers
    Matrix<T> getMinor(int excludeRow, int excludeCol) const;
    T cofactor(int row, int col) const;
    Matrix<T> adjugate() const;

    // matrix inversion
    T determinant() const;
    Matrix<T> transpose() const;
    bool isInvertible() const;
    Matrix<T> inverse() const; // guass jordan
    
    // utility functions
    static Matrix<T> identity(int size);
    void swapRows(int row1, int row2);
    void multiplyRow(int row, T factor);
    void addMultipleOfRow(int targetRow, int sourceRow, T factor);
    int findPivot(int col, int startRow) const;
    bool isNearZero(T value, T epsilon = 1e-10) const;
    
    // debugging
    void print() const;
};

// CONSTRUCTORS
template <class T>
Matrix<T>::Matrix() : data(new std::vector<std::vector<T>>()), 
    rows(0), cols(0) {}

template <class T>
Matrix<T>::Matrix(int rows, int cols):
    data(new std::vector<std::vector<T>>(rows, std::vector<T>(cols))),
    rows(rows), cols(cols) {}

template <class T>
Matrix<T>::Matrix(int rows, int cols, T defaultValue):
    data(new std::vector<std::vector<T>>(rows, std::vector<T>(cols, defaultValue))),
    rows(rows), cols(cols) {}

template <class T>
Matrix<T>::Matrix(const std::vector<std::vector<T>> &input) :
    data(new std::vector<std::vector<T>>(input)),
    rows(input.size()),
    cols(input.empty() ? 0 : input[0].size()) {}

// copy constructor
template <class T>
Matrix<T>::Matrix(const Matrix<T> &other):
    data(new std::vector<std::vector<T>>(*other.data)),
    rows(other.rows), cols(other.cols) {}

// OPERATORS
template <class T>
Matrix<T>& Matrix<T>::operator=(const Matrix<T>& other) {
    if (this != &other) {
        rows = other.rows;
        cols = other.cols;
        *data = *other.data;
    }
    return *this;
}

template <class T>
std::vector<T>& Matrix<T>::operator[](int row) {
    return (*data)[row];
}

template <class T>
const std::vector<T>& Matrix<T>::operator[](int row) const {
    return (*data)[row];
}

// GETTERS
template <class T>
int Matrix<T>::Matrix::getRows() const {
    return rows;
}

template <class T>
int Matrix<T>::Matrix::getCols() const {
    return cols;
}

template <class T>
bool Matrix<T>::Matrix::isEmpty() const {
    return (*data).empty();
}

template <class T>
bool Matrix<T>::isSquare() const {
    return rows == cols && rows > 0;
}

// ARITHMETIC OPERATORS
template <class T>
Matrix<T> Matrix<T>::operator + (const Matrix& other) const {
    if (rows != other.rows || cols != other.cols) {
        throw std::invalid_argument("Matrix dimensions must match for addition.");
    }

    Matrix<T> modified(*this);

    for (int i = 0; i<rows; i++) {
        for (int j = 0; j<cols; j++) {
            modified[i][j] += other[i][j];
        }
    }

    return modified;
}

template <class T>
Matrix<T> Matrix<T>::operator - (const Matrix& other) const {
    if (rows != other.rows || cols != other.cols) {
        throw std::invalid_argument("Matrix dimensions must match for subtraction.");
    }

    Matrix<T> modified(*this);

    for (int i = 0; i<rows; i++) {
        for (int j = 0; j<cols; j++) {
            modified[i][j] -= other[i][j];
        }
    }

    return modified;
}

template <class T>
Matrix<T> Matrix<T>::operator * (const Matrix& other) const {
    if (cols != other.rows) {
        throw std::invalid_argument("Columns of first matrix must be equal to Rows of the second matrix.");
    }

    Matrix<T> modified(rows, other.cols);
    for (int i = 0; i<rows; i++) {
        for (int j = 0; j< other.cols; j++) {
            T sum = T();
            for (int k = 0; k < cols; k++) {
                sum += (*this)[i][k] * other[k][j];
            }
            modified[i][j] = sum;
        }
    }

    return modified;
}

template <class T>
Matrix<T> Matrix<T>::operator * (T scalar) const {
    Matrix<T> modified(*this);

    for (int i = 0; i<rows; i++) {
        for (int j = 0; j<cols; j++) {
            modified[i][j] *= scalar;
        }
    }

    return modified;
}


// HELPERS
template <class T>
Matrix<T> Matrix<T>::getMinor(int excludeRow, int excludeCol) const {
    Matrix<T> minor(rows -1, cols-1);
    int mi = 0;
    for (int i = 0; i < rows; ++i) {
        if (i == excludeRow) continue;
        
        int mj = 0;
        for (int j = 0; j < cols; ++j) {
            if (j == excludeCol) continue;
            minor[mi][mj] = (*data)[i][j];
            mj+=1;
        }
        mi+=1;
    }
    return minor;
}

template <class T>
T Matrix<T>::cofactor(int row, int col) const {
    T sign = ((row + col) % 2 == 0) ? 1 : -1;
    return sign * getMinor(row, col).determinant();
}

template <class T>
Matrix<T> Matrix<T>::adjugate() const {
    if (!isSquare())
        throw std::invalid_argument("Adjugate is only defined for square matrices.");
    
    Matrix<T> adj(rows, cols);
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            adj[j][i] = cofactor(i, j); // transpose
        }
    }
    return adj;
}

// MATRIX INVERSION
template <class T>
T Matrix<T>::determinant() const {
    if (!isSquare())
        throw std::invalid_argument("Determinant is only defined for square matrices.");

    if (rows == 1)
        return (*data)[0][0];
    
    if (rows == 2)
        return (*data)[0][0] * (*data)[1][1] - (*data)[0][1] * (*data)[1][0];
    
    T det = 0;
    for (int j = 0; j < cols; ++j)
        det += (*data)[0][j] * cofactor(0, j);
    
    return det;
}

template <class T>
Matrix<T> Matrix<T>::transpose() const {
    Matrix<T> transposed(cols, rows);

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            transposed[j][i] = (*this)[i][j];
        }
    }
    return transposed;
}

template <class T>
bool Matrix<T>::isInvertible() const {
    if (!isSquare())
        return false;
    if (isEmpty())
        return false;

    return !isNearZero(determinant());
}

template <class T>
Matrix<T> Matrix<T>::inverse() const {
    // Gauss-Jordan Inverse
    if (!isSquare())
        throw std::invalid_argument("Inverse is only defined for square matrices.");

    int n = rows;
    Matrix<T> augmented(n, 2*n);
    Matrix<T> identity = Matrix<T>::identity(n);

    // left side: original matrix
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            augmented[i][j] = (*this)[i][j];
        }
    }

    // augmenting with identity matrix
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            (*augmented.data)[i][j + n] = identity[i][j];
        }
    }

    // Gauss-Jordan elimination
    for (int i = 0; i < n; i++) {
        int pivotRow = augmented.findPivot(i, i);
        if (pivotRow == -1 || isNearZero((*augmented.data)[pivotRow][i]))
            throw std::runtime_error("Matrix is singular and cannot be inverted.");

        // if pivot row not current row
        if (pivotRow != i)
            augmented.swapRows(i, pivotRow);

        // nomralising to make pivot val= 1.0
        T pivotVal = (*augmented.data)[i][i];
        augmented.multiplyRow(i, 1.0 /pivotVal);

        // modifying other rows
        for (int k = 0; k <n; k++) {
            if (k == i) continue;
            T factor = (*augmented.data)[k][i];
            augmented.addMultipleOfRow(k, i, -factor);
        }
    }

    // right half: inverse
    Matrix<T> inv(n, n);
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            inv[i][j] = (*augmented.data)[i][j + n];

    return inv;
}

// INVERSE UTILITY
template <class T>
Matrix<T> Matrix<T>::identity(int size) {
    Matrix<T> I(size, size, T(0));
    for (int i = 0; i < size; i++)
        I[i][i] = T(1);
    
    return I;
}

template <class T>
void Matrix<T>::swapRows(int row1, int row2) {
    if (row1 < 0 || row2 < 0 || row1 >= rows || row2 >= rows)
        throw std::out_of_range("Row index out of range in swapRows.");
    
    std::swap((*data)[row1], (*data)[row2]);
}

template <class T>
void Matrix<T>::multiplyRow(int row, T factor) {
    if (row < 0 || row >= rows)
        throw std::out_of_range("Row index out of range in multiplyRow.");

    for (int j = 0; j < cols; j++)
        (*data)[row][j] *= factor;
}

template <class T>
void Matrix<T>::addMultipleOfRow(int targetRow, int sourceRow, T factor) {
    if (targetRow < 0 || targetRow >= rows || sourceRow < 0 || sourceRow >= rows)
        throw std::out_of_range("Row index out of range in multiplyRow.");

    for (int j = 0; j < cols; j++)
        (*data)[targetRow][j] += factor * (*this)[sourceRow][j];
}

template <class T>
bool Matrix<T>::isNearZero(T value, T epsilon) const {
    return std::abs(value) < epsilon;
}

template <class T>
int Matrix<T>::findPivot(int col, int startRow) const {
    int pivotRow = -1;
    T maxVal = 0;
    for (int i = startRow; i < rows; ++i) {
        T val = std::abs((*data)[i][col]);
        if (val > maxVal) {
            maxVal = val;
            pivotRow = i;
        }
    }
    return pivotRow;
}

// DEBUGGING
template <class T>
void Matrix<T>::print() const {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++)
            std::cout << (*this)[i][j] << " ";
        
        std::cout << std::endl;
    }
}
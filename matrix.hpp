#pragma once
#include <vector>
#include <math.h>

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
    Matrix(const Matrix<T>& other);

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

    // matrix inversion
    T determinant() const;
    Matrix<T> transpose() const;
    bool isInvertible() const;
    Matrix<T> inverse() const;
    
    // helper functions for inversion algorithms
    Matrix<T> getMinor(int excludeRow, int excludeCol) const;
    T cofactor(int row, int col) const;
    Matrix<T> adjugate() const;
    
    // Alternative inversion methods (choose one to implement)
    Matrix<T> inverseGaussJordan() const;
    
    // Utility functions
    static Matrix<T> identity(int size);
    void swapRows(int row1, int row2);
    void multiplyRow(int row, T factor);
    void addMultipleOfRow(int targetRow, int sourceRow, T factor);
    
    // Validation and debugging
    bool isValidForInversion() const;
    void print() const;
    
    // Numerical stability helpers
    int findPivot(int col, int startRow) const;
    bool isNearZero(T value, T epsilon = 1e-10) const;
};

template <class T>
Matrix<T>::Matrix() : data(new std::vector<std::vector<T>()), 
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
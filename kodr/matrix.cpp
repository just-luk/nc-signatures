#include <mcl/bn256.hpp>
#include <matrix.hpp>
#include <stdexcept>

using namespace mcl::bn256;

Matrix::Matrix(uint rows, uint cols) {
    this->rows = rows;
    this->cols = cols;
    data.resize(rows);
    for(int i = 0; i < rows; i++) {
        data[i].resize(cols);
    }
}

Matrix::Matrix() {};

Matrix::Matrix(std::vector<std::vector<Fr>> d) {
    data = d;
    rows = d.size();
    cols = d[0].size();
}

bool Matrix::Cmp(Matrix& other) {
    if (rows != other.rows || cols != other.cols) {
        return false;
    }
    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < cols; j++) {
            if (data[i][j] != other.data[i][j]) {
                return false;
            }
        }
    }
    return true;
}

Matrix Matrix::multiply(Matrix& other) {
    if (cols != other.rows) {
        throw std::runtime_error("Matrix dimensions do not match!");
    }
    Matrix ret(rows, other.cols);
    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < other.cols; j++) {
            ret.data[i][j] = 0;
            for(int k = 0; k < cols; k++) {
                ret.data[i][j] += data[i][k] * other.data[k][j];
            }
        }
    }
    return ret;
}
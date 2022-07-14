#include <matrix.hpp>
#include <mcl/bls12_381.hpp>
#include <vector>
#include <stdexcept>

using namespace mcl::bls12;

Matrix::Matrix(int rows, int cols) {
    this->rows = rows;
    this->cols = cols;
    this->data = std::vector < std::vector < Fr >> (rows, std::vector<Fr>(cols, 0));
}

Matrix::Matrix() {};

Matrix::Matrix(std::vector <std::vector<Fr>> d) {
    this->data = d;
    this->rows = d.size();
    this->cols = d[0].size();
}

bool Matrix::Cmp(Matrix &other) {
    if (this->rows != other.rows || this->cols != other.cols) {
        return false;
    }
    for (int i = 0; i < this->rows; i++) {
        for (int j = 0; j < this->cols; j++) {
            if (this->data[i][j] != other.data[i][j]) {
                return false;
            }
        }
    }
    return true;
}

Matrix Matrix::Multiply(Matrix other) {
    if (this->cols != other.rows) {
        throw std::runtime_error("Matrix dimensions do not match!");
    }
    Matrix ret(this->rows, other.cols);
    for (int i = 0; i < this->rows; i++) {
        for (int j = 0; j < other.cols; j++) {
            ret.data[i][j] = 0;
            for (int k = 0; k < this->cols; k++) {
                ret.data[i][j] += this->data[i][k] * other.data[k][j];
            }
        }
    }
    return ret;
}
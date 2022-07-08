# pragma once

#include <mcl/bn256.hpp>
#include <stdexcept>

#ifndef MATRIX_HPP
#define MATRIX_HPP

using namespace mcl::bn256;

typedef struct Matrix {
    std::vector<std::vector<Fr>> data;
    uint rows;
    uint cols;
    Matrix(std::vector<std::vector<Fr>> d);
    Matrix();
    Matrix(uint row, uint cols);
    bool Cmp(Matrix& other);
    Matrix multiply(Matrix& other);
} Matrix;


#endif
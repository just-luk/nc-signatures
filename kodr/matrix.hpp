#pragma once

#include <mcl/bls12_381.hpp>
#include <vector>
#include <stdexcept>

#ifndef MATRIX_HPP
#define MATRIX_HPP

using namespace mcl::bls12;

typedef struct Matrix
{
    std::vector<std::vector<Fr>> data;
    int rows;
    int cols;

    Matrix(std::vector<std::vector<Fr>> d);

    Matrix();

    Matrix(int row, int cols);

    bool Cmp(Matrix &other);

    Matrix Multiply(Matrix other);
} Matrix;

#endif
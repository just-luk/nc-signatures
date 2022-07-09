#pragma once

#include <mcl/bn256.hpp>
#include <stdexcept>

#ifndef MATRIX_HPP
#define MATRIX_HPP

using namespace mcl::bn256;

typedef struct Matrix {
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
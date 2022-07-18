#pragma once

#include "data.hpp"
#include "matrix.hpp"
#include <mcl/bls12_381.hpp>
#include "vector"

#ifndef RECODER_HPP
#define RECODER_HPP

typedef struct FullRLNCRecoder
{
    std::vector<CodedPiece> pieces;
    Matrix codingMatrix;
    int pieceCount;

    FullRLNCRecoder(std::vector<CodedPiece> ps);

    CodedPiece getCodedPiece();
} FullRLNCRecoder;

#endif
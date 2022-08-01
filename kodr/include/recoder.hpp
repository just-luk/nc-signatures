#pragma once

#include "data.hpp"
#include "matrix.hpp"
#include <mcl/bls12_381.hpp>
#include "vector"
#include "boneh.hpp"

#ifndef RECODER_HPP
#define RECODER_HPP

template <typename T>
class FullRLNCRecoder
{
public:
    std::vector<CodedPiece> pieces;
    T sig;
    int pieceCount;

    FullRLNCRecoder(std::vector<CodedPiece> ps, T sig);

    FullRLNCRecoder(T sig);

    FullRLNCRecoder();

    void addPiece(CodedPiece piece);

    void clear();

    CodedPiece getCodedPiece();
};

#endif
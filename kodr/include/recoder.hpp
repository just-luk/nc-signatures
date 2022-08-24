#pragma once

#include "data.hpp"
#include "matrix.hpp"
#include <mcl/bls12_381.hpp>
#include "vector"
#include "boneh.hpp"
#include "li.hpp"

#ifndef RECODER_HPP
#define RECODER_HPP

template <typename T, typename S>
class FullRLNCRecoder
{
public:
    std::vector<CodedPiece<S>> pieces;
    T sig;
    int pieceCount;

    FullRLNCRecoder(std::vector<CodedPiece<S>> ps, T sig);

    FullRLNCRecoder(T sig);

    FullRLNCRecoder();

    void addPiece(CodedPiece<S> piece);

    void clear();

    CodedPiece<S> getCodedPiece();
};

#endif
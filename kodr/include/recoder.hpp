#pragma once

#include "data.hpp"
#include "matrix.hpp"
#include <mcl/bls12_381.hpp>
#include "vector"
#include "signature.hpp"

#ifndef RECODER_HPP
#define RECODER_HPP

typedef struct FullRLNCRecoder
{
    std::vector<CodedPiece> pieces;
    Signature *sig;
    int pieceCount;

    FullRLNCRecoder(std::vector<CodedPiece> ps, Signature *sig);

    FullRLNCRecoder(Signature *sig);

    FullRLNCRecoder();

    void addPiece(CodedPiece piece);

    void clear();

    CodedPiece getCodedPiece();
} FullRLNCRecoder;

#endif
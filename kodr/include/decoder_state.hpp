#pragma once

#include "data.hpp"
#include "matrix.hpp"
#include <mcl/bls12_381.hpp>
#include <vector>
#include <stdexcept>

#ifndef DECODER_STATE_HPP
#define DECODER_STATE_HPP

template <typename S>
class DecoderState
{
public:
    int pieceCount;
    Matrix coeffs;
    Matrix coded;

    DecoderState(Matrix cfs, Matrix pieces);

    DecoderState(int p);

    DecoderState();

    void clean_forward();

    void clean_backward();

    void remove_zero_rows();

    void Rref();

    int Rank();

    Matrix CoeffMatrix();

    Matrix CodedMatrix();

    void AddPiece(CodedPiece<S> a);

    std::vector<Fr> GetPiece(int idx);
};

#endif
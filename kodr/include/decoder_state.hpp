#pragma once

#include <data.hpp>
#include <iostream>
#include <matrix.hpp>
#include <mcl/bls12_381.hpp>
#include <vector>
#include <stdexcept>

#ifndef DECODER_STATE_HPP
#define DECODER_STATE_HPP

typedef struct DecoderState
{
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

    void AddPiece(CodedPiece a, bool isFirst);

    std::vector<Fr> GetPiece(int idx);
} DecoderState;

#endif
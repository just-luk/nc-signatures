# pragma once

#include <mcl/bn256.hpp>
#include <matrix.hpp>
#include <data.hpp>
#include <iostream>
#include <stdexcept>

#ifndef DECODER_STATE_HPP
#define DECODER_STATE_HPP

typedef struct DecoderState {
    uint pieceCount;
    Matrix coeffs;
    Matrix coded;
    DecoderState(Matrix cfs, Matrix pieces);
    DecoderState(uint p);
    DecoderState();
    void clean_forward();
    void clean_backward();
    void remove_zero_rows();
    void Rref();
    uint Rank();
    Matrix CoeffMatrix();
    Matrix CodedMatrix();
    void AddPiece(CodedPiece a, bool isFirst);
    std::vector<Fr> GetPiece(uint idx);
} DecoderState;

#endif
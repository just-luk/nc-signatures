#pragma once

#include "data.hpp"
#include "decoder_state.hpp"
#include <mcl/bls12_381.hpp>
#include <vector>
#include <stdexcept>

#ifndef DECODER_HPP
#define DECODER_HPP

typedef struct FullRLNCDecoder
{
    int expected, useful, received;
    DecoderState state;

    FullRLNCDecoder(int pieceCount);

    FullRLNCDecoder();

    int PieceLength();

    bool IsDecoded();

    int Required();

    void addPiece(CodedPiece piece, bool isFirst);

    std::vector<Fr> getPiece(int i);

    std::vector<uint8_t> getData();
} FullRLNCDecoder;

#endif
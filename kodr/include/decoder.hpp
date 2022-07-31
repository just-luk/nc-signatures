#pragma once

#include "data.hpp"
#include "decoder_state.hpp"
#include <mcl/bls12_381.hpp>
#include <vector>
#include <stdexcept>
#include <signature.hpp>

#ifndef DECODER_HPP
#define DECODER_HPP

typedef struct FullRLNCDecoder
{
    int expected, useful, received;
    DecoderState state;
    Signature *sig;

    FullRLNCDecoder(int pieceCount, Signature *sig);

    FullRLNCDecoder();

    int PieceLength();

    bool IsDecoded();

    int Required();

    void addPiece(CodedPiece piece);

    std::vector<Fr> getPiece(int i);

    std::vector<uint8_t> getData();
} FullRLNCDecoder;

#endif
#pragma once

#include "data.hpp"
#include "decoder_state.hpp"
#include <mcl/bls12_381.hpp>
#include <vector>
#include <stdexcept>
#include "boneh.hpp"

#ifndef DECODER_HPP
#define DECODER_HPP

template <typename T>
class FullRLNCDecoder
{
public:
    int expected, useful, received;
    DecoderState state;
    T sig;

    FullRLNCDecoder(int pieceCount, T sig);

    FullRLNCDecoder();

    int PieceLength();

    bool IsDecoded();

    int Required();

    void addPiece(CodedPiece piece);

    std::vector<Fr> getPiece(int i);

    std::vector<uint8_t> getData();
};

#endif
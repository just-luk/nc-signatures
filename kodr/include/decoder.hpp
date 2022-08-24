#pragma once

#include "data.hpp"
#include "decoder_state.hpp"
#include <mcl/bls12_381.hpp>
#include <vector>
#include <stdexcept>
#include "boneh.hpp"
#include "li.hpp"

#ifndef DECODER_HPP
#define DECODER_HPP

template <typename T, typename S>
class FullRLNCDecoder
{
public:
    int expected, useful, received;
    DecoderState<S> state;
    T sig;

    FullRLNCDecoder(int pieceCount, T sig);

    FullRLNCDecoder();

    int PieceLength();

    bool IsDecoded();

    int Required();

    void addPiece(CodedPiece<S> piece);

    std::vector<Fr> getPiece(int i);

    std::vector<uint8_t> getData();
};

#endif
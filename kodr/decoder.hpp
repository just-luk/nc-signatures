#pragma once

#include <data.hpp>
#include <decoder.hpp>
#include <decoder_state.hpp>
#include <mcl/bls12_381.hpp>
#include <stdexcept>

#ifndef DECODER_HPP
#define DECODER_HPP

typedef struct FullRLNCDecoder {
    int expected, useful, received;
    DecoderState state;

    FullRLNCDecoder(int pieceCount);

    int PieceLength();

    bool IsDecoded();

    int Required();

    void AddPiece(CodedPiece piece, bool isFirst);

    std::vector <Fr> GetPiece(int i);

    std::vector<unsigned char> getData();
} FullRLNCDecoder;

#endif
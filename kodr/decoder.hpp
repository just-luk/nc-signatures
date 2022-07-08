# pragma once

#include <mcl/bn256.hpp>
#include <data.hpp>
#include <decoder.hpp>
#include <decoder_state.hpp>
#include <stdexcept>

#ifndef DECODER_HPP
#define DECODER_HPP

typedef struct FullRLNCDecoder {
    uint expected, useful, received;
    DecoderState state;
    FullRLNCDecoder(uint pieceCount);
    uint PieceLength();
    bool IsDecoded();
    uint Required();
    void AddPiece(CodedPiece piece, bool isFirst);
    std::vector<Fr> GetPiece(uint i);
    std::vector<std::vector<Fr>> GetPieces();
} FullRLNCDecoder;

#endif
#pragma once

#include "data.hpp"
#include <mcl/bls12_381.hpp>
#include <vector>
#include <string>
#include "boneh.hpp"

#ifndef ENCODER_HPP
#define ENCODER_HPP

template <typename T>
class FullRLNCEncoder
{
public:
    std::vector<std::vector<Fr>> pieces;
    T sig;
    bool useSystematic;
    int pieceIndex;

    int PieceCount();

    int PieceSize();

    int DecodableLen();

    int CodedPieceLen();

    CodedPiece getCodedPiece();

    FullRLNCEncoder(std::vector<std::vector<Fr>> pieces, T sig, bool generateSystematic);

    FullRLNCEncoder(std::vector<uint8_t> data, int pieceCountOrSize, T sig, bool generateSystematic, bool fromSize = false);

    FullRLNCEncoder();
};


#endif
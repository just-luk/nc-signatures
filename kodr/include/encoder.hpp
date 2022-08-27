#pragma once

#include "data.hpp"
#include <mcl/bls12_381.hpp>
#include <vector>
#include <string>
#include "boneh.hpp"
#include "li.hpp"

#ifndef ENCODER_HPP
#define ENCODER_HPP

template <typename T, typename S>
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

    CodedPiece<S> getCodedPiece();

    void setPieces(std::vector<std::vector<Fr>> pieces);

    FullRLNCEncoder(std::vector<std::vector<Fr>> pieces, T sig, bool generateSystematic);

    FullRLNCEncoder(std::vector<uint8_t> data, int pieceCountOrSize, T sig, bool generateSystematic, bool fromSize = false);

    FullRLNCEncoder();
};


#endif
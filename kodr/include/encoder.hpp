#pragma once

#include "data.hpp"
#include <mcl/bls12_381.hpp>
#include <vector>
#include <string>
#include <signature.hpp>

#ifndef ENCODER_HPP
#define ENCODER_HPP

typedef struct FullRLNCEncoder
{
    std::vector<std::vector<Fr>> pieces;
    Signature *sig;
    bool useSystematic;
    int pieceIndex;

    int PieceCount();

    int PieceSize();

    int DecodableLen();

    int CodedPieceLen();

    CodedPiece getCodedPiece();

    FullRLNCEncoder(std::vector<std::vector<Fr>> pieces, Signature *sig, bool generateSystematic);

    FullRLNCEncoder(std::vector<uint8_t> data, int pieceCountOrSize, Signature *sig, bool generateSystematic, bool fromSize = false);

    FullRLNCEncoder();
} FullRLNCEncoder;

void AggregateHash(G1 &P, std::vector<Fr> &vec, std::vector<Fr> &codingVec, std::vector<G1> &gens,
                   const std::string &id, int pieceID = -1);

#endif
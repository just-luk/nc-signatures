#pragma once

#include "data.hpp"
#include <mcl/bls12_381.hpp>
#include <vector>
#include <string>

#ifndef ENCODER_HPP
#define ENCODER_HPP

typedef struct FullRLNCEncoder
{
    std::vector<std::vector<Fr>> pieces;
    std::vector<G1> generators;
    std::string id;
    bool useSystematic;
    int pieceIndex;
    Fr secret;

    int PieceCount();

    int PieceSize();

    int DecodableLen();

    int CodedPieceLen();

    CodedPiece getCodedPiece();

    FullRLNCEncoder(std::vector<std::vector<Fr>> pieces, std::string id,
                    Fr secret, std::vector<G1> gens, bool generateSystematic);

    FullRLNCEncoder(std::vector<uint8_t> data, int pieceCountOrSize,
                    std::string id, Fr secret, std::vector<G1> gens, bool generateSystematic, bool fromSize = false);

    FullRLNCEncoder();
} FullRLNCEncoder;

void AggregateHash(G1 &P, std::vector<Fr> &vec, std::vector<Fr> &codingVec, std::vector<G1> &gens,
                   const std::string &id, int pieceID = -1);

#endif
#pragma once

#include <data.hpp>
#include <mcl/bls12_381.hpp>
#include <vector>
#include <string>

#ifndef ENCODER_HPP
#define ENCODER_HPP

typedef struct FullRLNCEncoder {
    std::vector <std::vector<Fr>> pieces;
    std::vector <G1> generators;
    int count, codedPieceCount;
    std::string id;
    Fr secret;

    int PieceCount();

    int PieceSize();

    int DecodableLen();

    int CodedPieceLen();

    CodedPiece getCodedPiece();

    FullRLNCEncoder(std::vector <std::vector<Fr>> pieces, std::string id,
                    Fr secret, std::vector <G1> gens, int cCount);

    FullRLNCEncoder(std::vector<unsigned char> data, int pieceCountOrSize,
                    std::string id, Fr secret, std::vector <G1> gens, bool fromSize, int cCount);
} FullRLNCEncoder;

void AggregateHash(G1 &P, std::vector <Fr> &vec, std::vector <Fr> &idVec, std::vector <G1> &gens,
                   const std::string &id);

#endif
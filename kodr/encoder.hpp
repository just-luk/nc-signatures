#pragma once

#include <data.hpp>
#include <mcl/bls12_381.hpp>
#include <vector>
#include <string>

#ifndef ENCODER_HPP
#define ENCODER_HPP

typedef struct FullRLNCEncoder {
    std::vector <std::vector<Fr>> pieces;
    int extra, count;
    std::string id;
    Fr secret;

    int PieceCount();

    int PieceSize();

    int DecodableLen();

    int CodedPieceLen();

    int Padding();

    CodedPiece getCodedPiece();

    FullRLNCEncoder(std::vector <std::vector<Fr>> pieces, std::string id,
                    Fr secret);

    FullRLNCEncoder(std::vector<unsigned char> data, int pieceCountOrSize,
                    std::string id, Fr secret, bool fromSize);
} FullRLNCEncoder;

void AggregateHash(G1 &P, std::vector <Fr> &vec, const std::string &id);

#endif
# pragma once

#include <mcl/bn256.hpp>
#include <data.hpp>
#include <string>

#ifndef ENCODER_HPP
#define ENCODER_HPP

typedef struct FullRLNCEncoder {
     std::vector<std::vector<Fr>> pieces;
     uint extra;
     std::string id;
     Fr secret;
     uint PieceCount();
     uint PieceSize();
     uint DecodableLen();
     uint CodedPieceLen();
     uint Padding();
     CodedPiece getCodedPiece();
     FullRLNCEncoder(std::vector<std::vector<Fr>> pieces, std::string id, Fr secret);
     FullRLNCEncoder(std::vector<unsigned char> data, uint pieceCountOrSize, std::string id, Fr secret, bool fromSize);
} FullRLNCEncoder;
void AggregateHash(G1& P, std::vector<Fr>& vec, const std::string& id);

#endif
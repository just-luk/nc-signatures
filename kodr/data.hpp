#pragma once

#include <mcl/bn256.hpp>
#include <vector>

#ifndef DATA_HPP
#define DATA_HPP

using namespace mcl::bn256;

std::vector<Fr> multiply(std::vector<Fr> piece1, std::vector<Fr> piece2, Fr by);
struct CodedPiece {
  std::vector<Fr> piece;
  std::vector<Fr> codingVector;
  G1 signature;
  CodedPiece(std::vector<Fr> p, std::vector<Fr> v, G1 s);
  int len();
  std::vector<Fr> flatten();
  bool isSystematic();
};
std::vector<Fr> generateCodingVector(int n);
std::vector<std::vector<Fr>>
OriginalPiecesWithCountAndSize(std::vector<unsigned char> data, int pieceCount,
                               int pieceSize);
std::vector<std::vector<Fr>>
OriginalPiecesFromDataAndPieceCount(std::vector<unsigned char> data,
                                    int pieceCount);
std::vector<std::vector<Fr>>
OriginalPiecesFromDataAndPieceSize(std::vector<unsigned char> data,
                                   int pieceSize);

#endif
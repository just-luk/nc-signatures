#pragma once

#include <mcl/bls12_381.hpp>
#include <vector>

#ifndef DATA_HPP
#define DATA_HPP

using namespace mcl::bls12;

std::vector<Fr> multiply(std::vector<Fr> piece1, std::vector<Fr> piece2, Fr by);

template <typename T>
struct CodedPiece
{
    std::vector<Fr> piece;
    std::vector<Fr> codingVector;
    T signature;

    CodedPiece(std::vector<Fr> p, std::vector<Fr> v, T s);

    CodedPiece(std::vector<uint8_t> &bytes, const int &pieceSize, const int &codingVectorSize);

    CodedPiece();

    int dataLen();

    int fullLen();

    std::vector<uint8_t> toBytes();

    std::vector<Fr> flatten();
};

std::vector<Fr> generateCodingVector(int n);
std::vector<Fr> generateSystematicVector(int idx, int n);

std::vector<std::vector<Fr>>
OriginalPiecesWithCountAndSize(std::vector<uint8_t> data, int pieceCount, int pieceSize);

std::vector<std::vector<Fr>> OriginalPiecesFromDataAndPieceCount(std::vector<uint8_t> data, int pieceCount);

std::vector<std::vector<Fr>> OriginalPiecesFromDataAndPieceSize(std::vector<uint8_t> data, int pieceSize);

std::string RandomString(int length);

#endif
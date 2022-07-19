#pragma once

#include <mcl/bls12_381.hpp>
#include <vector>

#ifndef DATA_HPP
#define DATA_HPP

using namespace mcl::bls12;

std::vector<Fr> multiply(std::vector<Fr> piece1, std::vector<Fr> piece2, Fr by);

struct CodedPiece
{
    std::vector<Fr> piece;
    std::vector<Fr> codingVector;
    G1 signature;

    CodedPiece(std::vector<Fr> p, std::vector<Fr> v, G1 s);

    CodedPiece(std::vector<unsigned char> &bytes, int &pieceSize, int &codingVectorSize);

    CodedPiece();

    int dataLen();

    int fullLen();

    std::vector<unsigned char> toBytes();

    std::vector<Fr> flatten();
};

std::vector<Fr> generateCodingVector(int n);
std::vector<Fr> generateSystematicVector(int idx, int n);

std::vector<std::vector<Fr>>
OriginalPiecesWithCountAndSize(std::vector<unsigned char> data, int pieceCount, int pieceSize);

std::vector<std::vector<Fr>> OriginalPiecesFromDataAndPieceCount(std::vector<unsigned char> data, int pieceCount);

std::vector<std::vector<Fr>> OriginalPiecesFromDataAndPieceSize(std::vector<unsigned char> data, int pieceSize);

#endif
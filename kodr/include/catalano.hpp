#pragma once

#include <mcl/bls12_381.hpp>
#include <vector>
#include <string>
#include "data.hpp"

#ifndef CAT_HPP
#define CAT_HPP

using namespace mcl::bls12;

typedef struct CatSignature
{
    G1 X;
    Fr s;
} CatSignature;

class Catalano
{
    private:
        // public key components
        Fr fid;
        G1 g;
        G1 h;
        G2 gPrime;
        G2 bigZ;
        std::vector<G1> hVec;
        std::vector<G1> gVec;

        // secret key components
        Fr z;

        G1 AggregateHash(Fr secret, std::vector<Fr> &vec, std::vector<Fr> &codingVec);

    public:
        Catalano(uint8_t numPieces, uint8_t pieceSize, Fr fileID);
        Catalano();
        CatSignature Sign(std::vector<Fr> &vec, std::vector<Fr> &codingVec);
        CatSignature Combine(std::vector<CatSignature> &signs, std::vector<Fr> &coeffs);
        bool Verify(CodedPiece<CatSignature> &encodedPiece);
};

#endif
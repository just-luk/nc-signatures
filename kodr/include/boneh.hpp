#pragma once

#include <mcl/bls12_381.hpp>
#include <vector>
#include <string>
#include "data.hpp"

#ifndef BONEH_HPP
#define BONEH_HPP

using namespace mcl::bls12;

class Boneh
{
private:
    G2 h;
    Fr alpha;
    G2 u;
    std::vector<G1> generators;
    std::string id;
    void AggregateHash(G1 &P, std::vector<Fr> &vec, std::vector<Fr> &codingVec);

public:
    Boneh(int pieceSize, std::string fileName);
    Boneh();
    G1 Sign(std::vector<Fr> &vec, std::vector<Fr> &codingVec);
    G1 Combine(std::vector<G1> &signs, std::vector<Fr> &coeffs);
    bool Verify(CodedPiece<G1> &encodedPiece);
};

#endif

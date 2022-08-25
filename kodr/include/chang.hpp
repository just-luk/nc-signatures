#pragma once

#include <mcl/bls12_381.hpp>
#include <vector>
#include <string>
#include "data.hpp"

#ifndef CHANG_HPP
#define CHANG_HPP

using namespace mcl::bls12;

class Chang
{
private:
    G2 h;
    Fr alpha;
    G2 u;
    std::vector<G1> generators;
    std::string id;
    void AggregateHash(G1 &P, std::vector<Fr> &vec, std::vector<Fr> &codingVec, bool encoding);
    void Hash(G1 &out, std::string &id, uint8_t &index, bool encoding);

public:
    Chang(int pieceSize, std::string fileName);
    Chang();
    G1 Sign(std::vector<Fr> &vec, std::vector<Fr> &codingVec);
    G1 Combine(std::vector<G1> &signs, std::vector<Fr> &coeffs);
    bool Verify(CodedPiece<G1> &encodedPiece);
};

#endif

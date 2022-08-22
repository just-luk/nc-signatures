#pragma once

#include <mcl/bls12_381.hpp>
#include <vector>
#include <string>
#include "data.hpp"

#ifndef LI_HPP
#define LI_HPP

using namespace mcl::bls12;

class Li
{
    private:
        G1 g;
        G2 h;
        Fr s;
        G2 mpk;
        std::vector<uint8_t> fileIDBytes;
        std::vector<uint8_t> nodeIDBytes;
        
        // secret key components
        Fr sk;
        G2 big_r;

        Fr h0(std::vector<uint8_t> &inputBytes, G2 &element);
        G1 h1(std::vector<uint8_t> &inputBytes, uint8_t &index);
        Fr h2(std::vector<uint8_t> &inputBytes, uint8_t &index, std::vector<uint8_t> &extraBytes, G2 &element);
        bool verifyPrivateKey();
        G1 AggregateHash(std::vector<Fr> &vec, std::vector<Fr> &codingVec);

    public:
        Li(std::string nodeID, std::string fileName);
        Li();
        G1 Sign(std::vector<Fr> &vec, std::vector<Fr> &codingVec);
        G1 Combine(std::vector<G1> &signs, std::vector<Fr> &coeffs);
        bool Verify(CodedPiece &encodedPiece);
};

#endif
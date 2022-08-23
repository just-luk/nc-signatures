#pragma once

#include <mcl/bls12_381.hpp>
#include <vector>
#include <string>
#include "data.hpp"

#ifndef ZHANG_HPP
#define ZHANG_HPP

using namespace mcl::bls12;

class Zhang
{
    private:
        G2 h;
        Fr msk;
        G2 hz;
        std::vector<uint8_t> nodeIDBytes;
        std::vector<uint8_t> fileIDBytes;

        // secret key components
        Fr y;
        G2 hr;

        Fr h0(G2 &element, std::vector<uint8_t> &extraBytes);
        G1 h1(std::vector<uint8_t> &inputBytes, uint8_t &index);
        G1 AggregateHash(std::vector<Fr> &vec, std::vector<Fr> &codingVec);
        
    public:
        Zhang(std::string nodeID, std::string fileName);
        Zhang();
        G1 Sign(std::vector<Fr> &vec, std::vector<Fr> &codingVec);
        G1 Combine(std::vector<G1> &signs, std::vector<Fr> &coeffs);
        bool Verify(CodedPiece &encodedPiece);
};

#endif
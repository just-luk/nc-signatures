#pragma once

#include "data.hpp"
#include <mcl/bls12_381.hpp>
#include <vector>
#include <string>
#include <data.hpp>

#ifndef SIGNATURE_HPP
#define SIGNATURE_HPP

class Signature
{
public:
    ~Signature() = default;
    virtual G1 Sign(std::vector<Fr> &vec, std::vector<Fr> &codingVec, int pieceID = -1) = 0;
    virtual G1 Combine(std::vector<G1> &signs, std::vector<Fr> &coeffs) = 0;
    virtual bool Verify(CodedPiece &encodedPiece) = 0;
};

#endif
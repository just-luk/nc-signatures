#include <mcl/bls12_381.hpp>
#include <vector>
#include <zhang.hpp>
#include <random>
#include <assert.h>

Zhang::Zhang(std::string nodeID, std::string fileName)
{
    mapToG2(h, rand());
    msk.setRand();
    hz = h * msk;
    fileIDBytes = std::vector<uint8_t>(fileName.begin(), fileName.end());
    nodeIDBytes = std::vector<uint8_t>(nodeID.begin(), nodeID.end());
    
    Fr r;
    r.setRand();
    hr = h * r;
    y = r + msk * h0(hr, nodeIDBytes);
}

Zhang::Zhang(){};

Fr Zhang::h0(G2 &element, std::vector<uint8_t> &extraBytes)
{
    Fr result;
    std::string tempString = element.getStr(mcl::IoSerialize);
    std::vector<uint8_t> inputBytes(tempString.begin(), tempString.end());
    std::vector<uint8_t> combined(inputBytes.size() + extraBytes.size());
    std::copy(inputBytes.begin(), inputBytes.end(), combined.begin());
    std::copy(extraBytes.begin(), extraBytes.end(), combined.begin() + inputBytes.size());
    result.setHashOf(combined.data(), combined.size());
    return result;
}

G1 Zhang::h1(std::vector<uint8_t> &inputBytes, uint8_t &index)
{
    G1 result;
    std::vector<uint8_t> inputBytesWithExtra(inputBytes.size() + 1);
    std::copy(inputBytes.begin(), inputBytes.end(), inputBytesWithExtra.begin());
    inputBytesWithExtra[inputBytes.size()] = index;
    hashAndMapToG1(result, inputBytesWithExtra.data(), inputBytesWithExtra.size());
    return result;
}

G1 Zhang::AggregateHash(std::vector<Fr> &vec, std::vector<Fr> &codingVec) {
    G1 sig;
    mapToG1(sig, 1);
    std::vector<Fr> fullVector(vec.size() + codingVec.size());
    std::copy(vec.begin(), vec.end(), fullVector.begin());
    std::copy(codingVec.begin(), codingVec.end(), fullVector.begin() + vec.size());
    std::vector<G1> hashes(fullVector.size());
    for (uint8_t i = 0; i < fullVector.size(); i++) {
        hashes[i] = h1(fileIDBytes, i);
    }
    G1::mulVec(sig, hashes.data(), fullVector.data(), fullVector.size());
    return sig;
}

G1 Zhang::Sign(std::vector<Fr> &vec, std::vector<Fr> &codingVec) {
    G1 sig = AggregateHash(vec, codingVec);
    sig *= y;
    return sig;
}

G1 Zhang::Combine(std::vector<G1> &signs, std::vector<Fr> &coeffs)
{
    G1 sig;
    G1::mulVec(sig, signs.data(), coeffs.data(), signs.size());
    return sig;
}

bool Zhang::Verify(CodedPiece<G1> &encodedPiece)
{
   Fp12 e1, e2;
   pairing(e1, encodedPiece.signature, h);
   Fr hzExp = h0(hr, nodeIDBytes);
   G2 part2 = hr + (hz * hzExp);
   G1 hashed = AggregateHash(encodedPiece.piece, encodedPiece.codingVector);
   pairing(e2, hashed, part2);
   return e1 == e2;
}

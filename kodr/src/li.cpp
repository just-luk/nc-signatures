#include <mcl/bls12_381.hpp>
#include <vector>
#include <li.hpp>
#include <random>
#include <assert.h>

Li::Li(std::string nodeID, std::string fileName)
{
    mapToG1(g, rand());
    mapToG2(h, rand());
    s.setRand();
    G2::mul(mpk, h, s);
    fileIDBytes = std::vector<uint8_t>(fileName.begin(), fileName.end());
    Fr r;
    r.setRand();
    big_r = h * r;
    nodeIDBytes = std::vector<uint8_t>(nodeID.begin(), nodeID.end());
    sk = r + s * h0(nodeIDBytes, big_r);
    assert(verifyPrivateKey());
}

Li::Li(){};

Fr Li::h0(std::vector<uint8_t> &inputBytes, G2 &element)
{
    Fr result;
    std::string tmp = element.getStr(mcl::IoSerialize);
    std::vector<uint8_t> tmpBytes(tmp.begin(), tmp.end());
    std::vector<uint8_t> inputBytesWithElement(inputBytes.size() + tmpBytes.size());
    std::copy(inputBytes.begin(), inputBytes.end(), inputBytesWithElement.begin());
    std::copy(tmpBytes.begin(), tmpBytes.end(), inputBytesWithElement.begin() + inputBytes.size());
    result.setHashOf(inputBytesWithElement.data(), inputBytesWithElement.size());
    return result;
}

G1 Li::h1(std::vector<uint8_t> &inputBytes, uint8_t &index)
{
    G1 result;
    std::vector<uint8_t> inputBytesWithIndex(inputBytes.size() + 1);
    std::copy(inputBytes.begin(), inputBytes.end(), inputBytesWithIndex.begin());
    inputBytesWithIndex[inputBytes.size()] = index;
    hashAndMapToG1(result, inputBytesWithIndex.data(), inputBytesWithIndex.size());
    return result;
}

Fr Li::h2(std::vector<uint8_t> &inputBytes, uint8_t &index, std::vector<uint8_t> &extraBytes, G2 &element)
{
    Fr result;
    std::string tmp = element.getStr(mcl::IoSerialize);
    std::vector<uint8_t> tmpBytes(tmp.begin(), tmp.end());
    std::vector<uint8_t> combined(inputBytes.size() + 1 + extraBytes.size() + tmpBytes.size());
    std::copy(inputBytes.begin(), inputBytes.end(), combined.begin());
    combined[inputBytes.size()] = index;
    std::copy(extraBytes.begin(), extraBytes.end(), combined.begin() + inputBytes.size() + 1);
    std::copy(tmpBytes.begin(), tmpBytes.end(), combined.begin() + inputBytes.size() + 1 + extraBytes.size());
    result.setHashOf(combined.data(), combined.size());
    return result;
}

bool Li::verifyPrivateKey()
{
    Fp12 e1, e2;
    G1 tmp1 = g * sk;
    G2 tmp2 = big_r + (mpk * h0(nodeIDBytes, big_r));
    pairing(e1, tmp1, h);
    pairing(e2, g, tmp2);
    return e1 == e2;
}

G1 Li::AggregateHash(std::vector<Fr> &vec, std::vector<Fr> &codingVec)
{
    G1 result;
    mapToG1(result, 1);
    std::vector<Fr> fullVec(vec.size() + codingVec.size());
    std::copy(vec.begin(), vec.end(), fullVec.begin());
    std::copy(codingVec.begin(), codingVec.end(), fullVec.begin() + vec.size());
    std::vector<G1> g1Hashes(codingVec.size());
    for(uint8_t i = 0; i < g1Hashes.size(); i++)
    {
        g1Hashes[i] = h1(fileIDBytes, i);
    }
    G1::mulVec(result, g1Hashes.data(), fullVec.data(), g1Hashes.size());
    Fr msgExp = 0;
    for(uint8_t j = 0; j < vec.size(); j++)
    {
        msgExp += h2(nodeIDBytes, j, fileIDBytes, big_r) * fullVec[j];
    }
    result += g * msgExp;
    return result;
}

G1 Li::Sign(std::vector<Fr> &vec, std::vector<Fr> &codingVec)
{
    
    G1 sig = AggregateHash(vec, codingVec);
    sig *= sk;
    return sig;
}

G1 Li::Combine(std::vector<G1> &signs, std::vector<Fr> &coeffs)
{
    G1 sig;
    G1::mulVec(sig, signs.data(), coeffs.data(), signs.size());
    return sig;
};

bool Li::Verify(CodedPiece &codedPiece)
{
    Fp12 e1, e2;
    pairing(e1, codedPiece.signature, h);
    G2 tmp = big_r + (mpk * h0(nodeIDBytes, big_r));
    G1 hashed = AggregateHash(codedPiece.piece, codedPiece.codingVector);
    pairing(e2, hashed, tmp);
    return e1 == e2;
}
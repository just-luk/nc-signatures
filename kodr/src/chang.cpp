#include <mcl/bls12_381.hpp>
#include <vector>
#include <chang.hpp>
#include <random>

Chang::Chang(int pieceSize, std::string fileName)
{
    id = fileName + RandomString(6);
    generators = std::vector<G1>(pieceSize);
    mapToG2(h, rand());
    alpha.setRand();
    G2::mul(u, h, alpha);
    for (int i = 0; i < generators.size(); i++)
    {
        mapToG1(generators[i], rand());
    }
}

Chang::Chang(){};

void Chang::AggregateHash(G1 &P, std::vector<Fr> &vec, std::vector<Fr> &codingVec, bool encoding)
{
    std::vector<G1> fullPoints(generators.size() + codingVec.size());
    std::vector<Fr> fullVec(vec.size() + codingVec.size());
    std::copy(vec.begin(), vec.end(), fullVec.begin());
    std::copy(codingVec.begin(), codingVec.end(), fullVec.begin() + vec.size());
    std::copy(generators.begin(), generators.end(), fullPoints.begin());
    for (uint8_t i = 0; i < codingVec.size(); i++)
    {
        Hash(fullPoints[i + generators.size()], id, i, encoding);
    }
    G1::mulVec(P, fullPoints.data(), fullVec.data(), fullVec.size());
}

void Chang::Hash(G1 &out, std::string &id, uint8_t &index, bool encoding)
{
    G2 extra;
    if (encoding) {
        extra = h * alpha;
    } else {
        extra = u;
    }
    std::string combined = id + std::to_string(index) + extra.getStr(mcl::IoSerialize);
    hashAndMapToG1(out, combined);
}

G1 Chang::Sign(std::vector<Fr> &vec, std::vector<Fr> &codingVec)
{
    G1 sig;
    AggregateHash(sig, vec, codingVec, true);
    G1::mul(sig, sig, alpha);
    return sig;
}

G1 Chang::Combine(std::vector<G1> &signs, std::vector<Fr> &coeffs)
{
    G1 sig;
    G1::mulVec(sig, signs.data(), coeffs.data(), signs.size());
    return sig;
}

bool Chang::Verify(CodedPiece<G1> &encodedPiece)
{
    Fp12 e1, e2;
    G1 hashed;
    AggregateHash(hashed, encodedPiece.piece, encodedPiece.codingVector, false);
    pairing(e1, encodedPiece.signature, h); // e1 = e(signature, h)
    pairing(e2, hashed, u);                 // e2 = e(hashed, u)
    return e1 == e2;
}
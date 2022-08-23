#include <mcl/bls12_381.hpp>
#include <vector>
#include <boneh.hpp>
#include <random>

Boneh::Boneh(int pieceSize, std::string fileName)
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

Boneh::Boneh(){};

void Boneh::AggregateHash(G1 &P, std::vector<Fr> &vec, std::vector<Fr> &codingVec)
{
    std::vector<G1> fullPoints(generators.size() + codingVec.size());
    std::vector<Fr> fullVec(vec.size() + codingVec.size());
    std::copy(vec.begin(), vec.end(), fullVec.begin());
    std::copy(codingVec.begin(), codingVec.end(), fullVec.begin() + vec.size());
    std::copy(generators.begin(), generators.end(), fullPoints.begin());
    for (int i = 0; i < codingVec.size(); i++)
    {
        hashAndMapToG1(fullPoints[i + generators.size()], id + std::to_string(i));
    }
    G1::mulVec(P, fullPoints.data(), fullVec.data(), fullVec.size());
}

G1 Boneh::Sign(std::vector<Fr> &vec, std::vector<Fr> &codingVec)
{
    G1 sig;
    AggregateHash(sig, vec, codingVec);
    G1::mul(sig, sig, alpha);
    return sig;
}

G1 Boneh::Combine(std::vector<G1> &signs, std::vector<Fr> &coeffs)
{
    G1 sig;
    G1::mulVec(sig, signs.data(), coeffs.data(), signs.size());
    return sig;
}

bool Boneh::Verify(CodedPiece &encodedPiece)
{
    Fp12 e1, e2;
    G1 hashed;
    AggregateHash(hashed, encodedPiece.piece, encodedPiece.codingVector);
    pairing(e1, encodedPiece.signature, h); // e1 = e(signature, h)
    pairing(e2, hashed, u);                 // e2 = e(hashed, u)
    return e1 == e2;
}
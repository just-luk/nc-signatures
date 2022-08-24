#include <mcl/bls12_381.hpp>
#include <vector>
#include <catalano.hpp>
#include <random>

Catalano::Catalano(uint8_t numPieces, uint8_t pieceSize, Fr fileID)
{
    fid = fileID;
    mapToG1(g, rand());
    z.setRand();
    mapToG2(gPrime, rand());
    bigZ = gPrime * z;

    mapToG1(h, rand());
    hVec.resize(numPieces);
    gVec.resize(pieceSize);
    for (int i = 0; i < numPieces; i++)
    {
        mapToG1(hVec[i], rand());
    }
    for (int i = 0; i < pieceSize; i++)
    {
        mapToG1(gVec[i], rand());
    }
}

Catalano::Catalano(){};

G1 Catalano::AggregateHash(Fr secret, std::vector<Fr> &vec, std::vector<Fr> &codingVec) {
    G1 multiExp1;
    G1 multiExp2;
    G1::mulVec(multiExp1, hVec.data(), codingVec.data(), codingVec.size());
    G1::mulVec(multiExp2, gVec.data(), vec.data(), vec.size());
    G1 sig = h * secret + multiExp1 + multiExp2;
    return sig;
}

CatSignature Catalano::Sign(std::vector<Fr> &vec, std::vector<Fr> &codingVec)
{
    Fr s;
    s.setRand();
    Fr bigExp = 1 / (fid + z);

    G1 X = AggregateHash(s, vec, codingVec);
    X = X * bigExp;
    return CatSignature{X, s};
}

CatSignature Catalano::Combine(std::vector<CatSignature> &signs, std::vector<Fr> &coeffs)
{
    G1 newX;
    Fr newS = 0;
    std::vector<G1> XVec(signs.size());
    for(int i = 0; i < signs.size(); i++) {
        XVec[i] = signs[i].X;
        newS += signs[i].s * coeffs[i];
    }
    G1::mulVec(newX, XVec.data(), coeffs.data(), coeffs.size());
    return CatSignature{newX, newS};
}

bool Catalano::Verify(CodedPiece<CatSignature> &encodedPiece)
{
    Fp12 e1, e2;
    pairing(e1, encodedPiece.signature.X, bigZ + (gPrime * fid));
    G1 hashed = AggregateHash(encodedPiece.signature.s, encodedPiece.piece, encodedPiece.codingVector);
    pairing(e2, hashed, gPrime);
    return e1 == e2;
}
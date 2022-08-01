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

void Boneh::AggregateHash(G1 &P, std::vector<Fr> &vec, std::vector<Fr> &codingVec, int pieceID)
{
    if (pieceID != -1)
    {
        G1 hashedID;
        hashAndMapToG1(hashedID, id + std::to_string(pieceID));
        G1::mulVec(P, generators.data(), codingVec.data(), codingVec.size());
        P += hashedID;
    }
    else
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
}

std::string Boneh::RandomString(int length)
{
 std::string CHARACTERS = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

    std::random_device random_device;
    std::mt19937 generator(random_device());
    std::uniform_int_distribution<> distribution(0, CHARACTERS.size() - 1);

    std::string random_string;

    for (int i = 0; i < length; ++i)
    {
        random_string += CHARACTERS[distribution(generator)];
    }

    return random_string;
}

G1 Boneh::Sign(std::vector<Fr> &vec, std::vector<Fr> &codingVec, int pieceID)
{
    G1 sig;
    AggregateHash(sig, vec, codingVec, pieceID);
    G1::mul(sig, sig, alpha);
    return sig;
}

G1 Boneh::Combine(std::vector<G1> &signs, std::vector<Fr> &coeffs)
{
    G1 sig;
    G1::mulVec(sig, signs.data(), coeffs.data(), coeffs.size());
    return sig;
};

bool Boneh::Verify(CodedPiece &encodedPiece)
{
    Fp12 e1, e2;
    G1 hashed;
    if (encodedPiece.isSystematic)
    {
        int index = 0;
        for (int i = 0; i < encodedPiece.codingVector.size(); i++)
        {
            if (encodedPiece.codingVector[i].isOne())
            {
                index = i;
                break;
            }
        }
        AggregateHash(hashed, encodedPiece.piece, encodedPiece.codingVector, index);
    }
    else
    {
        AggregateHash(hashed, encodedPiece.piece, encodedPiece.codingVector, -1);
    }
    pairing(e1, encodedPiece.signature, h); // e1 = e(signature, h)
    pairing(e2, hashed, u);                 // e2 = e(hashed, u)
    return e1 == e2;
}
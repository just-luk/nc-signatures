#include <data.hpp>
#include <encoder.hpp>
#include <mcl/bls12_381.hpp>
#include <vector>
#include <string>

FullRLNCEncoder::FullRLNCEncoder(std::vector<std::vector<Fr>> pieces,
                                 std::string id, Fr secret, std::vector<G1> gens, bool generateSystematic)
{
    this->pieces = pieces;
    this->id = id;
    this->secret = secret;
    this->generators = gens;
    this->useSystematic = generateSystematic;
    this->pieceIndex = 0;
}

FullRLNCEncoder::FullRLNCEncoder(std::vector<uint8_t> data,
                                 int pieceCountOrSize, std::string id,
                                 Fr secret, std::vector<G1> gens, bool generateSystematic, bool fromSize)
{
    this->id = id;
    this->secret = secret;
    this->generators = gens;
    this->useSystematic = generateSystematic;
    this->pieceIndex = 0;
    if (fromSize)
    {
        this->pieces = OriginalPiecesFromDataAndPieceSize(data, pieceCountOrSize);
    }
    else
    {
        this->pieces = OriginalPiecesFromDataAndPieceCount(data, pieceCountOrSize);
    }
}

FullRLNCEncoder::FullRLNCEncoder(){};

int FullRLNCEncoder::PieceCount() { return pieces.size(); }

int FullRLNCEncoder::PieceSize() { return pieces[0].size(); }

int FullRLNCEncoder::DecodableLen() { return PieceCount() * CodedPieceLen(); }

int FullRLNCEncoder::CodedPieceLen() { return PieceCount() + PieceSize(); }

void AggregateHash(G1 &P, std::vector<Fr> &vec, std::vector<Fr> &codingVec, std::vector<G1> &gens,
                   const std::string &id, int pieceID)
{
    if (pieceID != -1)
    {
        G1 hashedID;
        hashAndMapToG1(hashedID, id + std::to_string(pieceID));
        G1::mulVec(P, gens.data(), codingVec.data(), codingVec.size());
        P += hashedID;
    }
    else
    {
        std::vector<G1> fullPoints(gens.size() + codingVec.size());
        std::vector<Fr> fullVec(vec.size() + codingVec.size());
        std::copy(vec.begin(), vec.end(), fullVec.begin());
        std::copy(codingVec.begin(), codingVec.end(), fullVec.begin() + vec.size());
        std::copy(gens.begin(), gens.end(), fullPoints.begin());
        for (int i = 0; i < codingVec.size(); i++)
        {
            hashAndMapToG1(fullPoints[i + gens.size()], id + std::to_string(i));
        }
        G1::mulVec(P, fullPoints.data(), fullVec.data(), fullVec.size());
    }
}

void Sign(G1 &sign, const Fr &secret, std::vector<Fr> &vec, std::vector<Fr> &codingVec, std::vector<G1> &gens,
          const std::string &id, int pieceID = -1)
{
    AggregateHash(sign, vec, codingVec, gens, id, pieceID);
    G1::mul(sign, sign, secret);
}

CodedPiece FullRLNCEncoder::getCodedPiece()
{
    std::vector<Fr> codingVec;
    std::vector<Fr> piece;
    G1 signature;
    if (useSystematic && pieceIndex < PieceCount())
    {
        codingVec = generateSystematicVector(pieceIndex, PieceCount());
        piece = pieces[pieceIndex];
        pieceIndex++;
        Sign(signature, secret, piece, codingVec, this->generators, id, pieceIndex - 1);
    }
    else
    {
        codingVec = generateCodingVector(PieceCount());
        piece = std::vector<Fr>(PieceSize(), 0);
        for (int i = 0; i < PieceCount(); i++)
        {
            piece = multiply(piece, pieces[i], codingVec[i]);
        }
        Sign(signature, secret, piece, codingVec, this->generators, id);
    }
    return CodedPiece(piece, codingVec, signature);
}
#include <data.hpp>
#include <encoder.hpp>
#include <mcl/bls12_381.hpp>
#include <vector>
#include <string>

FullRLNCEncoder::FullRLNCEncoder(std::vector <std::vector<Fr>> pieces,
                                 std::string id, Fr secret, std::vector <G1> gens, int cCount) {
    this->pieces = pieces;
    this->id = id;
    this->count = 0;
    this->secret = secret;
    this->generators = gens;
    this->codedPieceCount = cCount;
}

FullRLNCEncoder::FullRLNCEncoder(std::vector<unsigned char> data,
                                 int pieceCountOrSize, std::string id,
                                 Fr secret, std::vector <G1> gens, bool fromSize, int cCount) {
    this->id = id;
    this->secret = secret;
    this->count = 0;
    this->codedPieceCount = cCount;
    this->generators = gens;
    if (fromSize) {
        this->pieces = OriginalPiecesFromDataAndPieceSize(data, pieceCountOrSize);
    } else {
        this->pieces = OriginalPiecesFromDataAndPieceCount(data, pieceCountOrSize);
    }
}

int FullRLNCEncoder::PieceCount() { return pieces.size(); }

int FullRLNCEncoder::PieceSize() { return pieces[0].size(); }

int FullRLNCEncoder::DecodableLen() { return PieceCount() * CodedPieceLen(); }

int FullRLNCEncoder::CodedPieceLen() { return PieceCount() + PieceSize(); }

void AggregateHash(G1 &P, std::vector <Fr> &vec, std::vector <Fr> &codingVec, std::vector <G1> &gens,
                   const std::string &id) {
    std::vector <G1> fullPoints(gens.size() + codingVec.size());
    std::vector <Fr> fullVec(vec.size() + codingVec.size());
    std::copy(vec.begin(), vec.end(), fullVec.begin());
    std::copy(codingVec.begin(), codingVec.end(), fullVec.begin() + vec.size());
    std::copy(gens.begin(), gens.end(), fullPoints.begin());
    for (int i = 0; i < codingVec.size(); i++) {
        hashAndMapToG1(fullPoints[i + gens.size()], id + std::to_string(i));
    }
    G1::mulVec(P, fullPoints.data(), fullVec.data(), fullVec.size());
}

void Sign(G1 &sign, const Fr &secret, std::vector <Fr> &vec, std::vector <Fr> &codingVec, std::vector <G1> &gens,
          const std::string &id) {
    AggregateHash(sign, vec, codingVec, gens, id);
    G1::mul(sign, sign, secret);
}

CodedPiece FullRLNCEncoder::getCodedPiece() {
    std::vector <Fr> codingVec = generateCodingVector(PieceCount());
    std::vector <Fr> piece = std::vector<Fr>(PieceSize(), 0);
    for (int i = 0; i < PieceCount(); i++) {
        piece = multiply(piece, pieces[i], codingVec[i]);
    }
    G1 signature;
    Sign(signature, secret, piece, codingVec, this->generators, id);
    count++;
    return CodedPiece(piece, codingVec, signature);
}
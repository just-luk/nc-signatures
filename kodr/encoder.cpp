#include <data.hpp>
#include <encoder.hpp>
#include <mcl/bls12_381.hpp>
#include <vector>
#include <string>

FullRLNCEncoder::FullRLNCEncoder(std::vector <std::vector<Fr>> pieces,
                                 std::string id, Fr secret) {
    this->pieces = pieces;
    this->id = id;
    this->secret = secret;
}

FullRLNCEncoder::FullRLNCEncoder(std::vector<unsigned char> data,
                                 int pieceCountOrSize, std::string id,
                                 Fr secret, bool fromSize) {
    this->id = id;
    this->secret = secret;
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

int FullRLNCEncoder::Padding() { return extra; }

void AggregateHash(G1 &P, std::vector <Fr> &vec, const std::string &id) {
    G1 hashedID;
    hashAndMapToG1(hashedID, id);
    std::vector <G1> hashVec(vec.size(), hashedID);
    G1::mulVecMT(P, hashVec.data(), vec.data(), vec.size(), 0);
}

void Sign(G1 &sign, const Fr &secret, std::vector <Fr> &vector,
          const std::string &id) {
    AggregateHash(sign, vector, id);
    G1::mul(sign, sign, secret);
}

CodedPiece FullRLNCEncoder::getCodedPiece() {
    std::vector <Fr> vec = generateCodingVector(PieceCount());
    std::vector <Fr> piece = std::vector<Fr>(PieceSize(), 0);
    for (int i = 0; i < PieceCount(); i++) {
        piece = multiply(piece, pieces[i], vec[i]);
    }
    G1 signature;
    Sign(signature, secret, piece, id);
    return CodedPiece(piece, vec, signature);
}
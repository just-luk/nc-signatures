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

void AggregateHash(G1 &P, std::vector <Fr> &vec, std::vector <Fr> &idVec, std::vector <G1> &gens,
                   const std::string &id) {
    G1 hashedID;
    hashedID.clear();
    for (int i = 0; i < idVec.size(); i++) {
        if (!idVec[i].isZero()) {
            G1 tmp;
            hashAndMapToG1(tmp, id + std::to_string(i));
            hashedID += tmp * idVec[i];
        }
    }
    G1::mulVec(P, gens.data(), vec.data(), vec.size());
    P += hashedID;
}

void Sign(G1 &sign, const Fr &secret, std::vector <Fr> &vec, std::vector <Fr> &idVec, std::vector <G1> &gens,
          const std::string &id) {
    AggregateHash(sign, vec, idVec, gens, id);
    G1::mul(sign, sign, secret);
}

CodedPiece FullRLNCEncoder::getCodedPiece() {
    std::vector <Fr> codingVec = generateCodingVector(PieceCount());
    std::vector <Fr> piece = std::vector<Fr>(PieceSize(), 0);
    std::vector <Fr> idVec = std::vector<Fr>(codedPieceCount, 0);
    for (int i = 0; i < PieceCount(); i++) {
        piece = multiply(piece, pieces[i], codingVec[i]);
    }
    G1 signature;
    idVec[count] = 1;
    Sign(signature, secret, piece, idVec, this->generators, id);
    count++;
    return CodedPiece(piece, idVec, codingVec, signature);
}
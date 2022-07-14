#include <data.hpp>
#include <decoder.hpp>
#include <decoder_state.hpp>
#include <mcl/bls12_381.hpp>
#include <vector>
#include <stdexcept>
#include <vector>

FullRLNCDecoder::FullRLNCDecoder(int pieceCount) {
    expected = pieceCount;
    useful = 0;
    received = 0;
    state = DecoderState(pieceCount);
}

int FullRLNCDecoder::PieceLength() {
    if (received > 0) {
        return state.CodedMatrix().cols;
    }
    return 0;
}

bool FullRLNCDecoder::IsDecoded() { return useful >= expected; }

int FullRLNCDecoder::Required() { return expected - useful; }

void FullRLNCDecoder::addPiece(CodedPiece piece, bool isFirst) {
    if (IsDecoded()) {
        throw std::runtime_error("All useful pieces have been received!");
    }
    state.AddPiece(piece, isFirst);
    received++;
    if (received <= 1) {
        useful++;
        return;
    }

    state.Rref();
    useful = state.Rank();
}

std::vector <Fr> FullRLNCDecoder::getPiece(int i) { return state.GetPiece(i); }

std::vector<unsigned char> FullRLNCDecoder::getData() {
    if (!IsDecoded()) {
        throw std::runtime_error("More useful pieces are required!");
    }
    std::vector<unsigned char> pieces;
    std::vector <Fr> tempPiece;
    std::vector<unsigned char> tempVec;
    std::string tempString;
    for (int i = 0; i < useful; i++) {
        tempPiece = getPiece(i);
        for (int j = 0; j < tempPiece.size(); j++) {
            tempString = tempPiece[j].getStr(mcl::IoSerialize);
            tempVec = std::vector<unsigned char>(tempString.begin(), tempString.end());
            pieces.push_back(tempVec[0]);
        }
    }
    int len = pieces.size() - 1;
    while (pieces[len] == 0) {
        len--;
    }
    pieces.resize(len + 1);
    return pieces;
}
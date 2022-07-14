#include <data.hpp>
#include <decoder.hpp>
#include <decoder_state.hpp>
#include <mcl/bls12_381.hpp>
#include <stdexcept>

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

void FullRLNCDecoder::AddPiece(CodedPiece piece, bool isFirst) {
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

std::vector<Fr> FullRLNCDecoder::GetPiece(int i) { return state.GetPiece(i); }

std::vector<std::vector<Fr>> FullRLNCDecoder::GetPieces() {
  if (!IsDecoded()) {
    throw std::runtime_error("More useful pieces are required!");
  }
  std::vector<std::vector<Fr>> pieces(useful);
  for (int i = 0; i < useful; i++) {
    pieces[i] = GetPiece(i);
  }
  return pieces;
}
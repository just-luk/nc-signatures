#include <data.hpp>
#include <math.h>
#include <mcl/bn256.hpp>
#include <vector>

using namespace mcl::bn256;

std::vector<Fr> multiply(std::vector<Fr> piece1, std::vector<Fr> piece2,
                         Fr by) {
  for (int i = 0; i < piece1.size(); i++) {
    piece1[i] += piece2[i] * by;
  }
  return piece1;
}

CodedPiece::CodedPiece(std::vector<Fr> p, std::vector<Fr> v, G1 s) {
  piece = p;
  codingVector = v;
  signature = s;
}

int CodedPiece::len() { return piece.size() + codingVector.size(); }

std::vector<Fr> CodedPiece::flatten() {
  std::vector<Fr> ret;
  ret.insert(ret.begin(), piece.begin(), piece.end());
  ret.insert(ret.end(), codingVector.begin(), codingVector.end());
  return ret;
}

bool CodedPiece::isSystematic() {
  int pos = -1;
  for (int i = 0; i < codingVector.size(); i++) {
    Fr el = codingVector[i];
    if (el.isZero()) {
      continue;
    } else if (el.isOne()) {
      if (pos != -1) {
        return false;
      }
      pos = i;
    } else {
      return false;
    }
  }
  return pos >= 0 && pos < codingVector.size();
}

std::vector<Fr> generateCodingVector(int n) {
  std::vector<Fr> ret(n);
  for (int i = 0; i < n; i++) {
    ret[i].setRand();
  }
  return ret;
}

std::vector<std::vector<Fr>>
OriginalPiecesWithCountAndSize(std::vector<unsigned char> data, int pieceCount,
                               int pieceSize) {
  data.resize(pieceSize * pieceCount, 0);
  std::vector<std::vector<Fr>> ret(pieceCount);
  for (int i = 0; i < pieceCount; i++) {
    ret[i].resize(pieceSize);
    copy(data.begin() + i * pieceSize, data.begin() + (i + 1) * pieceSize,
         ret[i].begin());
  }
  return ret;
}

std::vector<std::vector<Fr>>
OriginalPiecesFromDataAndPieceCount(std::vector<unsigned char> data,
                                    int pieceCount) {
  int size = data.size();
  int pieceSize = ceil(size / pieceCount);
  return OriginalPiecesWithCountAndSize(data, pieceCount, pieceSize);
}

std::vector<std::vector<Fr>>
OriginalPiecesFromDataAndPieceSize(std::vector<unsigned char> data,
                                   int pieceSize) {
  int size = data.size();
  int pieceCount = ceil(size / pieceSize);
  return OriginalPiecesWithCountAndSize(data, pieceCount, pieceSize);
}
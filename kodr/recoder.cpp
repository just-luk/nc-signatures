#include <data.hpp>
#include <matrix.hpp>
#include <mcl/bls12_381.hpp>
#include <recoder.hpp>

FullRLNCRecoder::FullRLNCRecoder(std::vector<CodedPiece> ps) {
  this->pieces = ps;
  this->pieceCount = ps.size();
  fill();
}

// Fill the coding matrix with the coded this->pieces.
void FullRLNCRecoder::fill() {
  std::vector<std::vector<Fr>> data(
      this->pieceCount, std::vector<Fr>(this->pieces[0].codingVector.size()));
  Matrix m(data);
  for (int i = 0; i < this->pieceCount; i++) {
    m.data[i] = this->pieces[i].codingVector;
  }
  this->codingMatrix = m;
}

CodedPiece FullRLNCRecoder::getCodedPiece() {
  std::vector<Fr> vec = generateCodingVector(this->pieceCount);
  std::vector<Fr> pc(this->pieces[0].piece.size(), 0);
  std::vector<G1> sigs(this->pieceCount);

  for (int i = 0; i < this->pieceCount; i++) {
    sigs[i] = this->pieces[i].signature;
    pc = multiply(pc, this->pieces[i].piece, vec[i]);
  }

  G1 out;
  G1::mulVecMT(out, sigs.data(), vec.data(), sigs.size(), 4);

  std::vector<std::vector<Fr>> tempVec(1, vec);
  Matrix tempMat(tempVec);
  tempMat = tempMat.Multiply(this->codingMatrix);

  vec = tempMat.data[0];
  return CodedPiece(pc, vec, out);
}
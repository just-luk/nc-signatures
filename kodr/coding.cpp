#include <data.hpp>
#include <decoder.hpp>
#include <encoder.hpp>
#include <fstream>
#include <ios>
#include <iostream>
#include <math.h>
#include <mcl/bn256.hpp>
#include <vector>

using namespace mcl::bn256;

std::vector<unsigned char> readFile(const char *filename) {
  // open the file:
  std::streampos fileSize;
  std::ifstream file(filename, std::ios::binary);

  // get its size:
  file.seekg(0, std::ios::end);
  fileSize = file.tellg();
  file.seekg(0, std::ios::beg);

  // read the data:
  std::vector<unsigned char> fileData(fileSize);
  file.read((char *)&fileData[0], fileSize);
  return fileData;
}

void KeyGen(Fr &alpha, G2 &pub, G2 &h) {
  mapToG2(h, rand());
  alpha.setRand();
  G2::mul(pub, h, alpha);
}

bool Verify(const G1 &sign, const G2 &u, const G2 &h, std::vector<Fr> &vector,
            const std::string &id) {
  Fp12 e1, e2;
  G1 hashed;
  AggregateHash(hashed, vector, id);
  pairing(e1, sign, h);   // e1 = e(sign, Q)
  pairing(e2, hashed, u); // e2 = e(Hm, sQ)
  return e1 == e2;
}

int main() {
  srand(unsigned(time(NULL)));
  initPairing();
  std::vector<unsigned char> fileData = readFile("logo.png");

  Fr alpha;
  G2 u, h;
  KeyGen(alpha, u, h);
  std::cout << "secret key " << std::endl;
  std::cout << "\talpha = " << alpha << std::endl;
  std::cout << "public key " << std::endl;
  std::cout << "\th = " << h << std::endl;
  std::cout << "\tu = " << u << std::endl;

  int pieceCount = 64;
  int codedPieceCount = pieceCount * 2;
  int droppedPieceCount = pieceCount / 2;

  FullRLNCEncoder encoder(fileData, pieceCount, "logo.png123", alpha, false);

  std::vector<CodedPiece> codedPieces;
  for (int i = 0; i < codedPieceCount; i++) {
    codedPieces.push_back(encoder.getCodedPiece());
  }
  // print and verify the coded pieces
  for (int i = 0; i < codedPieceCount; i++) {
    std::cout << "coded piece " << i + 1 << std::endl;
    std::cout << "\t" << codedPieces[i].signature << std::endl;
    bool verified = Verify(codedPieces[i].signature, u, h, codedPieces[i].piece,
                           "logo.png123");
    std::cout << "\t" << (verified ? "verified" : "not verified") << std::endl;
  }

  std::random_shuffle(codedPieces.begin(), codedPieces.end());

  FullRLNCDecoder decoder(pieceCount);
  decoder.AddPiece(codedPieces[0], true);
  for (int i = 1; i < pieceCount; i++) {
    decoder.AddPiece(codedPieces[i], false);
  }

  std::vector<std::vector<Fr>> decodedPieces = decoder.GetPieces();
  std::cout << "decoded pieces " << decodedPieces.size() << std::endl;
  for (int i = 0; i < decodedPieces.size(); i++) {
    for (int j = 0; j < decodedPieces[0].size(); j++) {
      std::cout << "[" << decodedPieces[i][j] << "] ";
    }
    std::cout << std::endl;
  }
}